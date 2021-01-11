#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <Encoder.h>

#define BUTTON_ROTARY A3
#define ENCODER_A A2
#define ENCODER_B A1

LiquidCrystal_I2C lcd(0x27, 20, 4);

//9 pin is DIR, 8 is PUL in M542 Stepper driver
AccelStepper stepper(1, 8, 9);

//Rottary encoder
//Encoder EncoderKnob(ENCODER_A, ENCODER_B);

uint8_t counter = 0;
uint8_t currentStateCLK;
uint8_t previousStateCLK;

void setup() {
  
  pinMode (ENCODER_A,INPUT_PULLUP);
  pinMode (ENCODER_B, INPUT_PULLUP);
  previousStateCLK = digitalRead(ENCODER_A);
  
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("hello");
  lcd.setCursor(1, 1);
  lcd.print("no hell");
//  lcd.setCursor(1, 2);
//  lcd.print(EncoderKnob.read());

  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(100);
  stepper.setSpeed(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  do
  {
    stepper.runSpeed();
  } while (millis() < 10000);
  stepper.setSpeed(0);
/*
  int MenuPosition = EncoderKnob.read() / 4;
  if(MenuPosition < 1)
  {
    EncoderKnob.write(16);
    MenuPosition = 4;
  }
  if(MenuPosition > 4)
  {
   EncoderKnob.write(0);
   MenuPosition = 0;
  }
*/

  currentStateCLK = digitalRead(ENCODER_A);
  if(currentStateCLK != previousStateCLK)
  {
    if(digitalRead(ENCODER_B) != currentStateCLK)
    {
      counter--;
      lcd.setCursor(1, 2);
      lcd.clear();
      lcd.print(counter);   
    }
    else
    {
      counter++;
      lcd.setCursor(1, 2);
      lcd.clear();
      lcd.print(counter);
       
    }
  }
  previousStateCLK = currentStateCLK;
}
