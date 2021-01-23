#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

#define BUTTON_ROTARY 7
#define ENCODER_A 0
#define ENCODER_B 1 

#define LCD_YOFFSET 1

LiquidCrystal_I2C lcd(0x27, 20, 4);

//9 pin is DIR, 8 is PUL in M542 Stepper driver
AccelStepper stepper(1, 8, 9);

//Rottary encoder
Encoder EncoderKnob(ENCODER_A, ENCODER_B);

int8_t counter = 0;
int8_t counterPreviousState = 0;
long currentStateCLK = -999;
long previousStateCLK = -999;
volatile byte button_state = 0;
void button_high(void);
void screen_start(void);
void screen_pos_pointer(void);

enum MENU_POS
{
 MAIN,
 MAIN_START,
 MAIN_CONTINOUS,
 MAIN_STEPS,
 MAIN_SETTINGS,
 MAIN_SETTINGS_STEPS,
 MAIN_SETTINGS_ACCELERATION,
 MAIN_SETTINGS_SPEED,
 MAIN_SETTINGS_MODE,
 MAIN_SETTINGS_MODE_STEP,
 MAIN_SETTINGS_MODE_CONTINOUS
};

enum MENU_POS menu_status = MAIN;

void screen_pos_pointer()
{
  if(counter != counterPreviousState)
  {
    lcd.setCursor(0, counter-LCD_YOFFSET);
    lcd.print(">");
    lcd.setCursor(0, counterPreviousState-LCD_YOFFSET);
    lcd.print(" ");
  }
  else if((counter == 0)&&(counterPreviousState == 0))
  {
    lcd.setCursor(0, counter-LCD_YOFFSET);
    lcd.print(">");    
  } 
}

void screen_1()
{
//  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Start   ");
  lcd.setCursor(2, 1);
  lcd.print("Stop    ");
  lcd.setCursor(2, 2);
  lcd.print("Settings");
}

void screen_start()
{
//  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Stop    ");
  lcd.setCursor(2, 1);
  lcd.print("Stoper   ");
  lcd.setCursor(2, 2);
  lcd.print("Settings");
}

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
  pinMode(BUTTON_ROTARY, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_ROTARY),button_high, RISING);
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

      lcd.setCursor(4,3);
      lcd.print(button_state);
 
    if(HIGH == button_state){
      if(MAIN == menu_status)
      {
        switch(counter)
        {
          case MAIN_START:
            screen_start();
            button_state = LOW;            
            break;
          default:
            screen_1();
            button_state = LOW;
        }        
      }
    }




  currentStateCLK = EncoderKnob.read()/4 ;
  if(currentStateCLK != previousStateCLK)
  {
    if(currentStateCLK < previousStateCLK)
    {
      counter--;
      if(counter < 1) counter = 4; 

      lcd.setCursor(2, 3);
      lcd.print(counter);
    }
    else if(currentStateCLK > previousStateCLK)
    {
      counter++;
      if(counter > 4) counter = 1;

      lcd.setCursor(2, 3);
      lcd.print(counter);
    }
  screen_pos_pointer(); 
  counterPreviousState = counter;

  }
  previousStateCLK = currentStateCLK;
}

void button_high()
{
 button_state = !button_state;
}
