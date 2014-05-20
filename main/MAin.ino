#include <Adafruit_NeoPixel.h>
#include ircodes.h

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
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
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
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  if (IRcompare(numberpulses, ApplePlaySignal,sizeof(ApplePlaySignal)/4)) {
    Serial.println("PLAY");
  }
    if (IRcompare(numberpulses, AppleRewindSignal,sizeof(AppleRewindSignal)/4)) {
    Serial.println("REWIND");
  }
    if (IRcompare(numberpulses, AppleForwardSignal,sizeof(AppleForwardSignal)/4)) {
    Serial.println("FORWARD");
  }
  delay(500);
}

// from adafruit ir decoder example
// ****************************************************
boolean IRcompare(int numpulses, int Signal[], int refsize) {
  int count = min(numpulses,refsize);
  Serial.print("count set to: ");
  Serial.println(count);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
    
#ifdef DEBUG    
    Serial.print(oncode); // the ON signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 0]); // the ON signal we want 
#endif   
    
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
    
#ifdef DEBUG
    Serial.print("  \t"); // tab
    Serial.print(offcode); // the OFF signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 1]); // the OFF signal we want 
#endif    
    
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
#ifdef DEBUG
    Serial.println();
#endif
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;
  
  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       
       // KGO: Added check for end of receive buffer
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
        // KGO: Added check for end of receive buffer
        if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}
void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
  
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0};");
}
//*********************************************************************************
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
