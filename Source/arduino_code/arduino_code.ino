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

  //LCD initialization
  lcd.init();
  lcd.clear();
  lcd.setBacklight(true);
  lcd.setCursor(3,0);       // places the cursor in column 3 and row 0
  lcd.print("ColorMixer");     

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

  // Compute RGB
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

  // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
  // R G B :   X X X   X  X  X    X  X  X
  lcd.setCursor(0,1);
  lcd.print("                ");  // clean only the second line
  lcd.setCursor(0,1);    // Place the cursor on the first column of the second line
  lcd.print("RGB: ");
  lcd.setCursor(5, 1);  
  lcd.print((int)r);
  lcd.setCursor(9, 1);
  lcd.print((int)g);
  lcd.setCursor(13, 1);
  lcd.print((int)b);

  //serial print R,G,B
  Serial.print((int)r);
  Serial.print(",");
  Serial.print((int)g);
  Serial.print(",");
  Serial.print((int)b);
  Serial.println();
  delay(50); 
}