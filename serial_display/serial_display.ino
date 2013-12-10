#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by David Kavanagh (dkavanagh@gmail.com).  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels in a row and
// the second value to number of pixels in a column.
Adafruit_WS2801 strip = Adafruit_WS2801((uint16_t)5, (uint16_t)5, dataPin, clockPin);

uint32_t w = 5;
uint32_t h = 5;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  uint32_t x, y;
   
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
  
  // Set (0, 0) 
  //for (x=0; x<w; x++){
  //  for (y=0; y<h; y++){
  //    if (x==y){
  //      strip.setPixelColor(x, y, 50, 0, 0);    
  //    }
  //  }  
  //}
  //strip.show();
  
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}


void loop() {
  char cmd[4];
  if (stringComplete) {
    strncpy(cmd, (char*)inputString.c_str(), 3);
    cmd[3] = '\0';
    if (strcmp(cmd, "clr") == 0){
      clear(); 
    }
    else if (strcmp(cmd, "set") == 0){
      uint32_t xp, yp;
      int i = 0;
      while (inputString[i] != ' '){i++;}
      i++;
      xp = inputString[i] - 48;
      
      while (inputString[i] != ' '){i++;}
      i++;
      yp = inputString[i] - 48;
      
      strip.setPixelColor(xp, yp, 25, 25, 25);      
    }
    else if (strcmp(cmd, "drw") == 0){
      strip.show();
    }
    else{
      Serial.println("Command not found");
    }
    inputString = "";
    stringComplete = false;
  }
}

void clear() {
  uint32_t x, y;
  // Set (0, 0) 
  for (x=0; x<w; x++){
    for (y=0; y<h; y++){
        strip.setPixelColor(x, y, 0, 0, 0);    
    }  
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

