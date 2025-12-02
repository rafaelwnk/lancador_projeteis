#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Servo.h>
#include <SoftwareSerial.h>

Adafruit_SSD1306 display(-1);
Servo servo;

const int servoButton = 3;
const int trig = 4;
const int echo = 5;

const float initialOffset = 5.50;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  servo.attach(6);
  servo.write(180);
  pinMode(servoButton, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  display.clearDisplay();

  float distance = -1;

  if(digitalRead(servoButton) == LOW) {
    servo.write(80);
    delay(180);
    distance = calculateDistance();
    servo.write(180);
    showResults(distance);
  }

   if (distance > 0 && distance < 4000) {
      Serial.print("DIST:");
      Serial.println(distance);
  }
}

float calculateDistance() {
  float distance = 9999;
  unsigned long startTime = millis();

  while(millis() - startTime < 2500) {
    long duration = 0;

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH, 25000);

    if (duration == 0) continue;

    float d = (duration * 0.034) / 2.0;

    if(d > 1 && d < distance) {
      distance = d;
    }
  }

  if (distance == 9999) return -1;

  return distance + initialOffset;
}

void showResults(float distance) {
  display.setFont(&FreeSerif12pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 38);
  display.print(distance);
  display.println(" cm");
  display.display();
  delay(500);
  display.clearDisplay();
}