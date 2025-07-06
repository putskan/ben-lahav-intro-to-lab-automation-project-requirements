const int buttonPin = 6;  // the number of the pushbutton pin
const int ledPin = 4;    // the number of the LED pin
const int interruptPin = 2; // INT0 on Arduino UNO

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);

  pinMode(interruptPin, INPUT_PULLUP); // Set pin as input with pull-up resistor

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), handleButtonPress, CHANGE);

}

void loop() {
  // read the state of the pushbutton value:
  int a = 0;
  for (int i = 0; i< 10000; i++){
    a = 0;
    // Serial.println(i);
  }

}

void handleButtonPress() {
    buttonState = digitalRead(buttonPin);

    Serial.println("Handle Interrupt");
    if (buttonState == HIGH) {
      Serial.println("Button was clicked.");
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
}




