#include <Adafruit_NeoPixel.h>
//The strip is always on pin 7. ALWAYS.
#define PIN 7

//[NN] - Not Necessary. This feature is not for production and probably isn't production-ready. It's just for use in development and probably only works with the LED Control Utility.

//Define our globals. Globals aren't traditionally the best, but it's an AVR with 2KB RAM.
int       led_num         = 99;    //Total number of LEDs, zero-ordinated. Due to a less than healthy mix of zero- and one-ordination, the best solution is to confuse the other algorithms with an odd number.
int       col_tot         = 11;    //Total colors defined. This will almost certainly be 11, but there's nothing wrong with a little open-endedness.
int       col_len         = 0;     //The length of each color segment. This is calculated later.
int       col_num         = 0;     //The number of active/enabled colors. This is calculated later.
uint32_t  col_lst;                 //The last preset color passed by per-pixel mode. Dirty RAM-saving hack. [NN]
int       ren_stp         = 0;     //The number of iterations the rendering loop goes through before resetting. This is calculated later.
int       ren_lst         = -1;    //The last pixel passed by per-pixel mode. [NN]
boolean   ren_msk[256];            //The preset pixel map for per-pixel mode. Absorbs all your RAM. [NN]
int       ren_glx         = 0;     //Useful for resuming color separation mode as opposed to restarting the animation. Not really necessary.
int       sep_mod         = 1;     //The current separation mode.
int       sep_dly         = 100;   //The current animation frame delay.
int       absolute_offset = 4;     //The number of pixels to skip when drawing. Only purpose is to skip the four NeoPixels indev. [NN]

//Initialize our strip. There needs to be a way to clear this out, because when additional physical LED strips are added, the length can only be changed by redefining the strip.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(led_num+absolute_offset+1, PIN, NEO_GRB + NEO_KHZ800);

//Colors:
//1.  Red
//2.  Green
//3.  Blue
//4.  White
//5.  Orange
//6.  Yellow
//7.  Purple
//8.  Warm White
//9.  Magenta
//10. Turquoise
//11. Amber

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
  strip.Color(255,0,255),    //Magenta
  strip.Color(255,200,25)    //Amber
};

//Calculate the length of each color segment based on the length of the strip and the separation mode. (This is col_len.)
int CalcCL(int spm){
  //Preset the color length to one, in case separation mode is zero.
  int ccl = 1;
  //If separation mode is non-zero...
  if(spm>0){
    //The color length is calculated. It might be wise to ditch this whole block of code and replace it with floating-point math.
    //Really, it can't even handle even-number separation mode. It needs major TLC.
    ccl = floor(((led_num/spm)/col_num));
    //Serial.print("CCL: ");
    //Serial.println(ccl,DEC);
    //Total up the moduli (remainders) of the integer division.
    int mcl = led_num % spm;
    mcl += (led_num/spm) % col_num;
    //mcl += ((led_num/spm)/col_num) % 2;
    //Calculate how many colors to process before increasing the offset.
    int mcs = col_tot/mcl;
    //If there is no modulus, set it to zero, because we do not need to set an offset.
    if(mcl==0){mcs=col_tot;}
    //Serial.print("MCS: ");
    //Serial.print(mcs,DEC);
    //Serial.print("; MCL: ");
    //Serial.println(mcl,DEC);
    //Intialize the offset-accumulator and the current offset.
    int mcp = 0;
    int mci = 0;
    for(int i=0;i<col_tot;i++){
      //If the offset-accumulator has reached the color-offset-step, set it to zero and increment the current offset.
      if(mcp>=mcs){
        mcp=0;
        mci++;
      }
      //This just makes it work better. It's an isolated case but still an existing case.
      if(col_num==11&&sep_mod==1){color_offset[i]=mci*4;}
      //Set the offset.
      else{color_offset[i]=mci;}
      //Increment the offset-accumulator.
      mcp++;
    }
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
  return cur;
}

//Initialize out strips(s). This is where strip redefinition would need to happen.
void initStrips(boolean cle[],int spm){
  //Update our globals.
  col_num = CalcCN(cle);
  col_len = CalcCL(spm);
  //Set the number of render steps. For whatever reason (I blame the offsetter) this isn't accurate.
  if(spm>0){
    ren_stp = led_num/spm;
  }else{
    ren_stp = led_num;
  }
  //Test debug please ignore
  //Serial.println("ren_stp:");
  //Serial.println(ren_stp,DEC);
  //Serial.println("col_len:");
  //Serial.println(col_len,DEC);
  //Serial.println("col_num:");
  //Serial.println(col_num,DEC);
  //Initialize the render mask, this is doubly unneccesary but my eyeballs were about to fall out of my head. [NN]
  for(int i=0;i<256;i++){ren_msk[i]=0;}
  //Initialize our strip.
  strip.begin();
  strip.show();
}

//The separation mode renderer! Fun!
void RenderSPM(int offset){
  //Initialize the incrementing variables.
  int col_step = 0;
  int spm_step = 0;
  //If separation mode is 1 or higher...
  if(sep_mod>0){
    //This while loop increments spm_step to draw multiple instances of the separation for modes 2 and up.
    while(spm_step<sep_mod){
      //Serial.println("sep_mod:");
      //Serial.println(sep_mod,DEC);
      //Serial.println("spm_step:");
      //Serial.println(spm_step,DEC);
      //Really, this needs to get reset. Spent hours trying to figure this out.
      col_step=0;
      //This while loop steps through the different colors.
      while(col_step<col_tot){
        //If the color is enabled, render it.
        if(color_enable[col_step]){
          //Hours...
          //Serial.println("col_step:");
          //Serial.println(col_step,DEC);
          //Serial.println("sep_mod:");
          //Serial.println(sep_mod,DEC);
          //Serial.println("col_len:");
          //Serial.println(col_len,DEC);
          //Serial.println("sep_mod:");
          //Serial.println(sep_mod,DEC);
          //Serial.println("offset:");
          //Serial.println(offset,DEC);
          //Serial.println("spm_step:");
          //Serial.println(spm_step,DEC);
          //Serial.println("current color_offset:");
          //Serial.println(color_offset[col_step],DEC);
          //Calculate what pixel we should be drawing to.                \/ This is where we move over a lot for separatio mode 2+.
          int pixnum=(col_step*(col_len))+offset+color_offset[col_step]+(col_len*11*spm_step);
          //Serial.println("pre pixnum:");
          //Serial.println(pixnum,DEC);
          //Roll over from the end of the strip. This could be changed to a if, but other code needs checking first.
          while(pixnum>=led_num){
            pixnum-=led_num;
          }
          //Serial.println("post pixnum:");
          //Serial.println(pixnum,DEC);
          //Draw the pixel in the correct color.
          strip.setPixelColor(pixnum+absolute_offset,colors[col_step]);
          //This  \/  little guy caused quite a few problems. The less modulo the better.
          //offset+=col_len;
          //HOURS.
          //Serial.println("post offset:");
          //Serial.println("who cares, this is what broke it");
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
  //Actually send all these changes to the strip. We do this last for due to major opimization issues, as well as to avoid visually staggered rendering.
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
  //Listen for our 6-byte serial packet.
  if(Serial.available()>=6){
    //Get the signature/command byte, which tells us what the rest of the data means.
    byte csig;
    csig = Serial.read();
    //Per-pixel mode, previewing a change. This moves the selected pixel along the strip and restores preset pixels after passing them. [NN]
    if(csig==60){
      //Read all the relevant data.
      byte cr,cg,cb,lh,ll;
      cr = Serial.read();
      cg = Serial.read();
      cb = Serial.read();
      lh = Serial.read();
      ll = Serial.read();
      //Construct the received color.
      uint32_t nc=strip.Color(cr,cg,cb);
      //Construct the pixel value that is selected. This is a 16-bit number to allow for strips longer than 256 pixels.
      int ln = abs((int) word(lh,ll));
      //Clamp the pixel value to the length of the strip.
      if(ln>led_num){ln=led_num;}
      //If a new pixel is selected, and it is preset, save its color value.
      if(ln!=ren_lst){
        if(ren_msk[ren_lst]==1){
          col_lst=strip.getPixelColor(ren_lst+absolute_offset);
        }
      }
      //If the last pixel that was selected wasn't preset, clear it.
      if(ren_msk[ren_lst]==0){
        strip.setPixelColor(ren_lst+absolute_offset,strip.Color(0,0,0));
      }
      //If the last pixel that was selected was preset, set it back to 
      if(ren_msk[ren_lst]==1){
        strip.setPixelColor(ren_lst+absolute_offset,col_lst);
      }
      //Set the selected pixel to the selected color.
      strip.setPixelColor(ln+absolute_offset,nc);
      //If a new pixel is selected, set it to be the new last pixel for when another new pixel is selected.
      if(ln!=ren_lst){
        ren_lst=ln;
      }
    }
    //Per-pixel mode, presetting a pixel. This is something of a misnomer, as all that is done is indicating that the current color value is permanent, not actually saving the color value. [NN]
    if(csig==61){
      //Get our relevant data.
      byte cr,cg,cb,lh,ll;
      cr = Serial.read();
      cg = Serial.read();
      cb = Serial.read();
      lh = Serial.read();
      ll = Serial.read();
      //Construct the received color.
      uint32_t nc=strip.Color(cr,cg,cb);
      //Construct the pixel value that is selected. This is a 16-bit number to allow for strips longer than 256 pixels.
      int ln = (int) word(lh,ll);
      //Set the preset mask at this pixel value to true.
      ren_msk[ln]=1;
      //Set the last color to the selected color. This a dirty hack so that when a new pixel is previewed, the color is not lost.
      col_lst=nc;
      //Draw the pixel, just to be safe.
      strip.setPixelColor(ln+absolute_offset,nc);
    }
    //Per-pixel mode, un-presetting a pixel. Basically, we stop preserving this color value when it gets passed by a preview. [NN]
    if(csig==62){
      //Get the relevant data.
      byte cr,cg,cb,lh,ll;
      cr = Serial.read();
      cg = Serial.read();
      cb = Serial.read();
      lh = Serial.read();
      ll = Serial.read();
      //Construct the received color.
      uint32_t nc=strip.Color(cr,cg,cb);
      //Construct the pixel value that is selected. This is a 16-bit number to allow for strips longer than 256 pixels.
      int ln = (int) word(lh,ll);
      //Set the preset mask at this pixel value to false.
      ren_msk[ln]=0;
      //Clear the pixel. Doesn't make a difference, because the LED Control Utility continues sending the preview command at the selected color. But why not, this would be very useful in the bluetooth control scenario.
      strip.setPixelColor(ln+absolute_offset,strip.Color(0,0,0));
    }
    //Strip fill mode. This simply fills the entire strip with a single color. [NN]
    if(csig==44){
      //Get our relevant data.
      byte cr,cg,cb;
      cr = Serial.read();
      cg = Serial.read();
      cb = Serial.read();
      //Clear out the unused bytes.
      Serial.read();
      Serial.read();
      //Construct the selected color.
      uint32_t nc=strip.Color(cr,cg,cb);
      //Step through each pixel and set it to the selected color.
      for(int i=0;i<led_num;i++){
        strip.setPixelColor(i+absolute_offset,nc);
      }
    }
    //Separation mode mode! This runs separation mode, rendered exactly like it would be in a production scenario, with some serial parameters. This is sort of [NN]. In production, it would be controlled by a remote, not a PC, but the separation mode is absolutely neccessary.
    if(csig==40){
      //Get some relevant data.
      byte ctl,cth,csm;
      cth = Serial.read();
      ctl = Serial.read();
      csm = Serial.read();
      //Contstruct the selected animation delay in milliseconds. This is a WORD to allow for delays longer than 255ms.
      sep_dly = (int) word(cth,ctl);
      //Construct the selected separation mode.
      sep_mod = (int) csm;
      //Re-intialize the strip with the selected parameters.
      initStrips(color_enable,sep_mod);
      //Initialize our breaking var.
      int i=1;
      //Clear out the unused bytes.
      Serial.read();
      Serial.read();
      //Keep going until told to stop. From here on out the code is production-ready, except breaking is IR- and not Serial-triggered.
      while(i==1){
        //Run through the calculated number of rendering steps. Start where we stopped.
        for(int x=ren_glx;x<ren_stp;x++){
          //Render it!
          RenderSPM(x);
          //Keep track of where we are so that we can pause and resume.
          ren_glx=x;
          //Delay the animation by the selected amount.
          delay(sep_dly);
          //Look for the break command.
          if(Serial.read()==31){i=0;break;}
        }
        //Go back to rendering step zero.
        ren_glx=0;
      }
    }
    //Update the enabled colors mask. This sets color x high or low depending on serial parameters. Also semi-[NN], IR instead of serial, etc. etc.
    if(csig==30){
      //Become enlightened with relevant data.
      byte ccn,ccs;
      ccn = Serial.read();
      ccs = Serial.read();
      //Clear out unused bytes.
      Serial.read();
      Serial.read();
      Serial.read();
      //Make sure we got a valid command.
      if(ccs==0 || ccs==1){
        //Enable or disable the selected color.
        color_enable[ccn]=ccs;
      }
    }
    //I'm really not sure what this does, or that it's even valid C, but yet here it stands.
    if(csig==71)
      for(int i=0;i<5;i++){Serial.read();}
    //Push changes made by serial commands to the strip. Perhaps this should be moved into its own serial command, but for now, no.
    strip.show();
  }
}


///// FINAL REMARKS /////
//    Separation mode is fully functional on odd separation modes. It breaks thoroughly on even values, likely due to the way
//offsets are calculated. This needs fixing. The LED Control Utility is a great debugging and testing tool, and due to being
//written in AutoIt3 is easily modified. Twinkling and glowing should be fairly trivial, just using a weighted random value.
//The equations I developed earlier might be more smooth or objectively "better," but ultimately it will be difficult to notice.
//Perhaps (pre)calculating random values every 10 frames and blending towards them?
//   The big thing that need to be implemented is IR interrupt control. I don't know how difficult this will be, but it'll be
//a challenge. With that implemented, the serial control functions could be fairly easily ported to accept IR codes instead of
//the more flexible PC control.
/////////////////////////

