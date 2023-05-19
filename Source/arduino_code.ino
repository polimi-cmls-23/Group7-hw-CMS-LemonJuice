#include <DFRobot_RGBLCD1602.h>
#include "DFRobot_TCS34725.h"


DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_60X);
DFRobot_RGBLCD1602 lcd(/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show
char dtaUart[15];
char dtaLen = 0;



void setup() 
{
  Serial.begin(9600);
  Serial.println("Color View Test!");
  tcs.begin();

  lcd.init();
  // Print a message to the LCD.
  lcd.print("Color scanned");
  lcd.clear();

  lcd.setBacklight(true);


  while(!tcs.begin())
  {
    Serial.println("No TCS34725 found ... check your connections");
    delay(1000);
  }

}

void loop() {
  uint16_t clear, red, green, blue;
  tcs.getRGBC(&red, &green, &blue, &clear);
  // turn off LED
  tcs.lock();  

  /*
  // PRINT R G B in absolute value
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);
  Serial.println("\t");
  */

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 255; g *= 255; b *= 255;

  //COLOR LCD SCREEN
  lcd.setRGB(r, g, b);
  /*
  //print hex value
  Serial.print("\t");
  Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.println();
  delay(100); 
  */

  //print R,G,B
  Serial.print((int)r);
  Serial.print(",");
  Serial.print((int)g);
  Serial.print(",");
  Serial.print((int)b);
  Serial.println();
  delay(1000); 
}