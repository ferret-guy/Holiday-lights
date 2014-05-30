#include <Adafruit_NeoPixel.h>

#define PIN 7

//***********************
int sepratemode = 1;
//1 is 1 instance of every color
//2 is 2 instance of every color
//3 is 3 instance of every color
//0 is every other pixel
//***********************

//***********************
//Number of colors
int col_num = 0;
//***********************

//***********************
//scroll speed
int scroll_speed = 0;
//0 is no scroll
//1 - 4 are legal speeds
//***********************

//***********************
//Number of leds
int led_num = 100;
//***********************

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(104, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//color enable
boolean color_enable[] = {1,1,1,1,1,1,1,1,1,1,1};
//****************

/* color order
    red
    green
    blue
    white
    orange
    yellow
    purple
    warm white
    amber
    turquoise
    magenta */

// color vaues
uint32_t colors[] = {strip.Color(255,0,0),strip.Color(0,0,255),strip.Color(0,255,0),strip.Color(255,255,255),strip.Color(255,165,0),strip.Color(255,255,0),strip.Color(128,0,128),strip.Color(255,178,48),strip.Color(255,245,230),strip.Color(175,238,238),strip.Color(255,0,255)};
//****************



void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //declare colors
//  Serial.begin(9600);
//  Serial.println("Ready to decode IR!");
}

void loop() {
  //example color sepration
  // Some example procedures showing how to display to the pixels:
  /*(Serial.println("color Wipe 1 \n");
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  Serial.println("color Wipe 2 \n");  
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  Serial.println("color Wipe 3 \n");
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  Serial.println("set (mine! \n");
  color_set(strip.Color(0, 0, 255), 5, 1, 10); //(color, wait, start pixel, number)
  // Send a theater pixel chase in...
  Serial.println("Theater chase \n");
  theaterChase(strip.Color(127, 127, 127), 50); // White
  Serial.println("Theater chase 2 \n");
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  Serial.println("Theater chase 3 \n");
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  Serial.println("Rander!!!!!! \n");
  */
  colorcount();
  int index_step =0;
  while(index_step < col_num){
  render(index_step);
  index_step++;}
}

//********************************
//Adafruit functions
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//**********************************

//*************************************
//Set Strip section color
void color_set(uint32_t c, uint8_t wait, int start, int num) {
  for(int i=0; i<num; i++) {
      strip.setPixelColor((start+i), c);
 //       Serial.println("pixel num");
   //     Serial.println(start+i);
     //   Serial.println("\n");
      strip.show();
  }
}
//*************************************



void colorcount() {
  col_num = 0;
  if (color_enable[0] == true) { col_num=col_num+1; } 
  if (color_enable[1] == true) { col_num=col_num+1; } 
  if (color_enable[2] == true) { col_num=col_num+1; } 
  if (color_enable[3] == true) { col_num=col_num+1; } 
  if (color_enable[4] == true) { col_num=col_num+1; } 
  if (color_enable[5] == true) { col_num=col_num+1; } 
  if (color_enable[6] == true) { col_num=col_num+1; } 
  if (color_enable[7] == true) { col_num=col_num+1; } 
  if (color_enable[8] == true) { col_num=col_num+1; } 
  if (color_enable[9] == true) { col_num=col_num+1; } 
  if (color_enable[10] == true){ col_num=col_num+1; } 
  if (color_enable[11] == true){ col_num=col_num+1; } 
  Serial.println("col_num:");
  Serial.println(col_num);
  Serial.println("\n");
}



//base color section render
void render (int offset) {
  int col_step = 0;
  int col_len = 0;
  int remainder = 0;
  int index = 0;
  index=index+offset;

        switch(sepratemode){
          case 1:
              col_len = ((strip.numPixels())/col_num);
              remainder = strip.numPixels()-(col_len*col_num);
              break;
          case 2:
              col_len = floor(col_num/(strip.numPixels()/2));
              remainder = floor(strip.numPixels()/2)-(col_len*col_num);
              break;
          case 3:
              col_len = floor(col_num/(strip.numPixels()/3));
              remainder = floor(strip.numPixels()/3)-(col_len*col_num);
              break;
           case 0:
              break;
        }

    while(col_step < 12 && col_num != 0 && sepratemode != 0){
        if (color_enable[col_step] == true){
                      int start = index;
                        for(int i=0; i<col_len; i++) {
                    strip.setPixelColor((start+i), colors[col_step]);
                    strip.show();}
            index = index + col_len;
            }
          col_step++;
        }



      if(sepratemode == 0){
              //put colors in array
              col_step = 0;
              int array_step = 0;
              uint32_t seprate_color[col_num];
                 while(col_step < 12){
                   if(color_enable[col_step] == 1){
                      seprate_color[array_step]= colors[col_step];
                      array_step++;
                  //    Serial.println("color:");
                  //    Serial.println(colors[col_step]);
                  //    Serial.println("array step");
                  //    Serial.println(array_step);
                       }
                       col_step++;
                   }
                 //End colors in array
                 Serial.println("end array");
                while(index < strip.numPixels()){
               //  Serial.println("index");
               //  Serial.println(index);
              //   Serial.println("\n");
                  array_step = 0;
                  while(array_step < col_num){
                      strip.setPixelColor(index,seprate_color[array_step]);
                      index=index+1;
                      array_step=array_step+1;
                  /*
                      Serial.println("array_step");
                       Serial.println(array_step);
                      Serial.println("color");
                       Serial.println(seprate_color[array_step]);
                       */
                  }
                }
              }
              strip.show();
      }
      
  
  
