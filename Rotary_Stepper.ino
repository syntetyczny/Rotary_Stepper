#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

#define BUTTON_ROTARY A3
#define ENCODER_A A2
#define ENCODER_B A1

LiquidCrystal_I2C lcd(0x27, 20, 4);

//9 pin is DIR, 8 is PUL in M542 Stepper driver
AccelStepper stepper(1, 8, 9);

//Rottary encoder
Encoder EncoderKnob(ENCODER_A, ENCODER_B);

int8_t counter = 0;
int8_t counterPreviousState = 0;
int8_t currentStateCLK;
int8_t previousStateCLK;

void screen_1()
{
//  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Start");
  lcd.setCursor(2, 1);
  lcd.print("Stop");
  lcd.setCursor(2, 2);
  lcd.print("Settings");
  if(counter != counterPreviousState)
  {
    lcd.setCursor(0, counter);
    lcd.print(">");
    lcd.setCursor(0, counterPreviousState);
    lcd.print(" ");
  }
  else if((counter == 0)&&(counterPreviousState == 0))
  {
    lcd.setCursor(0, counter);
    lcd.print(">");    
  }
 
}

enum MENU_POS
{
 POS_1,
 POS_2,
 POS_3,
 POS_4
};

void run_motor_countinous()
{
  do
  {
    stepper.runSpeed();
  } while (millis() < 10000);
  stepper.setSpeed(0); 
}


void setup() {
  
//  pinMode (ENCODER_A,INPUT_PULLUP);
//  pinMode (ENCODER_B, INPUT_PULLUP);
//  previousStateCLK = digitalRead(ENCODER_A);
  
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();  
  screen_1();
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(100);
  stepper.setSpeed(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
/*  int MenuPosition = EncoderKnob.read() / 4;
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

  currentStateCLK = EncoderKnob.read()/4 ;
  if(currentStateCLK != previousStateCLK)
  {
    if(currentStateCLK < previousStateCLK)
    {
      counter--;
      if(counter < 0) counter = 3; 
//      lcd.clear();
//      lcd.setCursor(2, 3);
//      lcd.print(counter);
    }
    else if(currentStateCLK > previousStateCLK)
    {
      counter++;
      if(counter > 3) counter = 0;
//      lcd.clear();
//      lcd.setCursor(2, 3);
//      lcd.print(counter);
   }
    screen_1();
    counterPreviousState = counter;
  }
  previousStateCLK = currentStateCLK;
}
