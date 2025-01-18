#include <Arduino.h>

void assertImpl(bool cond, int line)
{
    if (!cond)
    {
        Serial.println("Error");
        Serial.println(line);
        while (true)
        {
            __asm__ __volatile__("nop");
        }
    }
}

#define assert(cond) assertImpl(cond, __LINE__)