#include "states.hpp"
#include <Wire.h>

constexpr int SENSOR = 2;
constexpr int SENSOR_DATA_SIZE = 2;

// Input Arduino Uno
// constexpr int TEMP_MAIN = PIN_A0;
// constexpr int TEMP_AUX = PIN_A1;
// constexpr int HEATING = PIN3;
// constexpr int COOLING = PIN2;

// Input Arduino Mega pinout
// constexpr int TEMP_MAIN = PIN4;
// constexpr int TEMP_AUX = PIN4;
constexpr int HEATING = 13;
constexpr int COOLING = 12;

constexpr int input_pins_len = 2;
constexpr int input_pins[input_pins_len] = { HEATING, COOLING };

int temp_main = 0;
int temp_aux = 0;

struct state_input_t {
    int state;
    int inputs_len;
    int valid_inputs[input_pins_len + 2];
};

struct input_t {
    int temp_main;
    int temp_aux;
    int commands;
    boolean unchanged;

    void print() {
        Serial.print("Commands ");
        Serial.println(commands);
        Serial.print("Temp_main: ");
        Serial.print(temp_main);
        Serial.print(" Temp_aux: ");
        Serial.println(temp_aux);
    }
};

state_input_t expected_inputs[4] = {
    { controller_setcurrent, 3, { 0, 1, 2 } },
    { hc_unit_switchoff, 1, { 0 } },
    { hc_unit_setheating, 1, { 1 } },
    { hc_unit_setcooling, 1, { 2 }},
};

String output_strings[4] = {
    "Read temperature",
    "Turned off hc-unit",
    "Turned on heating"
    "Turned on cooling",
};

void setPins() {
    for (size_t i = 0; i < input_pins_len; i++) {
        pinMode(input_pins[i], INPUT);
    }

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
    for (int i = 0; i < input_pins_len; i++) {
        result += !digitalRead(input_pins[i]) << i; // active low for relay
    }

    int prev_temp_main = prev->temp_main;
    int prev_temp_aux = prev->temp_aux;

    // Lägg till temp-läsning här!
    readTemp(prev);

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

    int value = transitions[current_state][Value];

    // Lägg till matchning av temp_main & temp_aux här!
    if (value != -1 && value != current.temp_main) {
        match = false;
    }
    

    return match;
}
