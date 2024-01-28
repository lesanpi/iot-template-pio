#include "Development.h"

void log(String text, String name)
{
    Serial.print("[");
    Serial.print(name);
    Serial.print("] ");
    Serial.println(text);
}
