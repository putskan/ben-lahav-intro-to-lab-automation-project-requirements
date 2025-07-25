#include <MD_PWM.h>

#define PWM_PIN 4
#define ROTARY_PIN A0
#define FREQ 30

MD_PWM ledPwm(PWM_PIN);

void setup() {
  ledPwm.begin(FREQ);
  ledPwm.write(128);
}

void loop() {
  int rotaryValue = analogRead(ROTARY_PIN);
  uint8_t ledValue = map(rotaryValue, 0, 1023, 0, 255);
  ledPwm.write(ledValue);
  delay(10);
}
