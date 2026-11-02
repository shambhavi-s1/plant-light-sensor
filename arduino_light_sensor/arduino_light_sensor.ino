#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
int minLight = 20;
int maxLight = 100;
int lightVal = 0;


void setup(void) {
  u8x8.begin();
  u8x8.setFlipMode(1);
  Serial.begin(9600);

}
void loop(void) {
  //from searching vals of plant using python:
  if (Serial.available() >0 ){
    String input = Serial.readStringUntil('\n');
    int commaIndex = input.indexOf(',');
    
    if (commaIndex != -1) {
      minLight = input.substring(0, commaIndex).toInt();
      maxLight = input.substring(commaIndex + 1).toInt();
      
      u8x8.clearDisplay();
      u8x8.setCursor(0, 0);
      u8x8.print("New Range:");
      u8x8.setCursor(0, 1);
      u8x8.print(minLight);
      u8x8.print("-");
      u8x8.print(maxLight);
      delay(1000);
    }
  }
  // NOT FROM SEARCH:
  lightVal = analogRead(A6);
  Serial.print(lightVal);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("light: ");
  u8x8.print(lightVal);
  u8x8.setCursor(0, 33);
  if (lightVal < minLight){
    u8x8.println("too dark!!");
  }else if (lightVal > maxLight){
    u8x8.println("too bright!!");
  } else {
    u8x8.println("IDEAL :D");}
  delay(300);
} 