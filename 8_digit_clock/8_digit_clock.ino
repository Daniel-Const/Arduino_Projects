//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 13 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,13,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  Serial.begin(9600);
}

void longToSong(long x){
    char buff[9];
    sprintf(buff, "%8d", x);
    for (byte i = 0; i < 8; i ++) {
      lc.setChar(0, 7 - i, buff[i], false);
    }  
  
  
}
  int oldnums[8];
void intToPrint(long x) {
  int nums[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
  int index = 7;
  int len = 0;
   while (x > 0 && index >= 0) {
       nums[index] =  x % 10;
       x = x / 10;
       len ++;
      // Serial.println(nums[index]);
       index --;       
   }
    //Serial.println("==========");
  
   for (int i = 7; i >=0 ; i --) {
     if (nums[i] != -1) len ++;
     
     //lc.clearDisplay(i);
   }

   int inverseLen = 7 - len;
   int count = 0;
   for (int i = len; i >=0 ; i --){
      if (oldnums[inverseLen - (i - len)] != nums[inverseLen - (i - len)]) lc.setDigit(0,i,nums[inverseLen - (i - len)],false);
      oldnums[inverseLen - (i - len)] = nums[inverseLen - (i - len)];
    }
  
}

unsigned long x = 0;
void loop() { 
  //writeArduinoOn7Segment();
  //scrollDigits();
  intToPrint(x);
   
  x ++;
  
  
}
