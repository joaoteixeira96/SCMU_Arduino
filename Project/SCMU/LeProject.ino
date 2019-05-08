 /*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
//#include <ArduinoJson.h>
#include <SPI.h>

// Replace with your network credentials
const char* ssid     = "Redmi";
const char* password = "123123123";

const uint16_t port = 2000;
const char* host = "192.168.43.12";
WiFiClient client;
WiFiServer wifiServer(80);
char * response;
void setup() {
  
  //StaticJsonBuffer<200> jsonBuffer;
  char json[] = "{\"msgType\":\"helloArduino\",\"user\":\"ze\" }";
  //JsonObject& root = jsonBuffer.parseObject(json);
  //if(!root.success()) {
  //Serial.println("parseObject() failed");
  //return false;
  //}
  
  Serial.begin(115200);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (!client.connect(host,port)) { // If a new client connects,
   Serial.println("Connection to host failed");
   delay(1000);
   return;
  }
  client.print(json);
}


void loop(){
  String c ;
    while (client.connected()) {
      Serial.print("inside");
      delay(2000);
      while (client.available()>0) {
        Serial.print("available");
       c = client.readString();
       Serial.println( c);
      }
      
      delay(10);
    }
    client.stop();
    //Serial.println("Client disconnected");
 
  
}
