#include <MsTimer2.h>
const int buttonPin = 6;  // the number of the pushbutton pin
const int ledPin = 4;    // the number of the LED pin
const int interruptPin = 2; // INT0 on Arduino UNO
const unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
long interval = 0;

void turnOff() {
    digitalWrite(ledPin, LOW);
    MsTimer2::stop();
    Serial.println("0"); // button off
}

void setupTimer(long interval) {
    MsTimer2::set(interval, turnOff);
    MsTimer2::start();
}

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleButtonPress, CHANGE);
    Serial.begin(9600);
}

void loop() {
    // Check if data is available on the serial port
    if (Serial.available() > 0) {
        // Read the incoming data as a string
        String receivedData = Serial.readStringUntil('\n'); // Option 1: Wait for newline character

        // Convert the received string to a number
        long receivedNumber = receivedData.toInt();

        // Print the received number back to the serial monitor
        Serial.print("I received: ");
        Serial.println(receivedNumber);

        // Set the timer using the received number plus one ms
        interval = receivedNumber + 1;
    }
}


void handleButtonPress() {
    unsigned long prevDebounceTime = lastDebounceTime;
    lastDebounceTime = millis();
    if (lastDebounceTime - prevDebounceTime < debounceDelay) {
        return;
    }

    if (digitalRead(buttonPin) == HIGH) {
      digitalWrite(ledPin, HIGH);
      setupTimer(interval);
      Serial.println("1"); // button and LED on
    } else {
        Serial.println("2"); // button off
    }
    
}
