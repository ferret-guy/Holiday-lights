#include <Adafruit_NeoPixel.h>

#define PIN 7

//***********************
int sepratemode = 0;
//0 is 1 instance of every color
//1 is 2 instance of every color
//2 is 3 instance of every color
//3 is every other pixel
//***********************

//***********************
//color enable
boolean red = 1;
boolean blue = 0;
boolean green = 1;
boolean white = 0;
boolean orange = 0;
boolean yellow = 0;
boolean color = 1;
boolean color1 = 0;
boolean color2 = 0;
boolean color3 = 0;
boolean color4 = 1;
boolean color5 = 0;
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
int led_num = 4;
//***********************

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //declare colors
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t blue = strip.Color(0, 255, 0);
  uint32_t green = strip.Color(0, 0, 255);
  uint32_t white = strip.Color(255, 255, 255);
  uint32_t orange = strip.Color(255, 165, 0);
  uint32_t yellow = strip.Color(255, 0, 255);
  uint32_t color = strip.Color(255, 0, 255);
  uint32_t color2 = strip.Color(255, 0, 255);
  uint32_t color3 = strip.Color(255, 0, 255);
  uint32_t color4 = strip.Color(255, 0, 255);
  uint32_t color5 = strip.Color(255, 0, 255);
  uint32_t off = strip.Color(0, 0, 0);
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}

void loop() {
  //example color sepration
  colorcount();
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
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


//base color section render
void render (int offset) {
  //*********************
  // temp
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t blue = strip.Color(0, 255, 0);
  uint32_t green = strip.Color(0, 0, 255);
  uint32_t white = strip.Color(255, 255, 255);
  uint32_t orange = strip.Color(255, 165, 0);
  uint32_t yellow = strip.Color(255, 0, 255);
  uint32_t color = strip.Color(255, 0, 255);
  uint32_t color2 = strip.Color(255, 0, 255);
  uint32_t color3 = strip.Color(255, 0, 255);
  uint32_t color4 = strip.Color(255, 0, 255);
  uint32_t color5 = strip.Color(255, 0, 255);
  uint32_t off = strip.Color(0, 0, 0);  
    //************************
  
  int col_len = 1;
  int col_step = 0;
  int remainder = 0;
  int num = 0;
  colorcount();
  if (col_num = 0) {
    while (col_step < led_num)
      strip.setPixelColor(col_step, off);
      col_step++;
  }
    else
  {
    //get the number of pixels in color
     col_len = floor(led_num/col_num);
     //apply offset
     col_step = col_step + offset;
    //*****************************************************
    //write colors
    //*****************************************************
    //red
      switch (red = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, red);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //blue
      switch (blue = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, blue);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //green
      switch (green = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, green);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //white
      switch (white = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, white);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //orange
      switch (orange = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, orange);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //yellow
      switch (yellow = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, yellow);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //color
      switch (color = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, color);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //color2
      switch (color2 = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, color2);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //color3
      switch (color3 = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, color3);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //color4
      switch (color4 = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, color4);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    //color5
      switch (color5 = 1) {
    case 1:
      num = 0;
      while (num < col_len)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, color5);
      col_step++;
      num++;
      break;
    default: 
     break;
     }
    //*****************************************************     
    // fill rest with off
      remainder = led_num - (col_len * col_num);
      if ( remainder = 0) {}
      else {
      num = 0;
     while (num < remainder)
      if (col_step > led_num ) {col_step = col_step - led_num;}
      strip.setPixelColor(col_step, off);
      col_step++;
      num++;
     }
    //*****************************************************     
  }
  }
  

int colorcount() {
  col_num = 0;
  if (red = 0){ col_num=col_num+1; } 
  if (blue = 0){ col_num=col_num+1; } 
  if (green = 0){ col_num=col_num+1; } 
  if (white = 0){ col_num=col_num+1; } 
  if (orange = 0){ col_num=col_num+1; } 
  if (yellow = 0){ col_num=col_num+1; } 
  if (color = 0){ col_num=col_num+1; } 
  if (color1 = 0){ col_num=col_num+1; } 
  if (color2 = 0){ col_num=col_num+1; } 
  if (color3 = 0){ col_num=col_num+1; } 
  if (color4 = 0){ col_num=col_num+1; } 
  if (color5 = 0){ col_num=col_num+1; } 
  Serial.println(col_num);
  Serial.println("/n");
  return col_num;
}
