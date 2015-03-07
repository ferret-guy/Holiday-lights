#include "AdafruitNECIR.h"
#include <Adafruit_NeoPixel.h>



// ****************************************** Defines and Globals Go Here

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

#define NUMMODES          6        //Number of modes, including off.
#define NUMLENGTHS        3        //Number of preset lengths.
#define MAXLEDS           300      //Specified maximum number of LEDS.

//Define our globals.
int       led_num         = 45;    //Total number of LEDs, zero-ordinated.
int       cur_len         = 0;     //The current length preset.
int       col_tot         = 11;    //Total colors defined. This will almost certainly be 11, but there's nothing wrong with a little open-endedness.
int       col_len         = 0;     //The length of each color segment. This is calculated later.
float     col_fln         = 0;     //The floating-point color length.
int       col_seg         = 0;     //The length of each separation mode segment.
int       col_num         = 0;     //The number of active/enabled colors. This is calculated later.
int       col_rep         = 0;     //The number of times to draw the colors in color separation mode 2+.
int       ren_stp         = 0;     //The number of iterations the rendering loop goes through before resetting. This is calculated later.
int       ren_stp_b       = 256;   //Rainbow steps.
int       ren_glx         = 0;     //Useful for resuming color separation mode as opposed to restarting the animation.
int       ren_glx_b       = 0;     //Useful for rainbows.
int       ren_bri         = 255;   //Global brightness multiplier. 255 is fullbright, 0 is blackout.
int       sep_mod         = 1;     //The current separation mode.
int       sep_prv         = 1;     //The previous separation mode.
int       sep_dly         = 100;   //The current animation frame delay.
int       absolute_offset = 0;     //The number of pixels to skip when drawing. Artifact from an old devkit.
int       cmd_last        = 0;     //The last IR code recieved.
int       cmd_prev        = 0;     //The one before that.
int       mode_state      = 0;     //The current mode.
int       mode_last       = 1;     //The mode before we turned off. Also lets us turn on to sepmod.
int       ani_dir         = 0;     //Direction of animation progression, 0 is forward, 1 is reverse.
int       efc_col         = 3;     //Current color for Twinkle and Blink

//Initialize our strip. There needs to be a way to clear this out, because when additional physical LED strips are added, the length can only be changed by redefining the strip.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAXLEDS+absolute_offset+1, PIN, NEO_GRB + NEO_KHZ800);
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

//Alternatively, for twinkle...
int colorparts[11][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255},
  {255,255,255},
  {255,100,0},
  {255,255,0},
  {128,0,128},
  {255,178,48},
  {175,238,238},
  {255,0,128},
  {255,200,25}
};

//Predefined, switchable strip lengths.
int lengths[] = {
  45,
  144,
  189
};

//MODE TABLE - Important stuff.
void (*modes[])() = {
  *ModeOffHndlr,
  *ModeSepModHndlr,
  *ModeSolidHndlr,
  *ModeRainbowHndlr,
  *ModeBlinkHndlr,
  *ModeTwinkleHndlr
};
  

// ****************************************** General Purpose Functions Go Here

//Literally Nothing
void dust(){}

//Nothing For Your Something
void dust2(int cs_s){}

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

//Adafruit colorwheel function... no need to reinvent the wheel.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//Increment or decrement a variable to progress animation.
int progressAnimation(int input){
  int output;
  //Serial.println("got called");
  //Serial.println(input);
  if(ani_dir==1){
    //Serial.println("minus minus");
    output = input-1;
  }else{
    //Serial.println("plus plus");
    output = input+1;
  }
  //Serial.println(output);
  return output;
}



// ****************************************** Renderers Go Here

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

//Predraw all the pixels so separation mode doesn't fill over initial black.
void SPMPreRender(int offs){
  for(int x=offs;x<col_len+offs;x++){
    RenderSPM(x);
  }
}

//Do a rainbow.
void RenderRainbow(uint16_t j) {
  uint16_t i;
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
}

//Twinkly.
void RenderTwinkle(int index){
  int n;
  for(n=0;n<(led_num/4);n++){
    float r = (float) colorparts[index][0];
    //float r = 255;
    float g = (float) colorparts[index][1];
    //float g = 0;
    float b = (float) colorparts[index][2];
    //float b = 0;
    float di = (float) random(100);
    float d = (di/125)+0.2;
    r = r*d;
    g = g*d;
    b = b*d;
    int ri = (int) r;
    int gi = (int) g;
    int bi = (int) b;
    uint32_t color = strip.Color(ri,gi,bi);
    strip.setPixelColor(random(led_num),color);
  }
  strip.show();
}
    


// ****************************************** Button Handlers Go Here (Real IDE Goes Here)

//Handle mode button in sepmod
void SepModModeHndlr(){
  if (sep_mod == 3){sep_mod = 0;}
  else {sep_mod++;}
  ren_glx = 0;
}

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

void BlinkTwinkleColorHndlr(int index){
  efc_col=index;
}

void SolidColorHndlr(int index){
  FillStripC(colors[index]);
}

void SolidModeHndlr(){
  cur_len++;
  if(cur_len==NUMLENGTHS){cur_len=0;}
  led_num=lengths[cur_len];
  initStrips(color_enable,sep_mod);
}



// ****************************************** Top-Level Handlers Go Here

//Single byte command processor. This is split out so that we can take commands during ANY MODE with ease. Handlers for mode-specific events are passed in.
int SingleByteCommand(int csig, void (*colHndlr)(int), void (*modHndlr)(), bool volatileColors){
  int gret=0;
  
  //colHndlr is called when any color button is pressed, parameter passed is color number as per array up top.
  //modHndlr is called when mode button is pressed, no paramter is passed.
  
  switch(csig){
    //DO COLORS
    //Red
    case 9:
      colHndlr(0);
      if(!volatileColors){gret=1;}
      break;
      
    //Green
    case 8:
      colHndlr(1);
      if(!volatileColors){gret=1;}
      break;
    
    //Blue
    case 10:
      colHndlr(2);
      if(!volatileColors){gret=1;}
      break;
  
    //White
    case 11: 
      colHndlr(3);
      if(!volatileColors){gret=1;}
      break;
    
    //Orange
    case 13:
      colHndlr(4);
      if(!volatileColors){gret=1;}
      break;
    
    //Yellow
    case 12:
      colHndlr(5);
      if(!volatileColors){gret=1;}
      break;
    
    //Purple
    case 14:
      colHndlr(6);
      if(!volatileColors){gret=1;}
      break;
    
    //Warm White
    case 15:
      colHndlr(7);
      if(!volatileColors){gret=1;}
      break;
    
    //Turquoise
    case 20:
      colHndlr(8);
      if(!volatileColors){gret=1;}
      break;
    
    //Magenta
    case 22:
      colHndlr(9);
      if(!volatileColors){gret=1;}
      break;
    
    //Amber
    case 21:
      colHndlr(10);
      if(!volatileColors){gret=1;}
      break;
    
    //DO ADJUSTMENTS
    //Speed Down
    case 27:
      sep_dly+=15;
      if(sep_dly>250){
        sep_dly=250;
      }
      gret=1;
      break;
    
    //Speed Up
    case 23:
      sep_dly-=15;
      if(sep_dly<25){
        sep_dly=25;
      }
      gret=1;
      break;
    
    //Brightness Down
    case 4:
      ren_bri-=15;
      if(ren_bri<15){
        ren_bri=15;
      }
      gret=1;
      strip.setBrightness(ren_bri);
      break;
    
    //Brightness Up
    case 5:
      ren_bri+=15;
      if(ren_bri>255){
        ren_bri=255;
      }
      gret=1;
      strip.setBrightness(ren_bri);
      break;
    
    //DO THESE MISCELLANEOUS STUFF
    //Mode Button Handler
    case 24:
      modHndlr();
      break;
    
    //Someone held down the button, and we haven't implemented that yet.
    case -3:
      gret=1;
      break;
    
    //Reverse the animation progression.
    case 19:
      if(ani_dir==1){
        ani_dir=0; //forward
      }else{
        ani_dir=1; //reverse
      }
      gret=1;
      break;
    
    //DO MODES
    //Turn On
    case 7:
      if(mode_state!=0){gret=1;}
      if(mode_last==0){mode_last=1;}
      mode_state=mode_last;
      break;
    
    //Off Mode
    case 6:
      if(mode_state!=0){
        mode_last=mode_state;
      }
      mode_state=0;
      break;
    
    //Separation Mode
    case 26:
      mode_state=1;
      break;
    
    //Solid Mode
    case 25:
      mode_state=2;
      break;
    
    //Rainbow Mode
    case 17:
      mode_state=3;
      break;
    
    //Blink Mode
    case 16:
      mode_state=4;
      break;
    
    //Twinkle Mode
    case 18:
      mode_state=5;
      break;
      
    default:
      gret=1;
      break;
    
  }  
  return gret;
}

//Accept commands and be blank.
void ModeOffHndlr(){
  FillStrip(0,0,0);
  strip.show();
  while(1==1){
    cmd_last=remote.listen(250);
    int cret=SingleByteCommand(cmd_last,*dust2,*dust,false);
    if(cret==0){break;}
  }
}

//Run separation mode.
void ModeSepModHndlr(){
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
    //for(int x=ren_glx;x<ren_stp;progressAnimation(x)){
      //Serial.println("start");
      //Serial.println(ren_glx);
      int x = ren_glx;
      //Render it!
      RenderSPM(x);
      
      x = progressAnimation(x);
      //Keep track of where we are so that we can pause and resume.
      ren_glx=x;
      //Delay the animation by the selected amount, and get an IR command. Two stones with one bird.
      cmd_last = remote.listen(sep_dly);
      //Serial.println("progresses");
      //Serial.println(ren_glx);
      int cret = 1;
      if(cmd_last>0){cret=SingleByteCommand(cmd_last,*colenable,*SepModModeHndlr,true);}//Run the IR command.
      //Serial.println(cret);
      //if(cmd_last==6||cmd_last==25){i=0;FillStrip(0,0,0);strip.show();break;}//If the command is right, break from the sepmod loop.
      if(cret==0){initStrips(color_enable,sep_mod);ren_glx=col_len;SPMPreRender(0);break;}//Reset and restart sepmod for necessary changes
      cmd_last = 0;//Make sure we don't re-run commands.
    //}
    //If we've made a full render, go back to rendering step zero.
    if(ani_dir==1){
      if(ren_glx==0){
        ren_glx=(ren_stp-1);
      }
    }else{
      if(ren_glx==(ren_stp-1)){
        ren_glx=0;
      }
    }
  }
}

//Does solid colors.
void ModeSolidHndlr(){
  initStrips(color_enable,sep_mod);
  int i=1;
  FillStrip(255,255,255);
  while(i==1){
    cmd_last=remote.listen(250);
    int cret = SingleByteCommand(cmd_last,*SolidColorHndlr,*SolidModeHndlr,false);
    strip.show();
    if(cret==0){i=0;break;}
    cmd_prev=cmd_last;
  }
  //FillStrip(0,0,0);
  //strip.show();
}

//Make it rainbow.
void ModeRainbowHndlr(){
  //Re-intialize the strip with the selected parameters.
  initStrips(color_enable,sep_mod);
  //Initialize our breaking var.
  int i=1;

  //Keep going until told to stop.
  while(i==1){
      int x = ren_glx_b;
      //Render it!
      RenderRainbow(x);
      x = progressAnimation(x);
      //Keep track of where we are so that we can pause and resume.
      ren_glx_b=x;
      //Delay the animation by the selected amount, and get an IR command. Two stones with one bird.
      cmd_last = remote.listen(sep_dly);
      int cret = 1;
      if(cmd_last>0){cret=SingleByteCommand(cmd_last,*dust2,*dust,false);}//Run the IR command.
      if(cret==0){break;}//Reset and restart sepmod for necessary changes
      cmd_last = 0;//Make sure we don't re-run commands.
    //If we've made a full render, go back to rendering step zero.
    if(ani_dir==1){
      if(ren_glx==0){
        ren_glx=(ren_stp_b-1);
      }
    }else{
      if(ren_glx==(ren_stp_b-1)){
        ren_glx=0;
      }
    }
  }
}

void ModeBlinkHndlr(){
  //Re-intialize the strip with the selected parameters.
  initStrips(color_enable,sep_mod);
  //Initialize our breaking var.
  int it=1;

  //Keep going until told to stop.
  while(it==1){
    for (int q=0; q < 2; q++) {
      for (int i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, colors[efc_col]);
      }
      strip.show();
      //Delay the animation by the selected amount, and get an IR command. Two stones with one bird.
      cmd_last = remote.listen(sep_dly);
      int cret = 1;
      if(cmd_last>0){cret=SingleByteCommand(cmd_last,*BlinkTwinkleColorHndlr,*dust,false);}//Run the IR command.
      if(cret==0){it=0;break;}//Reset and restart sepmod for necessary changes
      cmd_last = 0;//Make sure we don't re-run commands.
      for (int i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, 0);
      }
    }
  }
}

//Twinkle like stars.
void ModeTwinkleHndlr(){
  //Re-intialize the strip with the selected parameters.
  initStrips(color_enable,sep_mod);
  //Initialize our breaking var.
  int i=1;

  //Keep going until told to stop.
  while(1==1){
      RenderTwinkle(efc_col);
      cmd_last = remote.listen(sep_dly);
      int cret = 1;
      if(cmd_last>0){cret=SingleByteCommand(cmd_last,*BlinkTwinkleColorHndlr,*dust,false);}//Run the IR command.
      if(cret==0){break;}//Reset and restart sepmod for necessary changes
      cmd_last = 0;//Make sure we don't re-run commands.
  }
}

//This runs first, setting up the necessary things.
void setup(void) {
  //Do everything relevant to the LEDs.
  initStrips(color_enable,sep_mod);
  //Open our serial connection to the PC.
  Serial.begin(9600);
}

//The main chunk of the program.
void loop(void) {
  (*modes[mode_state])(); // Exciting, right?
}

