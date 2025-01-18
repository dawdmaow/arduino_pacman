#pragma once
#include <Arduino.h>

constexpr byte LED_PIN = 13;

void assertImpl(bool cond, int line)
{
    if (!cond)
    {
        Serial.println("ERROR LINE:");
        Serial.println(line);
        while (true)
        {
            digitalWrite(LED_PIN, HIGH);
            delay(250);
            digitalWrite(LED_PIN, LOW);
            delay(250);
        }
    }
}

#define assert(cond) assertImpl(cond, __LINE__)

#define echo(x) Serial.println(x);