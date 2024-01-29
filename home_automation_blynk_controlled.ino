/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "*******"
#define BLYNK_DEVICE_NAME "********"
#define BLYNK_AUTH_TOKEN "*************"


// Comment this out to disable prints 
//#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include<BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
//#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,cooler_sw;
//unsigned int tank_volume;

 BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// This function is called every time the Virtual Pin 0 state changes
/*To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
 cooler_sw = param.asInt();
 if(cooler_sw)
 {
  cooler_control(ON);
  lcd.setCursor(7 , 0);
    lcd.print("CO_LR ON");
 }
 else
 {
  cooler_control(OFF); 
  lcd.setCursor(7, 0);
    lcd.print("CO_LR OFF");
 }
}
/*To turn ON and OFF heater based virtual PIN value*/
BLYNK_WRITE(HEATER_V_PIN )
{
   heater_sw = param.asInt();
 if(heater_sw)
 {
  heater_control(ON);
  lcd.setCursor(7 , 0);
    lcd.print("HT_R ON");
 }
 else
 {
  heater_control(OFF); 
  lcd.setCursor(7, 0);
    lcd.print("HT_R OFF");
 }
}
/*To turn ON and OFF inlet vale based virtual PIN value*/
/*BLYNK_WRITE(INLET_V_PIN)
{
  
}
/*To turn ON and OFF outlet value based virtual switch value*/
/*BLYNK_WRITE(OUTLET_V_PIN)
{
  
}
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_GAUGE,read_temperature());
}

/*To turn off the heater if the temperature raises above 35 deg C*/
void handle_temp(void)
{
  if((read_temperature() > float(35)) && heater_sw)
  {
    heater_sw = 0;
    heater_control(OFF);
    lcd.setCursor(7, 0);
    lcd.print("HT_R OFF");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN , "Temperature s above 35 degrees celsius\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN , "Turning OFF the heater\n");
    Blynk.virtualWrite(HEATER_V_PIN,0);
  }
}

/*To control water volume above 2000ltrs*/
/*void handle_tank(void)
{
  
  

}*/


void setup(void)
{
  init_ldr();
  /*initialize the lcd*/
    lcd.init();  
    /*turn the backlight */                   
    lcd.backlight();
    /*clear the clcd*/
    lcd.clear();
    /*cursor to the home */
    lcd.home();
    
    /*To display string*/
    lcd.setCursor(0, 0);
    lcd.print("T=");

    Blynk.begin(auth);
    
    init_temperature_system();
    timer.setInterval(500L, update_temperature_reading);
    
}

void loop(void) 
{
  Blynk.run();
   brightness_control();   
   String temperature;
   temperature = String(read_temperature(),2);
   lcd.setCursor(2,0);
   lcd.print(temperature);
   
}
