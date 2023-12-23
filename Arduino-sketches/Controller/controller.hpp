#include <Wire.h>

#define POLL_TIME        1000
#define BAUD             115200

// Heating
#define HEATING_LED      PIN7
#define HEATING_ELEMENT  PIN6
// Cooling
#define COOLING_LED      PIN5
#define COOLING_FAN      PIN4
// Signal shut off
#define SWITCH_OFF_LED   PIN3
// Random seed source
#define SEED             PIN_A4
// Sensor adress
#define SENSOR           2
#define SENSOR_DATA_SIZE 2

#define ATTACKS_LEN      5
#define ATTACK_BUTTON    13

// Temp definitions
#define SHUTOFF_TEMP     35

#define THRESHOLD_LOW    24
#define DESIRED_TEMP     25
#define THRESHOLD_HIGH   26

int current_temp;

enum attacks_t {
    block_heating,
    block_cooling,
    block_shutdown,
    both_heat_cool,
    switch_output,
    none
};

// Description for attacks
String attack_labels[ATTACKS_LEN] = {
    "Blocking signal to heating element.",
    "Blocking signal to cooling fan.",
    "Blocking shutdown mechanism.",
    "Both heater and fan",
    "Switch fan and heater output"
};

attacks_t current_attack = none;

boolean is_current_attack(attacks_t attack) {
    return current_attack == attack;
}

void switchOffUnit();
void setHeating();
void setCooling();

void hacked() {
    Serial.println(F("Oh, no. I've been hacked!"));
    
    randomSeed(analogRead(SEED)); // Seed

    current_attack = (attacks_t) random(0, ATTACKS_LEN);

    // Print out description of current attack
    Serial.println(attack_labels[current_attack]); 
}

// Precaution if the box gets too hot
void emergencyswitchoff() {
    Serial.println(F("Too hot, shutting down!"));
    switchOffUnit();

    while (true) {
        digitalWrite(HEATING_LED, HIGH);
        delay(1000);
        digitalWrite(HEATING_LED, LOW);
        delay(1000);
    }
}

void setPins() {
    pinMode(HEATING_LED, OUTPUT);
    pinMode(HEATING_ELEMENT, OUTPUT);

    pinMode(COOLING_LED, OUTPUT);
    pinMode(COOLING_FAN, OUTPUT);

    pinMode(SWITCH_OFF_LED, OUTPUT);

    pinMode(ATTACK_BUTTON, INPUT_PULLUP);
}

/*
    Relay used is active low, meaning LOW turns on the relay!
*/

void switchOffUnit() {
    digitalWrite(SWITCH_OFF_LED, HIGH);
    digitalWrite(HEATING_LED, LOW);
    digitalWrite(COOLING_LED, LOW);

    // Set both low if not attack
    if (!is_current_attack(block_shutdown)) {
        digitalWrite(HEATING_ELEMENT, HIGH); // active low
        digitalWrite(COOLING_FAN, HIGH); // active low
    }

    // Set both high if attack
    if (is_current_attack(both_heat_cool)) {
        digitalWrite(HEATING_ELEMENT, LOW); // active low
        digitalWrite(COOLING_FAN, LOW); // active low
    }
}

void setHeating() {
    digitalWrite(SWITCH_OFF_LED, LOW);
    digitalWrite(HEATING_LED, HIGH);
    digitalWrite(COOLING_LED, LOW);

    // If not block output attack
    if (!is_current_attack(block_heating)) {
        digitalWrite(HEATING_ELEMENT, LOW); // active low
    }

    digitalWrite(COOLING_FAN, HIGH); // active low    

    // If attack is switch output
    if (is_current_attack(switch_output)) {
        digitalWrite(COOLING_FAN, LOW); // active low    
        digitalWrite(HEATING_ELEMENT, HIGH); // active low
    }
    
}

void setCooling() {
    digitalWrite(SWITCH_OFF_LED, LOW);
    digitalWrite(HEATING_LED, LOW);
    digitalWrite(COOLING_LED, HIGH);

    // If not block output attack
    if (!is_current_attack(block_cooling)) {
        digitalWrite(COOLING_FAN, LOW); // active low
    }

    digitalWrite(HEATING_ELEMENT, HIGH); // active low

    // If attack is switch output
    if (is_current_attack(switch_output)) {
        digitalWrite(COOLING_FAN, HIGH); // active low    
        digitalWrite(HEATING_ELEMENT, LOW); // active low
    }
}