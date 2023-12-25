#include "states.hpp"
#include <Wire.h>

#define SENSOR       2
#define SENSOR_DATA_SIZE  2

// Input Arduino Uno
// #define HEATING = PIN3;
// #define COOLING = PIN2;

// Input Arduino Mega pinout
// #define TEMP_MAIN = PIN4;
// #define TEMP_AUX = PIN4;

#define COOLING    13
#define HEATING    12
#define INPUT_LEN  2

constexpr int input_pins[INPUT_LEN] = { HEATING, COOLING };

int temp_main = 0;
int temp_aux = 0;

struct state_input_t {
    int state;
    int inputs_len;
    int valid_inputs[INPUT_LEN + 2];
};

struct input_t {
    int temp_main;
    int temp_aux;
    int commands;
    boolean unchanged;

    void print() {
        Serial.print("Command read: ");
        Serial.print(commands);
        Serial.print(", Temp_main: ");
        Serial.print(temp_main);
        Serial.print(" Temp_aux: ");
        Serial.println(temp_aux);
    }
};

state_input_t expected_inputs[4] = {
    { controller_updatetemp, 3, { 0, 1, 2 } },
    { hc_unit_setcooling, 1, { 2 }},
    { hc_unit_switchoff, 1, { 0 } },
    { hc_unit_setheating, 1, { 1 } },
};

String output_strings[4] = {
    "Read temperature",
    "Turned on cooling",
    "Turned off hc-unit",
    "Turned on heating",
};

void setPins() {
    for (size_t i = 0; i < INPUT_LEN; i++) {
        pinMode(input_pins[i], INPUT);
    }

    delay(1000);

    Wire.begin();
}

void readTemp(input_t *input) {
    Wire.requestFrom(SENSOR, SENSOR_DATA_SIZE);

    while (Wire.available()) { // peripheral may send less than requested
        input->temp_main = Wire.read(); // receive a byte as character
        input->temp_aux = Wire.read(); // receive a byte as character
    }    
}

void readInputs(input_t *prev) {
    int result = 0;
    for (int i = 0; i < INPUT_LEN; i++) {
        result += !digitalRead(input_pins[i]) << i; // active low for relay
    }

    int prev_temp_main = prev->temp_main;
    int prev_temp_aux = prev->temp_aux;

    // Lägg till temp-läsning här!
    readTemp(prev);

    // if (prev->commands == result) {
    if (prev->commands == result && prev_temp_main == prev->temp_main) {
        prev->unchanged = true;
    } else {
        prev->commands = result;
        prev->unchanged = false;
    }
    
}

boolean is_expected_input(input_t current, state_input_t state, int current_state) {
    boolean match = false;
    
    for (size_t i = 0; i < state.inputs_len; i++) {
        if (current.commands == state.valid_inputs[i]) {
            match = true;
        }
    }

    transition_labels label = transitions[current_state][Label];
    int value = transitions[current_state][Value];

    if (label == controller_updatetemp && value != current.temp_main) {
        match = false;
    }
    

    return match;
}
