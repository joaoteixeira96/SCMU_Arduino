#include <SoftwareSerial.h>
#include <DRV8835MotorShield.h>
//reset arduino
int Reset = 7;

// DC motor
DRV8835MotorShield motores;

//Pin for photoresistor
const int sensorPin=5;
int lightLevel;

//Pin for motionSensor
int motionSensorPin = 5;

//Piezo
int sensorReading = 0;
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 200;  // threshold value to decide when the detected sound is a knock or not 
const String allOn= "allOn";
const String LightOn= "LightOn";
const String BlindsOn ="BlindsOn";
const String allOff = "allOff";

String response;
// software serial #1: RX = digital pin 0, TX = digital pin 2
SoftwareSerial arduino(2, 3);
void setup() {
  response ="";
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
  response ="";
  if (arduino.available()) {
    //Serial.write(esp.read());
    response = arduino.readString();
    Serial.println(response);
  }
  if(!(response =="" ||response == allOn ||response == LightOn  || response == BlindsOn || response == allOff) ){
    arduino.write("Repeat");
    delay(4000);
    Serial.println("Trash message please resend");
  }
  
  if(response == allOn)
  {
    openLightsAndBlinds();
    pressureDetector();
    delay(20000);
  }
  else if(response == LightOn){
     openLights();
     pressureDetector();
     delay(20000);       
  } else if(response ==BlindsOn ){
     openBlinds();
     pressureDetector();
     delay(20000);
  } else if(response == allOff){
    pressureDetector();
      delay(20000);
  } else return;
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
     Serial.println("LightLevel    :"+lightLevel);
     if(lightLevel >150){
      //digitalWrite(LED_BUILTIN, HIGH); // FOR TEST : simulates its a motor
      motores.setM2Speed(-100);
      delay(6000);
      motores.setM2Speed(0);
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
  motionDetect();
  if (sensorReading >= threshold && motionDetect()) {
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
