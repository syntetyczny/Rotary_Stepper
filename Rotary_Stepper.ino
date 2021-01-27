#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

#define BUTTON_ROTARY 7
#define ENCODER_A 0
#define ENCODER_B 1 

#define LCD_YOFFSET 1
#define LCD_PS_POSITION 14

#define MOTOR_STEPS 200

LiquidCrystal_I2C lcd(0x27, 20, 4);

//9 pin is DIR, 8 is PUL in M542 Stepper driver
AccelStepper stepper(1, 8, 9);

//Rottary encoder
Encoder EncoderKnob(ENCODER_A, ENCODER_B);

uint8_t counter = 0;
uint8_t continous_percent = 100;
uint16_t motor_steps = MOTOR_STEPS;
int8_t counterPreviousState = 0;
long currentStateCLK = -999;
long previousStateCLK = -999;
volatile byte button_state = 0;
void button_high(void);

void screen_start(void);
void screen_pos_pointer(void);
void screen_settings(void);
void screen_settings_steps(void);

enum MENU_POS
{
 MAIN,
 MAIN_START,
 MAIN_SETTINGS,
 MAIN_SETTINGS_STEPS,
 MAIN_SETTINGS_ACCELERATION,
 MAIN_SETTINGS_SPEED,
 MAIN_SETTINGS_MODE,
 MAIN_SETTINGS_MODE_STEP,
 MAIN_SETTINGS_MODE_CONTINOUS
};

enum MOTOR_MODES
{
  CONTINOUS,
  STEP
};

enum MENU_POS menu_status = MAIN;
enum MOTOR_MODES motor_mode = CONTINOUS;

void screen_pos_pointer()
{
  if(counter != counterPreviousState)
  {
    lcd.setCursor(0, counter-LCD_YOFFSET);
    lcd.print(">");
    lcd.setCursor(0, counterPreviousState-LCD_YOFFSET);
    lcd.print(" ");
  }
  else if((counter == 1)&&(counterPreviousState == 1))
  {
    lcd.setCursor(0, counter-LCD_YOFFSET);
    lcd.print(">");    
  } 
}

void screen_1()
{ 
  lcd.setCursor(2, 0);
  lcd.print("Start   ");
  lcd.setCursor(LCD_PS_POSITION,0);
  lcd.print(continous_percent);
  lcd.setCursor(18,0);
  lcd.print("%");
  lcd.setCursor(2, 1);
  lcd.print("Settings");
}

void screen_start()
{
//  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Stop    ");
  lcd.setCursor(LCD_PS_POSITION,0);
  lcd.print("   ");
  lcd.setCursor(2, 1);
  lcd.print("         ");

  int encoder_state = EncoderKnob.read();
  int current_encoder_state = 0;
  EncoderKnob.write(current_encoder_state);
  int previous_encoder_state = 0;
  if(CONTINOUS == motor_mode)
  {
  do{
    lcd.setCursor(19,3);
    lcd.print(button_state);
    lcd.setCursor(LCD_PS_POSITION,0);
    current_encoder_state = EncoderKnob.read()/4;
    if(current_encoder_state != previous_encoder_state)
    {
    if(current_encoder_state > previous_encoder_state)
    {
      continous_percent++;
      if(continous_percent > 100) continous_percent = 100;
    }
    if(current_encoder_state < previous_encoder_state)
    {
      continous_percent--;
      if(continous_percent > 254) continous_percent = 0;
    }
    }

    previous_encoder_state = current_encoder_state;
    lcd.print("   ");
    lcd.setCursor(LCD_PS_POSITION,0);
    lcd.print(continous_percent);
    lcd.setCursor(18,0);
    lcd.print("%");
   
  }while(LOW == button_state);
  button_state = LOW;
  EncoderKnob.write(encoder_state);
  }
screen_1();
}

void run_motor_countinous()
{
  do
  {
    stepper.runSpeed();
  } while (millis() < 10000);
  stepper.setSpeed(0); 
}

void screen_settings()
{
    lcd.setCursor(2, 0);
    lcd.print("Steps   ");
    lcd.setCursor(LCD_PS_POSITION,0);
    lcd.print(motor_steps);
    lcd.setCursor(LCD_PS_POSITION+3,0);
    lcd.print(" st");
 


    lcd.setCursor(2, 1);
    lcd.print("Acceleration");
  
    lcd.setCursor(2, 2);
    lcd.print("Speed   ");
    lcd.setCursor(LCD_PS_POSITION,2);
    lcd.print("1000");
  
    lcd.setCursor(2, 3);
    lcd.print("Mode");
}

void screen_settings_steps()
{
  int encoder_state = EncoderKnob.read();
  int current_encoder_state = 0;
  EncoderKnob.write(current_encoder_state);
  int previous_encoder_state = 0;

  do{
    lcd.setCursor(19,3);
    lcd.print(button_state);
    lcd.setCursor(LCD_PS_POSITION,0);
    current_encoder_state = EncoderKnob.read()/4;
    if(current_encoder_state != previous_encoder_state)
    {
    if(current_encoder_state > previous_encoder_state)
    {
      motor_steps++;
    }
    if(current_encoder_state < previous_encoder_state)
    {
      motor_steps--;
    }
    }

    previous_encoder_state = current_encoder_state;
    lcd.print("   ");
    lcd.setCursor(LCD_PS_POSITION,0);
    lcd.print(motor_steps);
    lcd.setCursor(18,0);
    lcd.print("st");
   
  }while(LOW == button_state);
  button_state = LOW;
  EncoderKnob.write(encoder_state);
  screen_settings();
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

    lcd.setCursor(19,3);
    lcd.print(button_state);

    if(HIGH == button_state)
    {  
      if(MAIN == menu_status)
      {
        button_state = LOW;
        switch(counter)
        {
          case MAIN_START:
            screen_start();            
            break;
          case MAIN_SETTINGS:
            screen_settings();
            menu_status = MAIN_SETTINGS;
            counter = 1;
            counterPreviousState = 1;
            EncoderKnob.write(0);
            previousStateCLK = 0;
            lcd.setCursor(0,1);
            lcd.print(" ");
           screen_pos_pointer();
            break;
          default:
            screen_1();
        }        
      }
    }
    if(HIGH == button_state)
    {
      if(MAIN_SETTINGS == menu_status)
      {
        button_state = LOW;
        switch(counter)
        {
          case MAIN_SETTINGS_STEPS - MAIN_SETTINGS:
            screen_settings_steps();
            break;
          default:
            screen_settings();
        }
        
      }
    }
    




  currentStateCLK = EncoderKnob.read()/4 ;
  if(currentStateCLK != previousStateCLK)
  {
    if(currentStateCLK < previousStateCLK)
    {
      counter--;
      if(counter > 254) counter = 3;  

      lcd.setCursor(17, 3);
      lcd.print(counter);
    }
    else if(currentStateCLK > previousStateCLK)
    {
      counter++;
      if(counter > 3) counter = 0;

      lcd.setCursor(17, 3);
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
