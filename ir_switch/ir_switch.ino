//Include required NEC IR library (https://github.com/adafruit/Adafruit-NEC-remote-control-library)
#include "Adafruit_NECremote.h"
//Set the pin to which the IR input is connected as Arduino pin 9
#define IRpin 2
//Create NEC IR Remote object named "remote" on pin "IRpin" (pin 9)
Adafruit_NECremote remote(IRpin);

void setup() {
  //Open serial communications at 9600 baud.
  Serial.begin(9600);
}

void loop() {
  //Set variable c to NEC IR Code captured from NEC IR Remote object "remote", but only wait a tenth of a second for the IR code before moving on.
  int c = remote.listen(0.1);
  //Run a block of code based on what NEC IR Code was captured to variable c.
  switch(c){
    case 0:
        //vol+ button;
        //code
        break;
    case 1:
        //pause button;
        //code
        break;
    case 2:
        //vol- button;
        //code
        break;
    case 4:
        //setup button;
        //code
        break;
    case 5:
        //up button;
        //code
        break;
    case 6:
        //stop button;
        //code
        break;
    case 8:
        //left button;
        //code
        break;
    case 9:
        //enter button;
        //Indicate over serial that the "Enter/Save" button on the Adafruit Mini Remote has been pressed.
        Serial.println("Enter/Save pressed (Code #9).");
        break;
    case 10:
        //right button;
        //code
        break;
    case 12:
        //zero button;
        //code
        break;
    case 13:
        //down button;
        //code
        break;
    case 14:
        //back button;
        //code
        break;
    case 16:
        //one button;
        //code
        break;
    case 17:
        //two button;
        //code
        break;
    case 18:
        //three button;
        //code
        break;
    case 20:
        //four button;
        //code
        break;
    case 21:
        //five button;
        //code
        break;
    case 22:
        //six button;
        //code
        break;
    case 24:
        //seven button;
        //code
        break;
    case 25:
        //eight button;
        //code
        break;
    case 26:
        //nine button;
        //code
        break;
    default :
        //nothing
        break;
  }
}
