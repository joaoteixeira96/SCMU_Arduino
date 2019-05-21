#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

String user = "ze";
WiFiUDP Udp;
unsigned int localUdpPort = 37020;
char incomingPacket[256];
char replyPacket[] = "{'msgType': 'triggerAlarm', 'user' : 'ze'}";

//For testing


void setup()
{
  Udp.begin(localUdpPort);
  Serial.begin(9600);
 
}
void loop(){
    //Serial.write("Hello from ESP");
    //delay(2000);  
int packetSize = Udp.parsePacket();
if (packetSize)
{
  Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  int len = Udp.read(incomingPacket, 255);
  if (len > 0)
  {
    incomingPacket[len] = '\0';
  }
  Serial.printf("UDP packet contents: %s\n", incomingPacket);
   const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(incomingPacket);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
    // Decode JSON/Extract values
    String incomingUser = root["user"].as<char*>();
    String incomingUserLights = root["settings"]["lights"].as<char*>();
    String incomingUserBlinds = root["settings"]["blinds"].as<char*>();
    
    // Print incoming result
    Serial.println(F("Response:"));
    Serial.println(incomingUser);
    Serial.println(incomingUserLights);
    Serial.println(incomingUserBlinds);
    Serial.println("End of response");

    if(incomingUser==user){
        if(incomingUserLights=="true"){
          Serial.write("LightOn");     
        }
    }

}
    //send information to arduino uno
   boolean flag =true;
    if(flag){ 
    Serial.write("LightOn");
    delay(4000);
    flag=false;
   }
 
    // send back a reply, to the IP address and port we got the packet from
    Serial.println(Udp.remoteIP());
    Serial.println(Udp.remotePort());
    StaticJsonBuffer<200> jsonBufferEncode;
    JsonObject& root2 = jsonBuffer.createObject();
    root2["msgType"] = "triggerAlarm";
    root2["user"] = "ze";
    String answer;
    root2.printTo(answer);

    WiFiClient client;
    if(!client.connect(Udp.remoteIP(),Udp.remotePort())){
      Serial.println("Connection to host failed");
      return;  
    }
    client.print(answer);
}
