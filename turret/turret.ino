#include <Servo.h>

Servo servoX;
Servo servoY;

String serialData;

void setup() {
  servoX.attach(8);
  servoY.attach(7);
  Serial.begin(9600);
  Serial.setTimeout(10);

  servoX.write(90);
  servoY.write(90);

}

void loop() {
  

}

void serialEvent() {
  serialData = Serial.readString();  

  servoX.write(parseDataX(serialData));
  servoY.write(parseDataY(serialData));  
  
}

int parseDataX(String data) {
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("X"), 1); 

   return data.toInt();
   
}


int parseDataY(String data) {
  data.remove(0, data.indexOf("Y") + 1);
     return data.toInt();
   
}
