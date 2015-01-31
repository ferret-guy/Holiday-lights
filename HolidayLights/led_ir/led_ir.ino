#include <Adafruit_NECremote.h>
#include <Adafruit_NeoPixel.h>

//Pin the strip is on.
#define PIN 11
//Pin the IR Sensor is on.
#define IRpin 2

//Serial Definitions
#define STARTBIT          253
#define STOPBIT           254
#define FILLSTRIPSIG      44
#define UPDATECOLORSIG    50
#define PREVIEWPIXELSIG   60
#define SETPIXELSIG       61
#define CLEARPIXELSIG     62
#define CHANGECOLSIG      30
#define SEPMODESIG        40
#define KILLSEPBIT        31

//Define our globals.
int       led_num         = 59;    //Total number of LEDs, zero-ordinated.
int       col_tot         = 11;    //Total colors defined. This will almost certainly be 11, but there's nothing wrong with a little open-endedness.
int       col_len         = 0;     //The length of each color segment. This is calculated later.
float     col_fln         = 0;     //The floating-point color length.
int       col_seg         = 0;     //The length of each separation mode segment.
int       col_num         = 0;     //The number of active/enabled colors. This is calculated later.
int       col_rep         = 0;     //The number of times to draw the colors in color separation mode 2+.
int       ren_stp         = 0;     //The number of iterations the rendering loop goes through before resetting. This is calculated later.
int       ren_glx         = 0;     //Useful for resuming color separation mode as opposed to restarting the animation.
int       ren_bri         = 255;   //Global brightness multiplier. 255 is fullbright, 0 is blackout.
int       sep_mod         = 1;     //The current separation mode.
int       sep_prv         = 1;     //The previous separation mode.
int       sep_dly         = 100;   //The current animation frame delay.
int       absolute_offset = 0;     //The number of pixels to skip when drawing. Artifact from an old devkit.
int       cmd_last        = 0;     //The last IR code recieved.
int       mode_state      = 0;     //Used to assist in switching modes.

//Initialize our strip. There needs to be a way to clear this out, because when additional physical LED strips are added, the length can only be changed by redefining the strip.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(led_num+absolute_offset+1, PIN, NEO_GRB + NEO_KHZ800);
//Initialize our IR sensor.
Adafruit_NECremote remote(IRpin);

//Colors:
//0.  Red
//1.  Green
//2.  Blue
//3.  White
//4.  Orange
//5.  Yellow
//6.  Purple
//7.  Warm White
//8.  Turquoise
//9.  Magenta
//10. Amber

//Mask of enabled colors.
boolean color_enable[] = {1,1,1,1,1,1,1,1,1,1,1};
//Offset of each color in color separation mode, and possibly other modes.
int     color_offset[] = {0,1,1,2,2,3,3,4,4,5,5};
//The color definitions themselves.
uint32_t colors[] = {
  strip.Color(255,0,0),      //Red
  strip.Color(0,255,0),      //Green
  strip.Color(0,0,255),      //Blue
  strip.Color(255,255,255),  //White
  strip.Color(255,100,0),    //Orange
  strip.Color(255,255,0),    //Yellow
  strip.Color(128,0,128),    //Purple
  strip.Color(255,178,48),   //Warm White
  strip.Color(175,238,238),  //Turquoise
  strip.Color(255,0,128),    //Magenta
  strip.Color(255,200,25)    //Amber
};

void colenable(int index){
  if(col_num>2){
    if (color_enable[index]==1){
      color_enable[index]=0;
    }else{
      color_enable[index]=1;
    }
    ren_glx = 0;
  }else{color_enable[index]=1;}
}


//Calculate the length of each color segment based on the length of the strip and the separation mode. (This is col_len.)
int CalcCL(int spm){
  //Preset the color length to one, in case separation mode is zero.
  int ccl = 1;
  //If separation mode is non-zero...
  if(spm>0){
    //The color length is calculated.
    if(col_num>0){
      ccl = ceil(((led_num/spm)/col_num));
      if(ccl<1){ccl=1;}
      //Calculate the floating-point color length.
      col_fln = (led_num/spm)/col_num;
      col_seg = (int) (ccl*col_num);
      col_rep = led_num/col_seg;
    }else{ccl=1;}
  }
  return ccl;
}

//Calculate the number of colors enabled. (This is col_num.)
int CalcCN(boolean cle[]){
  int cur=0;
  //Iterate through the color_enable mask and add one to the proto-col_num everytime we find a high bit.
  for(int cc=0;cc<col_tot;cc++){
    if(cle[cc]){cur++;}
  }
  if(cur==0){cur=1;}
  return cur;
}

//Initialize our strips(s). This is where strip redefinition would need to happen.
void initStrips(boolean cle[],int spm){
  //Update our globals.
  col_num = CalcCN(cle);
  col_len = CalcCL(spm);
  //Set the number of render steps.
  if(spm>0){
    ren_stp = col_seg;
  }else{
    ren_stp = led_num;
  }
  //Initialize our strip.
  strip.begin();
  strip.setBrightness(ren_bri);
  strip.show();
}

//The separation mode renderer!
void RenderSPM(int offset){
  //Initialize the incrementing variables.
  int rend_step = 1; //NOT TO BE CONFUSED WITH ren_stp.
  int col_step = 0;
  int spm_step = -1;
  //Make sure we have a nonzero number of colors
  if(col_num>0){
    //If separation mode is 1 or higher...
    if(sep_mod>0){
      //This while loop increments spm_step to draw multiple instances of the separation for modes 2 and up.
      while(spm_step<=col_rep){
        col_step=0;
        rend_step=0;
        //This while loop steps through the different colors.
        while(col_step<col_tot){
          //If the color is enabled, render it.
          if(color_enable[col_step]){
            //Calculate what pixel we should be drawing to.                 \/ This is where we move over a lot for separation mode 2+.
            int pixnum=(rend_step*(col_len))+offset+0*color_offset[col_step]+(col_seg*spm_step);
            //Don't draw to pixels lower than the absolute offset.
            if(pixnum<0){
              pixnum=-absolute_offset-1;
            }
            //Draw the pixel in the correct color.
            strip.setPixelColor(pixnum+absolute_offset,colors[col_step]);
            //Go to the next color length.
            rend_step++;
          }
          //Go to the next color.
          col_step++;
        }
        //Draw the next separation instance.
        spm_step++;
      }
    //Draw separation mode 0. We handle this separately as the effect achived is actually the expected result of the separation mode approaching infinity, where as true mode zero would be a single color of infinite length.
    }else{
      //Pretty simple stuff. It won't need changing, ever.
      //Calculate how many times the pattern needs repeating.
      int led_rpt=led_num/col_num;
      //Initialize a thing.
      int pixnum=0;
      //Step through the colors.
      while(col_step<col_tot){
        //If a color is enabled, draw it.
        if(color_enable[col_step]){
          //Repeat it however many times with the offset increased by the total colors.
          for(int i=0;i<led_num;i+=col_num){
            pixnum = col_step+i+offset;
            if(pixnum>led_num){pixnum-=led_num;}
              //Actually draw the pixel.
              strip.setPixelColor(pixnum+absolute_offset,colors[col_step]);
          }
        }
        //Go to the next color.
        col_step++;
      }
    }
  //If there are no colors enabled, draw black, so as to not break everything.
  }else{
    FillStrip(0,0,0);
  }
  //Actually send all these changes to the strip. We do this last for due to major opimization issues, as well as to avoid visually staggered rendering.
  strip.show();
}

//Fill the strip with a color.
void FillStrip(int cr, int cg, int cb){
  //Construct the selected color.
  uint32_t nc=strip.Color(cr,cg,cb);
  //Step through each pixel and set it to the selected color.
    for(int i=0;i<=led_num;i++){
      strip.setPixelColor(i+absolute_offset,nc);
  }
}

//Fill the strip with a predefined color.
void FillStripC(uint32_t nc){
  //Step through each pixel and set it to the selected color.
    for(int i=0;i<=led_num;i++){
      strip.setPixelColor(i+absolute_offset,nc);
  }
}

//Single byte command processor. This is split out so that we can take commands during separation mode with ease.
int SingleByteCommand(int csig){
  int gret=0;
  
  //Red
  if(csig==9){colenable(0);}
  
  //Green
  else if(csig==8){colenable(1);}
  
  //Blue
  else if(csig==10){colenable(2);}
  
  //White
  else if(csig==11){colenable(3);}
  
  //Orange
  else if(csig==13){colenable(4);}
  
  //Yellow
  else if(csig==12){colenable(5);}
  
  //Purple
  else if(csig==14){colenable(6);}
  
  //Warm White
  else if(csig==15){colenable(7);}
  
  //Turquoise
  else if(csig==20){colenable(8);}
  
  //Magenta
  else if(csig==22){colenable(9);}
  
  //Amber
  else if(csig==21){colenable(10);}
  
  //Speed Down
  else if(csig==27){sep_dly+=15;if(sep_dly>250){sep_dly=250;}gret=1;}
  
  //Speed Up
  else if(csig==23){sep_dly-=15;if(sep_dly<10){sep_dly=10;}gret=1;}
  
  //Brightness Down
  else if(csig==4){ren_bri-=15;if(ren_bri<15){ren_bri=15;}gret=1;strip.setBrightness(ren_bri);}
  
  //Brightness Up
  else if(csig==5){ren_bri+=15;if(ren_bri>255){ren_bri=255;}gret=1;strip.setBrightness(ren_bri);}
  
  //Incrment Color sepration mode
  else if(csig==24){
    if (sep_mod == 3){sep_mod = 0;}
    else {sep_mod++;}
    ren_glx = 0;
  }
  
  //Someone held down the button, and we haven't implemented that yet.
  else if(csig==-3){gret=1;}
  
  return gret;
}

int SingleColorCommand(int csig){
  //Red
  if(csig==9){FillStripC(colors[0]);}
  //Green
  else if(csig==8){FillStripC(colors[1]);}
  //Blue
  else if(csig==10){FillStripC(colors[2]);}
  //White
  else if(csig==11){FillStripC(colors[3]);}
  //Orange
  else if(csig==13){FillStripC(colors[4]);}    
  //Yellow
  else if(csig==12){FillStripC(colors[5]);}
  //Purple
  else if(csig==14){FillStripC(colors[6]);}
  //Warm White
  else if(csig==15){FillStripC(colors[7]);}
  //Turquoise
  else if(csig==20){FillStripC(colors[8]);}
  //Magenta
  else if(csig==22){FillStripC(colors[9]);}
  //Amber
  else if(csig==21){FillStripC(colors[10]);}

}

//Predraw all the pixels so separation mode doesn't fill over initial black.
void SPMPreRender(int offs){
  for(int x=offs;x<col_len+offs;x++){
    RenderSPM(x);
  }
}

//Run separation mode from IR.
void InfraredSepRenHandler(){
  //Re-intialize the strip with the selected parameters.
  initStrips(color_enable,sep_mod);
  //Initialize our breaking var.
  int i=1;
  //If we're not resuming separation mode, predraw the colors so we don't have empty pixels visible.
  if(ren_glx==0){
    //Render a single color-length.
    SPMPreRender(0);
    //Update our resume variable for when we actually start rendering.
    ren_glx=col_len;
  }else{
    SPMPreRender(ren_glx-col_len);
  }
  //Keep going until told to stop.
  while(i==1){
    //Run through the calculated number of rendering steps. Start where we stopped.
    for(int x=ren_glx;x<ren_stp;x++){
      //Render it!
      RenderSPM(x);
      //Keep track of where we are so that we can pause and resume.
      ren_glx=x;
      //Delay the animation by the selected amount, and get an IR command. Two stones with one bird.
      cmd_last = remote.listen(sep_dly);
      
      int cret = 1;
      if(cmd_last>0){cret=SingleByteCommand(cmd_last);}//Run the IR command.
      Serial.println(cret);
      if(cmd_last==6||cmd_last==25){i=0;FillStrip(0,0,0);strip.show();break;}//If the command is right, break from the sepmod loop.
      if(cret==0){initStrips(color_enable,sep_mod);ren_glx=col_len;SPMPreRender(0);break;}//Reset and restart sepmod for necessary changes
      cmd_last = 0;//Make sure we don't re-run commands.
    }
    //If we've made a full render, go back to rendering step zero.
    if(ren_glx==(ren_stp-1)){
      ren_glx=0;
    }
  }
}

//Does solid colors
void InfraredSolidHandler(){
  initStrips(color_enable,sep_mod);
  int i=1;
  FillStrip(255,255,255);
  while(i==1){
    cmd_last=remote.listen(250);
    SingleColorCommand(cmd_last);
    strip.show();
    if(cmd_last==6||cmd_last==26){break;}
  }
  FillStrip(0,0,0);
  strip.show();
}
  

//This runs first, setting up the necessary things. Since this is the PC-control setup, there isn't much, and most everything is condensed into initStrips.
void setup(void) {
  //Do everything relevant to the LEDs.
  initStrips(color_enable,sep_mod);
  //Open our serial connection to the PC.
  Serial.begin(9600);
}

//The main chunk of the program.
void loop(void) {
  if(mode_state==0){cmd_last=remote.listen(250);}//Get A Command
  else{mode_state=2;}
  if(cmd_last==26||cmd_last==7){InfraredSepRenHandler();}//run Separations mode
  if(cmd_last==25){InfraredSolidHandler();}//do solid mode
  if(cmd_last>0&&mode_state==0){mode_state=1;}
  if(mode_state==2){mode_state=0;}
}

