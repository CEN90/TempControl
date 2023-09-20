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
    // Serial.begin(BAUD);

    EdgeReader edgereader;
    edgereader.test();
    // auto edges = edgereader.getEdges();
    
    // // Serial.println(F("Got here"));
    // Serial.println(edges.size());

    // for (size_t i = 0; i < edges.size(); i++)
    // {
    //     Serial.println(i);
        // edgereader.printEdge(edges[i]);
        // Serial.print(edges[i].from);
        // Serial.print(" -> ");
        // Serial.print(edges[i].label);
        // Serial.print(" -> ");
        // Serial.print(edges[i].to);
        // Serial.print(" -> ");
        // Serial.println(edges[i].value);
    // }
    
    // auto test = edges.tester();

// enum
    // 1 - setCurrent
    // 2 - cooling
    // 3 - heating
    // 4 - switchoff
    // from, enum mess, value, to
}

void loop() {

}