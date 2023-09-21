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
    Serial.begin(BAUD);

    EdgeReader edgereader;
    edgereader.test();

// enum
    // 0 - Unknown
    // 1 - setCurrent
    // 2 - cooling
    // 3 - heating
    // 4 - switchoff
    // from, enum mess, value, to
}

void loop() {

}