#include <MsTimer2.h>
const int buttonPin = 6;  // the number of the pushbutton pin
const int ledPin = 4;    // the number of the LED pin
const int interruptPin = 2; // INT0 on Arduino UNO

long interval = 0;

void turnOff() {
    digitalWrite(ledPin, LOW);
    MsTimer2::stop();
}

void setupTimer(long interval) {
    MsTimer2::set(interval / 1000.0, turnOff);
    MsTimer2::start();
}

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(interruptPin, INPUT_PULLUP);
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
    if (digitalRead(buttonPin) == HIGH) {
      digitalWrite(ledPin, HIGH);
    }
    setupTimer(interval);
}
