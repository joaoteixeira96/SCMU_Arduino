#include <DRV8835MotorShield.h>
DRV8835MotorShield motores;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  motores.setM1Speed(200);
  motores.setM2Speed(-200);
}
