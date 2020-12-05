#include "Arduino.h"
#include "../lib/address.h"
#include "../lib/mqtt/mqtt.h"
#include <SPI.h>
#include <EEPROM.h>
#include <WiFi.h>
#include "MFRC522.h"
#include "../lib/config.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
//#include "../lib/ScreenTouch.h"

MFRC522 mfrc522(RFID_SS, RFID_RST);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
//Adafruit_ILI9341 display = Adafruit_ILI9341(_TFT_CS,_TFT_DC,_TFT_RST);
XPT2046_Touchscreen ts(_TP_CS, _TP_IRQ);

void setup() {
  Serial.begin(115200);
  Serial.println(".");
  SPI.begin(__SCK,__MISO,__MOSI,5); //SCLK, MISO, MOSI, SS
  WiFi.mode(WIFI_MODE_STA);
  //pinMode(PIN_MODE_WIFI,INPUT);
  byte clave[6] = {0x50,0x49,0x4E,0x45,0x44,0x41};
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;//clave[i];
  begin_eemprom();
  
  showText("Bienvenido");
  begin_mqtt(); 
  //writeEEPROM("0", STATUS_NEW,1);
  //Serial.println(readEEPROM(STATUS_NEW));
  //writeEEPROM("1", STATUS_NEW,1);
  //Serial.println(readEEPROM(STATUS_NEW));
  dataDeviceUpload(readEEPROM(STATUS_NEW));
  
}

void loop() {
  loop_mqtt();
  //Serial.println(readEEPROM(KEY,10));
  /*ts.begin();
  ts.setRotation(3);
  if (ts.tirqTouched()) {
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      Serial.print("Pressure = ");
      Serial.print(p.z);
      Serial.print(", x = ");
      Serial.print(p.x/10);
      Serial.print(", y = ");
      Serial.print(p.y/10);
      delay(30);
      Serial.println();
    }
  }*/
  delay(50);
  mfrc522.PCD_Init();
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.println(mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)).substring(1));
  String q=mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)).substring(1);
  setRFID(q);
  //loop_mqtt(q);
  /*display.begin();
  display.setRotation(2);
  display.fillScreen(ILI9341_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ILI9341_GREEN);
  display.setTextSize(2);
  display.println(q);*/
}

