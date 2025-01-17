#include <Arduino.h>

constexpr byte CLK = 13;
constexpr byte DIN = 11;
constexpr byte CS = 10;
constexpr byte LED_PIN = 13;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World!");
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}