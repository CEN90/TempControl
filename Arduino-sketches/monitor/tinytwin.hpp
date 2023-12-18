#include "states.hpp"
#include <TMP36.h>

// Input Arduino Uno
constexpr int TEMP_MAIN = PIN_A0;
constexpr int TEMP_AUX = PIN_A1;
constexpr int HEATING = PIN3;
constexpr int COOLING = PIN2;

// Input Arduino Mega pinout
// constexpr int TEMP_MAIN = PIN4;
// constexpr int TEMP_AUX = PIN4;
// constexpr int HEATING = PIN3;
// constexpr int COOLING = PIN2;

constexpr int input_pins_len = 4;
constexpr int input_pins[input_pins_len] = { TEMP_MAIN, TEMP_AUX, HEATING, COOLING };

TMP36 temp_main(TEMP_MAIN, 5.0); // 5.0 volt sensor
TMP36 temp_aux(TEMP_AUX, 5.0); 

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
}

void readInputs(input_t *prev) {
    int result = 0;
    for (int i = 0; i < input_pins_len; i++) {
        result += digitalRead(input_pins[i]) << i;
    }

    // Lägg till temp-läsning här!

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

    // Lägg till matchning av temp_main & temp_aux här!

    return match;
}
