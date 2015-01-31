#include <Adafruit_NeoPixel.h>
//Pin the strip is on.
#define PIN 7

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
int       led_num         = 99;    //Total number of LEDs, zero-ordinated. Due to a less than healthy mix of zero- and one-ordination, the best solution is to confuse the other algorithms with an odd number.
int       col_tot         = 11;    //Total colors defined. This will almost certainly be 11, but there's nothing wrong with a little open-endedness.
int       col_len         = 0;     //The length of each color segment. This is calculated later.
float     col_fln         = 0;     //The floating-point color length.
int       col_seg         = 0;     //The length of each separation mode segment.
int       col_num         = 0;     //The number of active/enabled colors. This is calculated later.
int       col_rep         = 0;     //The number of times to draw the colors in color separation mode 2+.
uint32_t  col_lst;                 //The last preset color passed by per-pixel mode. Dirty RAM-saving hack. [NN]
int       ren_stp         = 0;     //The number of iterations the rendering loop goes through before resetting. This is calculated later.
int       ren_lst         = -1;    //The last pixel passed by per-pixel mode. [NN]
boolean   ren_msk[256];            //The preset pixel map for per-pixel mode. Absorbs all your RAM. [NN]
int       ren_glx         = 0;     //Useful for resuming color separation mode as opposed to restarting the animation. Not really necessary.
int       ren_bri         = 255;   //Global brightness multiplier. 255 is fullbright, 0 is blackout.
int       sep_mod         = 1;     //The current separation mode.
int       sep_prv         = 1;     //The previous separation mode.
int       sep_dly         = 100;   //The current animation frame delay.
int       absolute_offset = 4;     //The number of pixels to skip when drawing. Only purpose is to skip the four NeoPixels indev. [NN]

//Initialize our strip. There needs to be a way to clear this out, because when additional physical LED strips are added, the length can only be changed by redefining the strip.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(led_num+absolute_offset+1, PIN, NEO_GRB + NEO_KHZ800);

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
  //Initialize the render mask.
  for(int i=0;i<256;i++){ren_msk[i]=0;}
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

//Single byte command processor. This is split out so that we can take commands during separation mode with ease.
//**********************************************************************************************************************************TODO: I think IR stuff goes here?
int SingleByteCommand(int csig){
  int gret=0;
  
  //Red
  if(csig==120){colenable(0);}
  
  //Green
  else if(csig==122){colenable(1);}
  
  //Blue
  else if(csig==121){colenable(2);}
  
  //White
  else if(csig==110){colenable(3);}
  
  //Orange
  else if(csig==116){colenable(4);}    
 
  //Yellow
  else if(csig==117){colenable(5);}
  
  //Purple
  else if(csig==118){colenable(6);}
  
  //Warm White
  else if(csig==112){colenable(7);}
  
  //Turquoise
  else if(csig==114){colenable(8);}
  
  //Magenta
  else if(csig==108){colenable(9);}
  
  //Amber
  else if(csig==113){colenable(10);}
  
  //Speed Down
  else if(csig==105){sep_dly+=15;if(sep_dly>250){sep_dly=250;}gret=1;}
  
  //Speed Up
  else if(csig==101){sep_dly-=15;if(sep_dly<10){sep_dly=10;}gret=1;}
  
  //Brightness Down
  else if(csig==106){ren_bri-=15;if(ren_bri<10){ren_bri=10;}gret=1;strip.setBrightness(ren_bri);}
  
  //Brightness Up
  else if(csig==104){ren_bri+=15;if(ren_bri>255){ren_bri=255;}gret=1;strip.setBrightness(ren_bri);}
  
  //Incrment Color sepration mode
  else if(csig==109){
    if (sep_mod == 3){sep_mod = 0;}
    else {sep_mod++;}
    ren_glx = 0;
  }
  return gret;
}

void SPMPreRender(int offs){
  for(int x=offs;x<col_len+offs;x++){
    RenderSPM(x);
  }
}

//Run separation mode with specified parameters. All serial commands should be handled like this, but only this one needs to be.
//*************************************************************************************************************************************************TODO: SerialSepRenHandler -> InfraredSepRenHandler in more ways than one
void SerialSepRenHandler(int cth, int ctl, int csm){
  //Construct the selected animation delay in milliseconds. This is a WORD to allow for delays longer than 255ms.
  sep_dly = (int) word(cth,ctl);
  //Construct the selected separation mode.
  sep_mod = (int) csm;
  //If a new separation mode has been selected, we won't be resuming the last render.
  if(sep_mod!=sep_prv){
    //Clear out the resume variable.
    ren_glx=0;
    //Update the previous separation mode to the current one.
    sep_prv=sep_mod;
  }
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
  //Keep going until told to stop. From here on out the code is production-ready, except breaking is IR- and not Serial-triggered.
  while(i==1){
    //Serial.println("Begin render outer loop");
    //Run through the calculated number of rendering steps. Start where we stopped.
    for(int x=ren_glx;x<ren_stp;x++){
      //Render it!
      RenderSPM(x);
      //Keep track of where we are so that we can pause and resume.
      ren_glx=x;
      //Delay the animation by the selected amount.
      delay(sep_dly);
      //Look for the break command.
      //*************************************************************************************************************************************TODO: Refactor in-loop controls?
      int cbrk=Serial.read();
      if(cbrk==STARTBIT){
        delay(3);
        int cbl=Serial.read();
        int cret = 0;
        if(cbl==1){
          delay(1);
          cret = SingleByteCommand(Serial.read());
        }
        if(cret==0){initStrips(color_enable,sep_mod);ren_glx=col_len;SPMPreRender(0);break;}
      }
      if(cbrk==31||cbrk==102){i=0;FillStrip(0,0,0);break;}//If the command is right, break from the sepmod loop.
    }
    //If we've made a full render, go back to rendering step zero.
    if(ren_glx==(ren_stp-1)){
      ren_glx=0;
    }
  }
}

//This runs first, setting up the necessary things. Since this is the PC-control setup, there isn't much, and most everything is condensed into initStrips.
void setup(void) {
  //Do everything relevant to the LEDs.
  initStrips(color_enable,sep_mod);
  //Open our serial connection to the PC.
  Serial.begin(9600);
}

//***************************************************************************************************************************************************************TODO: shit needs refactoring yo
//The main chunk of the program.
void loop(void) {
  //Listen for our variable-length serial packet!
  int  vs = 0;        //Valid serial?
  int  msg_max = 16;  //Max message length.
  byte msg_dat[16];   //[insert message here]
  int  msg_len = 0;   //Current message length.
  //Block until serial is available.
  while(!Serial.available()){}
  delay(10);
  //It should be available, but no one likes changing indents.
  if(Serial.available()){
    //Get the first byte.
    int sb = Serial.read();
    //Let the LED Control Utility catch up, it's slower than a 16MHz AVR.
    delay(10);
    //Check if the first bit is actually the start of a packet.
    if(sb==STARTBIT){
      //Serial.println("Started!");
      //Find out how long the packet will be.
      msg_len = Serial.read();
      //It can't be longer than the max length.
      if(msg_len>msg_max){msg_len=msg_max;}
      //We've received valid serial, so set this to true.
      vs=1;
      //Serial.print("Length: ");
      //Serial.println(msg_len,DEC);
      //Block until whole message is here.
      while(Serial.available()<msg_len){}
      //Read our bytes into msg_dat.
      for(int i=0;i<msg_len;i++){
        sb = Serial.read();
        //Serial.print("Got: ");
        //Serial.println(sb,DEC);
        msg_dat[i]=sb;
        //Again, let the utility catch up.
        delay(1);
      }
      //This is from when there was a start bit and a stop bit, which was stupid because often the data value was equal to the stop bit...
/*    while(st==0){
        sb = Serial.read();
        Serial.print("Got: ");
        Serial.println(sb,DEC);
        if(sb==STOPBIT){
          vs = 1;
          st = 1;
        }else{
          msg_dat[msg_len]=sb;
          msg_len++;
        }
        if(msg_len==msg_max){
          st = 1;
          vs = -1;
        }
        delay(1);
      }*/
      //Serial.println("Stopped!");
    }else{
      //Serial.println("Invalid start!");
    }
  }
  //Debug function, in case an invalid start byte is had.
  if(vs==-1){
    //Serial.println("Invalid message!");
  }
  //If we have received a valid serial packet...
  if(vs==1){       
    //Get the signature/command byte, which tells us what the rest of the data means.
    byte csig;
    csig = msg_dat[0];
    //Multi-byte messages.
    if(msg_len>1){

      //Color update mode. This allows for predefined colors to be updated, so that separation mode can be previewed with different colors, without reuploading.
      else if(csig==50){
        //Get our relevant data.
        byte cr,cg,cb,cn;
        cr = msg_dat[1];
        cg = msg_dat[2];
        cb = msg_dat[3];
        cn = msg_dat[4];
        //Construct the selected color.
        uint32_t nc=strip.Color(cr,cg,cb);
        //Clamp the predefined color number to a valid value.
        if(cn>col_tot){cn=col_tot;}
        if(cn<0){cn=0;}
        //Update the selected predefined color.
        colors[cn]=nc;
      }
      
      //Strip fill mode. This simply fills the entire strip with a single color.
      else if(csig==44){
        //Get our relevant data.
        byte cr,cg,cb;
        cr = msg_dat[1];
        cg = msg_dat[2];
        cb = msg_dat[3];
        //Clearly, we aren't resuming a render. Also, this command is used for clearing the strip, in which case we're definitely not resuming the render.
        ren_glx = 0;
        //Fill the strip.
        FillStrip(cr,cg,cb);
      }
      
      //Separation mode mode! This runs separation mode, rendered exactly like it would be in a production scenario, with some serial parameters.
      else if(csig==40){
        //Get some relevant data.
        byte ctl,cth,csm;
        cth = msg_dat[1];
        ctl = msg_dat[2];
        csm = msg_dat[3];
        Seri
        alSepRenHandler(cth,ctl,csm);
      }
      //Update the enabled colors mask. This sets color x high or low depending on serial parameters.
      else if(csig==30){
        //Become enlightened with relevant data.
        byte ccn,ccs;
        ccn = msg_dat[1];
        ccs = msg_dat[2];
        //Make sure we got a valid command.
        if(ccs==0 || ccs==1){
          //If we're actually changing the color...
          if(color_enable[ccn]!=ccs){
            //Enable or disable the selected color.
            color_enable[ccn]=ccs;
            //Since we're changing colors, we won't be resuming a render.
            ren_glx=0;
          }
        }
      }
      //Update the brightness, without using the remote.
      else if(csig==21){
        byte cbr;
        //Get the selected brightness.
        cbr = msg_dat[1];
        //Update the global brightness.
        ren_bri = cbr;
        //Actually set the brightness.
        strip.setBrightness(cbr);
      }
    }else if(msg_len==1){
      //Separation Mode
      if(csig==100){SerialSepRenHandler(0,sep_dly,sep_mod);}
      else{SingleByteCommand(csig);}
    }

    //Push changes made by serial commands to the strip. Perhaps this should be moved into its own serial command, but for now, no.
    strip.show();
  }
}

