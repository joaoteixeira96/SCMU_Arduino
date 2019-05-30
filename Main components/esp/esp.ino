#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial esp(4, 5);

String user = "ze";
WiFiUDP Udp;
unsigned int localUdpPort = 37020;
char incomingPacket[256];
char replyPacket[] = "{'msgType': 'triggerAlarm', 'user' : 'ze'}";

//TEST : CHANGE VALUES BACK TO ""
String incomingUser="";
String incomingUserLights="";
String incomingUserBlinds="";

void setup()
{
  Udp.begin(localUdpPort);
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only -> O que é isto?
  } 
  esp.begin(9600);
}
void loop(){
// TODO dar reset aos valores user lights e blinds
incomingUser="";
incomingUserLights="";
incomingUserBlinds="";
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
     incomingUser = root["user"].as<char*>();
     incomingUserLights = root["settings"]["lights"].as<char*>();
     incomingUserBlinds = root["settings"]["blinds"].as<char*>();
    
    // Print incoming result
    Serial.println(F("Response:"));
    Serial.println(incomingUser);
    Serial.println(incomingUserLights);
    Serial.println(incomingUserBlinds);
    Serial.println("End of response");

    // Send request for Arduino
    delay(1000);
    sendRequestToArduino();

    //Wait until user pressures the piezo and triggers and Knock! message
    while(!(esp.readString()=="Knock!")){}

    // send back a reply, to the IP address and port we got the packet from
    Serial.println(Udp.remoteIP());
    Serial.println(Udp.remotePort());
    StaticJsonBuffer<200> jsonBufferEncode;
    JsonObject& root2 = jsonBuffer.createObject();
    root2["msgType"] = "triggerAlarm";
    root2["user"] = incomingUser;
    String answer ="";
    root2.printTo(answer);

    WiFiClient client;
    if(!client.connect(Udp.remoteIP(),Udp.remotePort())){
      Serial.println("Connection to host failed");
      return;  
    }
    client.print(answer);

}
   
/*    //FOR TESTING
    sendRequestToArduino();
 
    // send back a reply, to the IP address and port we got the packet from
    while(!(esp.readString()=="Knock!")){
    }
    //FOR TESTING
    Serial.println("triggerAlarm");
    Serial.println("ze");
 */    
}
void sendRequestToArduino()
{
  if(incomingUser==user){
        if(incomingUserLights == "true" && incomingUserBlinds == "true")
        {
          esp.write("allOn");
          delay(4000);
          while(esp.readString()=="Repeat"){
          esp.write("allOn");
          delay(4000);
          }
        } 
        else if(incomingUserLights=="true"){
          esp.write("LightOn");
          delay(4000);
          while(esp.readString()=="Repeat"){
          esp.write("LightOn");
          delay(4000);
          }
        }else if(incomingUserBlinds == "true"){
          esp.write("BlindsOn");
          delay(4000);
          while(esp.readString()=="Repeat"){
          esp.write("LightOn");
          delay(4000);
          }
        }else if(incomingUserLights == "false" && incomingUserBlinds == "false"){
          esp.write("allOff");
          delay(4000);
        }
    }

}
