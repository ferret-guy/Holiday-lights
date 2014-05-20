#include <Adafruit_NeoPixel.h>

#define PIN 6

//***********************
int sepratemode = 0;
//0 is 1 instance of every color
//1 is 2 instance of every color
//2 is 3 instance of every color
//3 is every other pixel
//***********************

//***********************
//color enable
boolean red = 0;
boolean blue = 0;
boolean green = 0;
boolean white = 0;
boolean orange = 0;
boolean yellow = 0;
boolean color = 0;
boolean color1 = 0;
boolean color2 = 0;
boolean color3 = 0;
boolean color4 = 0;
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
int led_num = 300;
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
}

void loop() {
  //example color sepration
  colorcount();
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127,   0,   0), 50); // Red
  //theaterChase(strip.Color(  0,   0, 127), 50); // Blue
}
//simple color scroll
void scroll (  ) {}

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
  if (red = 0){ col_num++; } 
  if (blue = 0){ col_num++; } 
  if (green = 0){ col_num++; } 
  if (white = 0){ col_num++; } 
  if (orange = 0){ col_num++; } 
  if (yellow = 0){ col_num++; } 
  if (color = 0){ col_num++; } 
  if (color1 = 0){ col_num++; } 
  if (color2 = 0){ col_num++; } 
  if (color3 = 0){ col_num++; } 
  if (color4 = 0){ col_num++; } 
  if (color5 = 0){ col_num++; } 
  return col_num;
}
