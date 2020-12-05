
#define mqtt_server  "192.168.0.3"
#define mqtt_port  1883
#define mqtt_user  "diego"
#define mqtt_pass  "diego123"
#define root_topic_subscribe  "key/action/let"
#define root_topic_publish  "register/input/site"

#define REGISTER  "register"
#define RESIDENT "resident"
#define SITE "site"
#define KEY "key"
#define ACTION "action"
#define LET "let"

//**************************************
//*********** WIFICONFIG ***************
//**************************************

/*#define SSID "diego4"
#define PASSWORD   "6Y6T10UO"*/


//**************************************
//************* PINOUT *****************
//**************************************

#define __MISO       19
#define __MOSI	    23
#define __SCK	    18
#define _TFT_DC	    21
#define _TFT_CS	    22
#define _TFT_RST	15
#define _TP_IRQ	    14
#define _TP_CS	    27
#define _SD_CS	    5
#define RFID_RST	2
#define RFID_SS	    4
#define ETH_CS	    13
#define RL	        12
#define LEDS	    27
//#define PIN_MODE_WIFI 35


//*****************************************
//************* ADD EEPROM ****************
//*****************************************
#define STATUS_NEW          0       
#define KEY                 1          
#define NETWORK             21      
#define PASSWORD            41

#define SIZE_OF_KEY         6      
#define SIZE_OF_NETWORK     51
#define SIZE_OF_PASSWORD    52
#define EEPROM_SIZE         100