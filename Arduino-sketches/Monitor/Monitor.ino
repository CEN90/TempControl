// Pin definitions
// #define HEATING_IN        8
// #define COOLING_IN        9
// #define SWITCH_OFF_IN     10

// #define HEATING_OUT       5
// #define COOLING_OUT       6
// #define SWITCH_OFF_OUT    7

#define POLL_TIME         1000

#include "EdgeReader.hpp"

void setup() {
    Serial.begin(115200);

    EdgeReader edges;
    auto test = edges.tester();

    Serial.print(test.from);
    Serial.print(" -> ");
    Serial.print(test.label);
    Serial.print(" -> ");
    Serial.print(test.to);
    Serial.print(" -> ");
    Serial.println(test.value);
}

void loop() {

}