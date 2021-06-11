// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include<Timers.h>
#include "FontData.h"
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_ZONES 2
#define ZONE_SIZE 4
#define MAX_DEVICES 8
#define CS_PIN 3
#define RIGHT 5
#define CENTER 6
#define LEFT 7
#define SCROLL_SPEED 50

#define ZONE_UPPER  1
#define ZONE_LOWER  0
// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

bool invertUpperZone = false;
// Setup for software SPI:
// #define DATAPIN 2
// #define CLK_PIN 4
// MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
Timer Right_Timer, Center_Timer, Left_Timer;
boolean T1=true, T2=true, T3=true;
boolean prevT1=true, prevT2=true, prevT3=true;
uint8_t dotZ[] = { 5, 105, 89, 73, 77, 75 }; //Ż
char *Napisy[]=
{
  "POMOC DROGOWA",
  "STRA{ PO{ARNA",
  "WERSJA DEMONSTRACYJNA",
  "ANDRZEJ BEDZIE DOBRZE"
};
int i=0;
int brigth=0;

void setup() 
{
  Serial.begin(9600);
  invertUpperZone = (HARDWARE_TYPE == MD_MAX72XX::GENERIC_HW || HARDWARE_TYPE == MD_MAX72XX::PAROLA_HW);
  Right_Timer.begin(STOP);
  Center_Timer.begin(STOP);
  Left_Timer.begin(STOP);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(CENTER,INPUT);
  // Intialize the object:
  /*
  myDisplay.addChar('{', dotZ);*/
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.begin(MAX_ZONES);
  
  // Clear the display:
  myDisplay.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  myDisplay.setFont(ZONE_LOWER, BigFontLower);
  //myDisplay.setIntensity(7);
  myDisplay.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  myDisplay.setFont(ZONE_UPPER, BigFontUpper);
  myDisplay.setCharSpacing(myDisplay.getCharSpacing() * 2); // double height --> double spacing
  if (invertUpperZone)
  {
    myDisplay.setZoneEffect(ZONE_UPPER, true, PA_FLIP_UD);
    myDisplay.setZoneEffect(ZONE_UPPER, true, PA_FLIP_LR);
  }
 
}
void loop() 
{
 
  if(myDisplay.displayAnimate())
  if(myDisplay.getZoneStatus(ZONE_LOWER) && myDisplay.getZoneStatus(ZONE_UPPER))
  {
   
    
    myDisplay.setFont(ZONE_LOWER, BigFontLower);
    myDisplay.setFont(ZONE_UPPER, BigFontUpper);
    if (invertUpperZone)
    {
       myDisplay.displayZoneText(ZONE_LOWER, Napisy[i], PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
       myDisplay.displayZoneText(ZONE_UPPER, Napisy[i], PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
    }
    else
    {
       myDisplay.displayZoneText(ZONE_LOWER, Napisy[i], PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
       myDisplay.displayZoneText(ZONE_UPPER, Napisy[i], PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    myDisplay.displayClear();
    myDisplay.synchZoneStart();
  }
  
  buttChecker(&Center_Timer, &T1, CENTER,500,&i,3,' ');
  buttChecker(&Left_Timer,&T2,LEFT,200,&brigth,15,'+');
  buttChecker(&Right_Timer,&T3,RIGHT,200,&brigth,15,'-');
  myDisplay.setIntensity(brigth);
  
  
  
}
void buttChecker(Timer * T, boolean *cond,int  numb, int period, int *i, int range, char operate)
{
  
  int intern=*i;
  if(!digitalRead(numb) && *cond==true) 
    {
      
      *cond=false;
      switch(operate)
      {
        case '-':
          if(intern>0) intern--;
          else intern=0;
          break;
        case '+':
          if(intern<range) intern++;
          else intern=range;
          break;

        default:
          if(intern<range) intern++;
          else intern=0;
          break;
      }
      
      T->begin(period);
      *i=intern;
      //return true;
    }
  if(T->available())
  {
    *cond=true;
    T->begin(STOP);
    //return false;
  }
  
}
