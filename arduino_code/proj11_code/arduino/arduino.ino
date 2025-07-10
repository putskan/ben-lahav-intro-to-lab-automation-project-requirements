#include <Arduino_SensorKit.h>
#include <Servo.h>

#define ANGLE_THRESHOLD 70        // degrees
#define BUZZER_PIN 5             // Grove buzzer
#define SERVO_PIN 6               // Grove servo port
#define FAN_PIN 7
#define ANGLE_NOISE_THRESHOLD 0
#define ANGLE_SMOOTHING_PARAM 0.2

Servo fanServo;
bool buzzerOn = false;
float prevAngle = 99999;
float angle;
float startTime;

void setup() {
  Serial.begin(9600);

  Accelerometer.begin();

  fanServo.attach(SERVO_PIN);
  
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);

  Oled.begin();
  Oled.setFlipMode(true);
  Oled.clear();
  Oled.setFont(u8x8_font_chroma48medium8_r);

  startTime = millis();
}

void logToOled() {
  Oled.clear();
  Oled.setCursor(0, 0);
  Oled.print("Angle: ");
  Oled.print(angle);
  Oled.print("°");

  Oled.setCursor(0, 2);
  Oled.print("Fan: ");
  Oled.print(abs(angle) > ANGLE_THRESHOLD ? "OFF" : "ON");

  Oled.setCursor(0, 4);
  Oled.print("Buzzer: ");
  Oled.print(buzzerOn ? "ON" : "OFF");
  Oled.refreshDisplay();
}

void logToSerial() {
  Serial.print("timeElapsed=" + String(millis() - startTime) + ";");
  Serial.print("angle=" + String(angle) + ";");
  Serial.println("buzzerOn=" + String(buzzerOn));
}

void loop() {
  // Read accelerometer
  float ax = Accelerometer.readX();
  float ay = Accelerometer.readY();
  float az = Accelerometer.readZ();

  // Calculate tilt angle (pitch) in degrees
  float currAngle = atan2(ay, az) * 180 / PI;

  angle = (1 - ANGLE_SMOOTHING_PARAM) * angle + ANGLE_SMOOTHING_PARAM * currAngle;
  // Serial.println(angle);
  // Serial.println(currAngle);

  // Map tilt to servo range
  int servoAngle = constrain(map(angle, -90, 90, 0, 180), 0, 180);

  if ((abs(angle) > ANGLE_THRESHOLD)) {
    // Tilt too far: stop fan & sound buzzer
    fanServo.detach();
    tone(BUZZER_PIN, 85);
    buzzerOn = true;
    digitalWrite(FAN_PIN, LOW);

  } else if(abs(prevAngle - angle) >= ANGLE_NOISE_THRESHOLD) {
    prevAngle = angle;
    
    // Safe angle: run fan & silence buzzer
    if (!fanServo.attached()) fanServo.attach(SERVO_PIN);
    fanServo.write(servoAngle);
    noTone(BUZZER_PIN);
    buzzerOn = false;
    digitalWrite(FAN_PIN, HIGH);
  }



  // Display status on OLED
  logToSerial();
  logToOled();



  delay(50);
}
