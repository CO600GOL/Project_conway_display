/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544
(7, 6, 5, 4, 3);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()   {
  Serial.begin(115200);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(55);

  display.display(); // show splashscreen
  delay(2000);
  //display.clearDisplay();   // clears the screen and buffer
  //display.display();
  
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
      uint32_t xp = 0; 
      uint32_t yp = 0;
      int i = 0;
      
      // Ignore command
      while (inputString[i] != ' '){i++;}
      
      // Ignore spaces
      while (inputString[i] == ' '){i++;}
      
      // Build x position
      while (inputString[i] != ' '){
        xp = (xp * 10) + (inputString[i] - 48);
        i++;
      }
            
      // Ignore spaces
      while (inputString[i] == ' '){i++;}
          
      // Build x position
      while (inputString[i] != '\n'){
        yp = (yp * 10)  + (inputString[i] - 48);
        i++;
      }
      
      display.drawPixel(xp, yp, 0xFF);
      Serial.write("OK");
    }
    else if (strcmp(cmd, "drw") == 0){
      display.display();
    }
    else{
      Serial.println("Command not found");
    }
    inputString = "";
    stringComplete = false;
  }
}

void clear() {
  // Clear display
  display.clearDisplay();
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
