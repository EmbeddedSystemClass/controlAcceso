#include "Arduino.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "../config.h"
#include "../ScreenTouch.h"
#include "../wps.h"
#include "MFRC522.h"

WiFiClient espClient;
PubSubClient client(espClient);
WiFiServer server(80);
/*MFRC522 mfrc522(RFID_SS, RFID_RST);
MFRC522::MIFARE_Key key;*/

char msg[50];
long _count=0;

//************************
//** F U N C I O N E S ***
//************************
void callback(char* topic, byte* payload, unsigned int length){
	String incoming = "";
	char msg[20];
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	if(incoming == "open"){
		digitalWrite(RL,HIGH);
		Serial.println("HIGH");
		delay(1000);
		digitalWrite(RL,LOW);
		Serial.println("HIGH");
	}
	else if(incoming.length()>=SIZE_OF_KEY){
		incoming.toCharArray(msg,SIZE_OF_KEY);
		writeEEPROM(msg, KEY,SIZE_OF_KEY);
		showText(incoming);
	}
	Serial.println("Mensaje -> " + incoming);

}
void reconnect(){

	while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("9cb9dff1586d47c590ea2ce428dee911")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish((const char*)root_topic_publish, "hello world");
      // ... and resubscribe
      client.subscribe((const char*)root_topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup_wifi(){
	delay(10);
	/*byte clave[6] = {0x50,0x49,0x4E,0x45,0x44,0x41};
  	for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;//clave[i];*/
	beginWPS();
	/*Serial.println();
	Serial.print("Conectando a ssid: ");
	Serial.println("diego4");
	
	const char*  SSID ="diego4";
	const char* PASSWORD  = "6Y6T10UO";
	WiFi.begin(SSID, PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());*/
	
}
/*void serverStart(){
	if(!digitalRead(PIN_MODE_WIFI)){
		WiFiClient client = server.available();   // Listen for incoming clients

		if (client) {                             // If a new client connects,
			Serial.println("New Client.");          // print a message out in the serial port
			String currentLine = "";
			String header;                // make a String to hold incoming data from the client
			while (client.connected()) {            // loop while the client's connected
			if (client.available()) {             // if there's bytes to read from the client,
				char c = client.read();             // read a byte, then
				Serial.write(c);                    // print it out the serial monitor
				header += c;
				if (c == '\n') {                    // if the byte is a newline character
				// if the current line is blank, you got two newline characters in a row.
				// that's the end of the client HTTP request, so send a response:
				if (currentLine.length() == 0) {
					// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
					// and a content-type so the client knows what's coming, then a blank line:
					client.println("HTTP/1.1 200 OK");
					client.println("Content-type:text/html");
					client.println("Connection: close");
					client.println();
					

					// turns the GPIOs on and off
					/*if (header.indexOf("GET /26/on") >= 0) {
					Serial.println("GPIO 26 on");
					output26State = "on";
					digitalWrite(output26, HIGH);
					} else if (header.indexOf("GET /26/off") >= 0) {
					Serial.println("GPIO 26 off");
					output26State = "off";
					digitalWrite(output26, LOW);
					} else if (header.indexOf("GET /27/on") >= 0) {
					Serial.println("GPIO 27 on");
					output27State = "on";
					digitalWrite(output27, HIGH);
					} else if (header.indexOf("GET /27/off") >= 0) {
					Serial.println("GPIO 27 off");
					output27State = "off";
					digitalWrite(output27, LOW);
					}
					
					// Display the HTML web page
					client.println("<!DOCTYPE html><html>");
					client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
					client.println("<link rel=\"icon\" href=\"data:,\">");
					// CSS to style the on/off buttons 
					// Feel free to change the background-color and font-size attributes to fit your preferences
					client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
					client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
					client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
					client.println(".button2 {background-color: #555555;}</style></head>");
					
					// Web Page Heading
					client.println("<body><h1>ESP32 Web Server</h1>");
					
					// Display current state, and ON/OFF buttons for GPIO 26  
					client.println("<p>GPIO 26 - State " + output26State + "</p>");
					// If the output26State is off, it displays the ON button       
					if (output26State=="off") {
					client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
					} else {
					client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
					} 
					
					// Display current state, and ON/OFF buttons for GPIO 27  
					client.println("<p>GPIO 27 - State " + output27State + "</p>");
					// If the output27State is off, it displays the ON button       
					if (output27State=="off") {
					client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
					} else {
					client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
					}
					client.println("</body></html>");
					
					// The HTTP response ends with another blank line
					client.println();
					// Break out of the while loop
					break;
				} else { // if you got a newline, then clear currentLine
					currentLine = "";
				}
				} else if (c != '\r') {  // if you got anything else but a carriage return character,
				currentLine += c;      // add it to the end of the currentLine
				}
			}
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
		}
	}
}*/
void setRFID(String text){
	text.toCharArray(msg,50);
	client.publish(root_topic_publish,msg);
}
void begin_mqtt(){
	setup_wifi();
	client.setServer((const char*)mqtt_server, mqtt_port);
	client.setCallback(callback);
}
void loop_mqtt(){
	if (!client.connected()) {
		reconnect();
	}

	if (client.connected()){
		if(msg[0] != '\0')client.publish(root_topic_publish,msg);
		String text="";
		text.toCharArray(msg,50);
	}
	client.loop();
	
	/*delay(50);
	mfrc522.PCD_Init();
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	Serial.println(mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)).substring(1));
	String str=mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)).substring(1);
	Serial.println("....");
	str.toCharArray(msg,50);
	client.publish(root_topic_publish,msg);*/
}
void dataDeviceUpload(byte statusDataDevice){
	if(statusDataDevice!=49){
		if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = WiFi.macAddress();
			str.toCharArray(msg,50);
			client.publish(root_topic_publish,msg);
			delay(300);
		}
		client.loop();
		writeEEPROM("1", STATUS_NEW,1);
	}
}

/*void registerDevice(byte statusDataDevice){
	if(statusDataDevice){
		if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(REGISTER,msg);
			count++;
			delay(300);
		}
		client.loop();
	}
}
void residentInput(){
	if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(RESIDENT,msg);
			count++;
			delay(300);
		}
		client.loop();
}
void siteInvite(){
	if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(SITE,msg);
			count++;
			delay(300);
		}
		client.loop();
}
void keyServer(){
	if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(root_topic_publish,msg);
			count++;
			delay(300);
		}
		client.loop();
}
void actionDoor(){
	if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(root_topic_publish,msg);
			count++;
			delay(300);
		}
		client.loop();
}
void letInputSite(){
	if (!client.connected()) {
			reconnect();
		}

		if (client.connected()){
			String str = "{'name':'HOLA','lastName':'MUNDO'}";
			str.toCharArray(msg,50);
			client.publish(root_topic_publish,msg);
			count++;
			delay(300);
		}
		client.loop();
}*/
