#include "monitor.hpp"
#define BAUD  115200


/* Not used for this tiny twin */
boolean timer_on = false;
unsigned long timer = 0;

// If error
boolean follow_mode = false;
boolean tau = false;

int read_inputs = 0;
int prev_inputs = 0;
input_t input = { 0, 0, 0, true };

int current_state = 0;
int next_state = 0;

// State id != index in transition[], keep track of index separately
int possible_states_len, startpos, endpos;

void setup() {
    Serial.begin(BAUD);

    setPins();

    next_state = start_state;

    Serial.println(F("Ready to start monitor now"));
}

void loop() {   
    readInputs(&input);

    // If timer is turned on and has run out, do something... later
    if (timer_on && timer < millis()) {
        Serial.println(F("Time ran out"));
    }

    // Do nothing if pin states is same
    if (input.unchanged && !tau) {
        delay(POLL_TIME);
        return;
    }
    
    // If error then just print out input for logging purposes
    if (next_state == ERROR_STATE) {
        follow();
        delay(POLL_TIME);
        prev_inputs = read_inputs;
        return;
    }

    current_state = next_state;
    possible_states_len = findState(current_state, &startpos, &endpos);

    // If no new state could be found then go to ERROR_STATE
    if (possible_states_len == ERROR_STATE) {
        next_state = ERROR_STATE;
        return;
    }

    printPossibleChoices(current_state, startpos, possible_states_len);

    // Actual comparison here!
    next_state = compare(startpos, possible_states_len);
    
    prev_inputs = read_inputs;
    delay(POLL_TIME);
}

// Match input with possible transitions in current state
int compare(int start, int len) {
    printInput(input);

    // For every possible transition in state
    for (size_t i = 0; i < len; i++)
    {
        int state_label = transitions[start + i][Label];
        int transition_to = transitions[start + i][To];
        int inputs_len = expected_inputs[state_label].inputs_len;
        

        // If no inputs given then tell main loop to do one more iteration
        if (inputs_len == 0) {
            tau = true;
            Serial.println(F("Encountered a tau transition"));
        } else {
            tau = false;
        }

        // Compare the states possible inputs
        if (is_expected_input(input, expected_inputs[state_label], start + i)) {
            printNextStateOutput(transition_to, state_label);
            return transition_to;
        }
    }

    // If can't match to a transition then enter follow mode instead
    tau = false;
    error(startpos);
    return ERROR_STATE;
}

// Follow mode, print input to Serial
void follow() {
    Serial.print("Input recorded: ");
    printInput(input);
    Serial.println("");
}