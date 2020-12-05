#include "Arduino.h"
#include <EEPROM.h>
#include "config.h"

void begin_eemprom(){
    EEPROM.begin(EEPROM_SIZE);
}

void writeEEPROM(const char* data, byte typeOfAdd,byte size){
    for(byte i=0;i<size;i++){
        EEPROM.write(i+typeOfAdd, *(data+i));
        EEPROM.commit();
    }
}
String readEEPROM(byte typeOfAdd, byte size){
    String returnData="";
    for(int i=typeOfAdd;i<(size+typeOfAdd-1);i++){
        returnData+=(char)EEPROM.read(i);
    }
    return returnData;
}
int readEEPROM(byte typeOfAdd){
    return EEPROM.read(typeOfAdd);
}