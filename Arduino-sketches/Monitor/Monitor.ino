// #include "Edges.hpp"
#include "EdgeReader.hpp"

// Pin definitions
#define MAIN_SENSOR_IN    A0
#define HEATING_IN        8
#define COOLING_IN        9
#define SWITCH_OFF_IN     10

#define POLL_TIME         1000
#define ADC_RESOLUTION    1024.0
#define POLL_TIME         1000
#define STATES_LEN        6

String names[] = {
        "other",
        "controller.setcurrent",
        "time",
        "hc_unit.setheating",
        "hc_unit.setcooling",
        "hc_unit.switchoff"
        };

int current_state = 0;
int next_state = 0;

float getTemperature(int sensor) {
    // convert the ADC value to voltage in millivolt
    float voltage = 5.0 * (analogRead(sensor) / ADC_RESOLUTION);

    // convert the voltage to the temperature in Celsius
    return (voltage - 0.5) * 100;
}



EdgeReader edgereader = EdgeReader();
Transitions transitions;

void setup() {
    Serial.begin(BAUD);

    pinMode(HEATING_IN, INPUT);
    pinMode(COOLING_IN, INPUT);
    pinMode(SWITCH_OFF_IN, INPUT);
    pinMode(MAIN_SENSOR_IN, INPUT);
    
    auto trans = edgereader.readTransitions("states.aut", names, STATES_LEN);

    Serial.println(trans);

    for (size_t i = 0; i < edgereader.edges_size; i++)
    {
        edgereader.printEdge(edgereader.edges[i]);
    }
    
}

void loop() {

}