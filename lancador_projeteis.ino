#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Servo.h>

Adafruit_SSD1306 display(-1);
Servo servo;

const int servoButton = 2;
const int trig = 3;
const int echo = 4;
const int sensorButton = 7;

const float initialOffset = 16.0;

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  servo.attach(5);
  servo.write(180);
  pinMode(servoButton, INPUT_PULLUP);
  pinMode(sensorButton, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop()
{
  display.clearDisplay();

  float distance;

  if(digitalRead(servoButton) == LOW) {
    launch();
  }

  if(digitalRead(sensorButton) == LOW) {
    distance = calculateDistance();
    showResults(distance);
  }
}

void launch() {
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);
}

float calculateDistance() {
  float sum = 0;
  int validReadings = 0;

  for (int i = 0; i < 5; i++) {
    long duration = 0;

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    duration = pulseIn(echo, HIGH, 25000);

    if (duration > 50 && duration < 20000) {
      float distance = duration * 0.034 / 2;
      sum += distance;
      validReadings++;
    }
  }

  if (validReadings == 0) return -1;

  return (sum / validReadings) + initialOffset;
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