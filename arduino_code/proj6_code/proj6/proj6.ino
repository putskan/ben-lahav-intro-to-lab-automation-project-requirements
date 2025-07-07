#include <MsTimer2.h>

const int buttonPin = 6;
const int ledPin = 4;
const int interruptPin = 2;
const int timerPeriod = 30;
int buttonState = 0;


void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleButtonPress, CHANGE);
  MsTimer2::set(timerPeriod, turnOff);
}

void loop() {
  int a = 0;
  for (int i = 0; i < 100000; i++){
    a = 0;
  }
}


void turnOff() {
    digitalWrite(ledPin, LOW);
    MsTimer2::stop();
}

void handleButtonPress() {
    Serial.println("Handle Interrupt");
    if (digitalRead(buttonPin) == HIGH && digitalRead(ledPin) == LOW) {
      digitalWrite(ledPin, HIGH);
      MsTimer2::start(); 
    }
}
