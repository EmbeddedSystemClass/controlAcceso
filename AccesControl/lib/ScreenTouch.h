#include "Arduino.h"
#include "Adafruit_ILI9341.h"
#include "config.h"

Adafruit_ILI9341 display = Adafruit_ILI9341(_TFT_CS,_TFT_DC,_TFT_RST);

void showText(String textInput){
    delay(300);
    display.begin();
    display.setRotation(2);
    display.fillScreen(ILI9341_BLACK);
    display.setCursor(0, 0);
    display.setTextColor(ILI9341_GREEN);
    display.setTextSize(2);
    display.println(textInput);
}