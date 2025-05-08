#pragma once
#include <Arduino.h>

constexpr byte LED_PIN = 13;

void assertImpl(bool cond, int line, int freq)
{
    if (!cond)
    {
        Serial.println("ERROR LINE:");
        Serial.println(line);
        while (true)
        {
            digitalWrite(LED_PIN, HIGH);
            delay(freq);
            digitalWrite(LED_PIN, LOW);
            delay(freq);
        }
    }
}

#define assert(cond, freq) assertImpl(cond, __LINE__, freq)

#define echo(x) Serial.println(x);