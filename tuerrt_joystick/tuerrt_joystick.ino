#include <Servo.h>

Servo servoX;
Servo servoY;

int lazerOn = 0;

int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;

char *str = "stay";
int dir = 5;

int changed = 5;

  int prevState = LOW;



void setup(){
 
 
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP); 
  
  pinMode(13, OUTPUT);

  servoX.attach(8);
  servoY.attach(7);
  servoX.write(90);
  servoY.write(90);
  Serial.begin(9600);
}

void loop(){ 

    xPosition = analogRead(xPin);
    yPosition = analogRead(yPin);

     
    xPosition = 180 * ((double)xPosition / 1023);
    yPosition = 180 * ((double)yPosition / 1023);

    Serial.println(xPosition);
          Serial.println(yPosition);

    servoX.write(yPosition);
    servoY.write(xPosition);  
    
    buttonState = digitalRead(buttonPin);
    

     if (buttonState == HIGH) digitalWrite(13, HIGH);
     if (buttonState == LOW) digitalWrite(13,LOW);

         
       
          
            
 }


int inRange(int a) {

  if (a >= 300 && a <= 700) return 1;

  return 0;
}
