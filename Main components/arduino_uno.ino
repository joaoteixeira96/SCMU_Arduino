#include <SoftwareSerial.h>
//Pin for photoresistor
const int sensorPin=5;
int lightLevel, high=0,low=1023;

// software serial #1: RX = digital pin 0, TX = digital pin 2
SoftwareSerial esp(2, 3);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only -> O que é isto?
  }

  // Start esp software serial port
  esp.begin(9600);

  // initialize digital pin LED_BUILTIN = 13 as an output. 
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  String response;
  if (esp.available()) {
    //Serial.write(esp.read());
    response = esp.readString();
    Serial.println(response);
  }
  /*if (Serial.available()) {
    esp.write(Serial.read());
    response = esp.readString();
    Serial.println(response);
  }*/

    //response == "LightOn"
  if(false){
   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(1000);                       // wait for a second
   digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
   delay(1000);                       // wait for a second
  } else{
     lightLevel = analogRead(sensorPin);
     manualTune();
     Serial.println(lightLevel);
     if(lightLevel < 100){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
     }
     
  }
}
void manualTune()
{
  lightLevel=map(lightLevel,0,1023,0,255);
  lightLevel=constrain(lightLevel,0,255);
}
