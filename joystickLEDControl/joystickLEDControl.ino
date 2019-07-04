#include <LedControl.h>

int DIN = 12;
int CS =  11;
int CLK = 10;

int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;

char *str = "stay";
int dir = 5;

int changed = 5;

LedControl lc=LedControl(DIN,CLK,CS,0);

byte draw[8] =   {0x0, 0x0, 0x0, 0x10,0x0, 0x0, 0x0};
int down = 0;
byte mask[8] = {0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0};

void setup(){
 lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
 lc.setIntensity(0,15);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display

  Serial.begin(9600); 
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP); 
  
 
}

void loop(){ 

   
    /*if (test[0] != 0x1 && down == 0){
       test[0] >>= 1; 
    } else {
      down = 1; 
    }

    if (test[0] != 0x80 && down == 1) {
      test[0] <<= 1;  
    } else {
      down = 0;
     }*/
    xPosition = analogRead(xPin);
    yPosition = analogRead(yPin);
    buttonState = digitalRead(buttonPin);
   
    
     

    getByte(draw, xPosition - 10, yPosition - 10);
    
    printByte(draw);  
            
            
    //lc.clearDisplay(0);  
}


int inRange(int a) {

  if (a >= 300 && a <= 700) return 1;

  return 0;
}

void getByte(byte draw[8], int x, int y) {

  if (x < 0) x = 0;
  if (y < 0) y = 0;

  int arrayPos = (1023 - x) / 128;
  int bytePos = y / 128;

  for (int i = 0; i < 8; i ++) {
    draw[i] = mask[i];
  }

  draw[arrayPos] = 0x1;
  
  draw[arrayPos] <<= bytePos;

  Serial.println(arrayPos);

  Serial.println(bytePos);
  
}


void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
