#include "states.hpp"

// Input Arduino Uno
constexpr int TEMP_MAIN = PIN4;
constexpr int TEMP_AUX = PIN4;
constexpr int HEATING = PIN3;
constexpr int COOLING = PIN2;

// Input Arduino Mega pinout
// constexpr int TEMP_MAIN = PIN4;
// constexpr int TEMP_AUX = PIN4;
// constexpr int HEATING = PIN3;
// constexpr int COOLING = PIN2;

constexpr int input_pins_len = 4;
constexpr int input_pins[input_pins_len] = { TEMP_MAIN, TEMP_AUX, HEATING, COOLING };

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
};

state_input_t expected_inputs[4] = {
    { controller_setcurrent, 4, { 8, 9, 11, 14 } },
    { hc_unit_switchoff, 3, { 0, 1, 3 } },
    { hc_unit_setheating, 1, { 6 } },
    { hc_unit_setcooling, 1, { 2 }},
};

String output_strings[4] = {
    "Updated temperature",
    "Turned off hc-unit",
    "Turned on heating"
    "Turned on cooling",
};


void readInputs(input_t *prev) {
    int result = 0;
    for (int i = 0; i < input_pins_len; i++) {
        result += digitalRead(input_pins[i]) << i;
    }

    if (prev->commands = result) {
        prev->unchanged = true;
    } else {
        prev->commands = result;
        prev->unchanged = false;
    }
    
}

boolean is_expected_input(input_t current, state_input_t state) {
    boolean match = false;
    
    for (size_t i = 0; i < state.inputs_len; i++) {
        if (current.commands == state.valid_inputs[i]) {
            match = true;
        }
    }

    return match;
}
