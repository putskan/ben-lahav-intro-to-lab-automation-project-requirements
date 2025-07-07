#include <MsTimer2.h>

const int buttonPin = 6;
const int ledPin = 4;
const int interruptPin = 2;
const int timerPeriod = 30;
int buttonState = 0;
unsigned long timer = 0;


void setup() {
  Serial.begin(9600);

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), handleButtonPress, CHANGE);

  MsTimer2::set(timerPeriod / 30, turnOff);
  MsTimer2::start();


}

void loop() {
  int a = 0;
  for (int i = 0; i < 100000; i++){
    a = 0;
  }
}


void turnOff() {
  if (millis() - timer > timerPeriod) {
    digitalWrite(ledPin, LOW);
    timer = 0;
  }
}

void handleButtonPress() {
    Serial.println("Handle Interrupt");
    if (digitalRead(buttonPin) == HIGH && digitalRead(ledPin) == LOW) {
      Serial.println("Button was clicked.");
      digitalWrite(ledPin, HIGH);
      timer = millis();
    }
}





