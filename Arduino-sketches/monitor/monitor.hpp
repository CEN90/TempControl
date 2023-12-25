#include "tinytwin.hpp"

// Error output
constexpr int ERROR_STATE = -1;
constexpr int POLL_TIME = 10;


template <typename T>
Print& operator<<(Print& printer, T value) {
    printer.print(value);
    return printer;
}

void error(int current_state) {
    int s = transitions[current_state][Label];
    
    Serial.println(F("Something went wrong!"));
    Serial.print(F("Previous state -> "));
    Serial.println(current_state + '\n');
    Serial.println(F("Failed to match input to a state, entering following mode instead!\n"));
}

void printInput(input_t input) {
    input.print();
}

void printNextStateOutput(int transition, int state) {
    Serial << "Next state: " << transition << " -> " << labels_string[state];
    Serial << ": " << output_strings[state] << "\n\n";
}

void printPossibleChoices(int current, int start, int len) { 
    Serial << "Current state:" << " " << current << ", possible labels -> ";   

    for (size_t i = 0; i < len; i++) {
        int s = transitions[start + i][Label];
        int v = transitions[start + i][Value];

        Serial << labels_string[s] << " ";

        if (v != ERROR_STATE) {
            Serial << v;
        }
        
        if (i < len -1) {
            Serial << ", ";
        }
    }
    
    Serial.println("");
}

int findState(int searched, int* startpos, int* endpos) {
    *startpos = searched;

    while (transitions[*startpos][From] != searched && *startpos < total_states) {
        *startpos += 1;
    }

    *endpos = *startpos;
    
    while (transitions[*endpos + 1][From] == searched && *endpos < total_states) {
        *endpos += 1;
    }
    
    int len = *endpos - *startpos + 1;
    return len > 0 ? len : ERROR_STATE; // return -1 if error
}