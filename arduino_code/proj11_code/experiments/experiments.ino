

#include <Arduino_SensorKit.h>

#define ANGLE_THRESHOLD 30        // degrees
#define BUZZER_PIN 5             // Grove buzzer
#define SERVO_PIN 6               // Grove servo port

int i = 0;

bool buzzerOn = true;
float angle = 87.3;
float startTime;

void setup() {
  Serial.begin(9600);
  startTime = millis();
  
  // Oled.begin();
  // Oled.setFlipMode(true);
  // Oled.clear();
  // Oled.setFont(u8x8_font_chroma48medium8_r); 
  // pinMode(BUZZER_PIN, OUTPUT);
  // tone(BUZZER_PIN, 85);

}

void logToSerial() {
  Serial.print("timeElapsed=" + String(millis() - startTime) + ";");
  Serial.print("angle=" + String(angle) + ";");
  Serial.println("buzzerOn=" + String(buzzerOn));
}

void loop() {
  logToSerial();

  // Oled.clear();
  // Oled.setCursor(0, 0);
  // Oled.print("i: ");
  // Oled.print(i);

  // Oled.setCursor(0, 2);
  // Oled.print("My Name Is: ");
  // Oled.print("Ben");

  // Oled.setCursor(0, 4);
  // Oled.print("JAJAJA");

  // Oled.refreshDisplay();

  delay(200);
  i += 1;
}


