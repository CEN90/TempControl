#include "EdgeReader.hpp"
#include "Monitor.hpp"

EdgeReader edgereader = EdgeReader();
Transitions transitions;

int current_state = 0;
int next_state = 0;

int temp_main_sensor = 0;
int temp_aux_sensor = 0;

void checkTempSensors() {
    temp_main_sensor = getTemperature(MAIN_SENSOR_IN);
    temp_aux_sensor = getTemperature(AUX_SENSOR_IN);

    // Compare main and aux sensors for bad data
    // if (temp_main_sensor != temp_aux_sensor) {
    //     Serial.print("ALERT: Mismatch between sensors!");
    //     Serial.print(" Main sensor: ");
    //     Serial.print(temp_main_sensor);
    //     Serial.print(", aux sensor: ");
    //     Serial.println(temp_aux_sensor);
    // }
}


void setup() {
    Serial.begin(BAUD);

    setPins();
    
    edgereader.readTransitions("states.aut", names, STATES_LEN);
    transitions = edgereader.edges;
    current_state = edgereader.start_state;
}

void loop() {
    checkTempSensors();

    current_state = next_state;

    // On error alert and stop
    if (current_state == -1) {
        current_state = next_state = 0;
        // giveUp("ALERT: lost place in state, stopping!");
    }
    

    int startpos, endpos, len_states, sleep = POLL_TIME;
    len_states = edgereader.findState(current_state, &startpos, &endpos);

    if (len_states == -1) { // Shouldn't occur
        giveUp("Error at getting next state!");
    }
    
    // If either time or commands to hc_unit
    if (len_states == 1)
    {
        auto state = edgereader.edges[startpos];

        Serial.println(names[state.label]);

        switch (state.label)
        {
        case time:
            Serial.print("Sleeping ");
            sleep = state.value * MUL_TIME;
            Serial.print(state.value);
            Serial.println(" sec");
            break;

        case heating:
            if (!digitalRead(HEATING_IN)) {
                Serial.println(F("ALERT: Heating not activating!"));
            }
            break;

        case cooling:
            if (!digitalRead(COOLING_IN)) {
                Serial.println(F("ALERT: Cooling not activating!"));
            }
            break;

        case switchoff:
            if (!digitalRead(SWITCH_OFF_IN)) {
                Serial.println(F("ALERT: HC_Unit not turned off!"));
            }
            break;

        case setcurrent:
            next_state = state.to;
            Serial.print("Going from state ");
            Serial.print(state.from);
            Serial.print(" to state ");
            Serial.println(state.to);
            break;
        
        default:
            Serial.print("state.label: ");
            Serial.print(state.label);
            Serial.println(F(" - Error at reading state!"));
            break;
        } 

        Serial.println("");
        next_state = state.to;
    } 
    else // controller.setcurrent(<value>)
    {
        // Multiple choice, go through each and decide based on temp
        for (size_t i = 0; i < len_states; i++) {
            auto state = edgereader.edges[startpos + i];

            if (temp_main_sensor == state.value) {
                next_state = state.to;
            }
        }

        // No matching setcurrent found, alert
        if (next_state == current_state) { 
            Serial.println(F("ALERT: Temperature mismatch!"));
            Serial.print(F("Current temp: "));
            Serial.println(temp_main_sensor);

            next_state = -1; // Stopping execution
        }
    }
    

    delay(sleep);
}