/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

const int real_led = 13;
const int fake_led = 12;
const int blink_delay = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  """
  Previous Version:

  pinMode(real_led, OUTPUT);
  pinMode(fake_led, OUTPUT);
  """
  DDRB |= (1 << PB5) | (1 << PB4); // DDRB is the Data Direction Register for Port B
}

// the loop function runs over and over again forever
void loop() {
  """
  Previous Version:

  int a;
  digitalWrite(real_led, HIGH);  // turn the LED on (HIGH is the voltage level)
  a = real_led + 3;
  digitalWrite(fake_led, HIGH);  // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(blink_delay);                      // wait for a second
  digitalWrite(real_led, LOW);   // turn the LED off by making the voltage LOW
  a = real_led + 3;
  digitalWrite(fake_led, LOW);   // turn the LED off by making the voltage LOW
  delayMicroseconds(blink_delay);                      // wait for a second
  """
  // Simultanious version:
  PORTB |= (1 << PB5) | (1 << PB4);
  delay(blink_delay);
  PORTB &= ~((1 << PB5) | (1 << PB4));
  delay(blink_delay);
}
