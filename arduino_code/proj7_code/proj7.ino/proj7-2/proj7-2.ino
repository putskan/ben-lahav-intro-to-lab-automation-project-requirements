#include <Servo.h>

Servo myServo;
const int potPin = A0;

void setup() {
  myServo.attach(7);
}

void loop() {
  int potValue = analogRead(potPin); 
  int angle = map(potValue, 0, 1023, 0, 175);
  myServo.write(angle);
  delay(15);
}
