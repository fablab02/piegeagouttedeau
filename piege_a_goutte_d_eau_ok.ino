#include <Wire.h>
#include <I2C_LCD.h>

I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

#define BUTTON_PIN 6
#define LED_PIN 7
#define APN_TRIGGER_PIN 8
#define FLASH_TRIGGER_PIN 9
#define SENSOR_PIN A2
#define DELAY_PIN A3
#define SENSIB_PIN A1

int delaiFlash=0;
int seuil=0;
int sensor=0;
boolean mode=false;

void setup(){
  Wire.begin();         //I2C controller initialization.
  
  pinMode(BUTTON_PIN,INPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(APN_TRIGGER_PIN,OUTPUT);
  pinMode(FLASH_TRIGGER_PIN,OUTPUT);
  pinMode(SENSOR_PIN,INPUT);
  pinMode(DELAY_PIN,INPUT);
  pinMode(SENSIB_PIN,INPUT);
  Serial.begin(115200);  
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
  delay(1000);            //Delay for 1s.

  //8*16 font size��auto new line��black character on white back ground.
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
 
}


unsigned int old_time = 0;
void loop() {
  unsigned int time = millis();
  Serial.println(time - old_time);
  sensor = analogRead(SENSOR_PIN);
   
  if(digitalRead(BUTTON_PIN) == HIGH)        // detecte pression sur bouton : OK pour demarrer
  {
    mode = true;             // paré à shooter !
    digitalWrite(LED_PIN, HIGH);                    // allume la lampe d'action
    digitalWrite(APN_TRIGGER_PIN,HIGH);  // déclenche l'appareil photo
    LCD.CharGotoXY(0,10);
    LCD.print(" ready to capture ");
  }
  if(!mode){
    delaiFlash = analogRead(DELAY_PIN);     // lit la valeur du délai de déclenchement du flash
    seuil = analogRead(SENSIB_PIN);             // lit la valeur du seuil de sensibilité
    LCD.CharGotoXY(0,0);       //Set the start coordinate.
    LCD.print("sensor :");
    LCD.print(sensor);
    LCD.CharGotoXY(0,10);
    LCD.print("                      ");
    LCD.CharGotoXY(0,20);
    LCD.print(" seuil: ");
    LCD.print(seuil);
    LCD.print("      ");
    LCD.CharGotoXY(0,30);
    LCD.print(" delaiFlash: ");
    LCD.print(delaiFlash);
    LCD.print("      ");
    
    Serial.print("sensor : ");
    Serial.print(analogRead(SENSOR_PIN));
    Serial.print(" delaiFlash : ");
    Serial.print(delaiFlash);
    Serial.println("  ");
  }
  if((mode==true) && (analogRead(SENSOR_PIN) > seuil)){
    //  si le mode de fonctionnement est actif et que le courant détecté par le curseur < seuil il y a
    //  interruption du faisceau (passage de la goutte ou autre).
    delay(delaiFlash);  
    digitalWrite(FLASH_TRIGGER_PIN,HIGH); // envoi impulsion au flash.
    delay(25);                                                       // durée d'impulsion necessaire au flash
    digitalWrite(FLASH_TRIGGER_PIN,LOW); // fin d'impulsion flash
    mode = false;    
    digitalWrite(APN_TRIGGER_PIN,LOW);     // fin de la session
    digitalWrite(LED_PIN, LOW);                     // eteint la led d'action.
    LCD.CharGotoXY(0,10);
    LCD.print(" capture ok              ");
    delay(1000);
  }
  old_time = time;
}

