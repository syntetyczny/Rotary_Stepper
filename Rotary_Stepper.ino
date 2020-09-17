#include <AccelStepper.h>

//9 pin is DIR, 8 is PUL in M542 Stepper driver
AccelStepper stepper(1,8,9);

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(100);
  stepper.setSpeed(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    do
    {
    stepper.runSpeed();
    }while(millis() < 10000);
    stepper.setSpeed(0);

}
