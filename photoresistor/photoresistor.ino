const int sensorPin=0;
const int ledPin=9;
int lightLevel,high=0,low=1023;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lightLevel = analogRead(sensorPin);
  manualTune();
  analogWrite(ledPin,lightLevel);
  Serial.println(lightLevel);
}
  void manualTune(){
    lightLevel = map(lightLevel,0,1023,0,255);
    lightLevel = constrain(lightLevel,0,255);
  }
