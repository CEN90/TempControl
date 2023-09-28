#include "EdgeReader.hpp"
#include "Monitor.hpp"

EdgeReader edgereader = EdgeReader();
Transitions transitions;

int current_state = 0;
int next_state = 0;
int temp_main_sensor = 0;
int temp_aux_sensor = 0;


void setup() {
    Serial.begin(BAUD);

    setPins();
    
    edgereader.readTransitions("states.aut", names, STATES_LEN);
    transitions = edgereader.edges;
    current_state = edgereader.start_state;

    int search = 70;
    int start = 0;
    int end = 0;

    auto len = edgereader.findState(search, &start, &end);

    Serial.print("Searched for ");
    Serial.print(search);
    Serial.print(". Start at ");
    Serial.print(start);
    Serial.print(", end at ");
    Serial.print(end);
    Serial.print(". Len of ");
    Serial.println(len);
    
}

void loop() {
    // temp_main_sensor = getTemperature(MAIN_SENSOR_IN);
    // temp_aux_sensor = getTemperature(AUX_SENSOR_IN);

    // // Compare main and aux sensors for bad data
    // if (temp_main_sensor != temp_aux_sensor) {
    //     Serial.println("ALERT: Mismatch between sensors!");
    // }

    // Look at temp and determine next state

    
}