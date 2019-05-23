#include <SoftwareSerial.h>
#include <DRV8835MotorShield.h>

// DC motor
DRV8835MotorShield motores;

//Pin for photoresistor
const int sensorPin=5;
int lightLevel, high=0,low=1023;

//Pin for motionSensor
int motionSensorPin = 7;

//Piezo
int sensorReading = 0;
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 200;  // threshold value to decide when the detected sound is a knock or not 

String response="";
// software serial #1: RX = digital pin 0, TX = digital pin 2
SoftwareSerial arduino(2, 3);
boolean motorStarted = false;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only -> O que é isto?
  }

  // Start esp software serial port
  arduino.begin(9600);

  // initialize digital pin LED_BUILTIN = 13 as an output. 
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  motorStarted = false;
  if (arduino.available()) {
    //Serial.write(esp.read());
    response = arduino.readString();
    Serial.println(response);
  }

  /*if (Serial.available()) {
    esp.write(Serial.read());
    response = esp.readString();
    Serial.println(response);
  }*/
  if(response == "allOn")
  {
    openLightsAndBlinds();
    pressureDetector();
    delay(20000);
  }
  else if(response == "LightOn"){
       openLights();
       pressureDetector();
       delay(20000);       
  } else if(response =="BlindsOn" ){
       openBlinds();
       pressureDetector();
       delay(20000);
  } else if(response == "allOff"){
    pressureDetector();
       delay(20000);
  }
}

void manualTune()
{
  lightLevel=map(lightLevel,0,1023,0,255);
  lightLevel=constrain(lightLevel,0,255);
}
void openLights()
{
  Serial.println("LED on");
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)   
}
void openBlinds()
{
     lightLevel = analogRead(sensorPin);
     manualTune();
     Serial.println(lightLevel);
     if(lightLevel >200){
      digitalWrite(LED_BUILTIN, HIGH); // FOR TEST : simulates its a motor
      motores.setM1Speed(200);
      motores.setM2Speed(-200);
      delay(6000);
      motores.setM1Speed(0);
      motores.setM2Speed(0);
      digitalWrite(LED_BUILTIN, LOW);
     }  
}
void openLightsAndBlinds()
{
   openLights();
   openBlinds(); 
}
void pressureDetector()
{
  while(true){
  sensorReading = analogRead(knockSensor);
  Serial.println(sensorReading);
  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold /*&& motionDetect()*/) {
    Serial.println("Knock!");
    arduino.write("Knock!");
    return ;
  }
  delay(100);  // delay to avoid overloading the serial port buffer
  }
}
boolean motionDetect()
{
   return digitalRead(motionSensorPin) > 0;
  
}
