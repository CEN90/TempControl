#include <TMP36.h>

#define ATTACK           0

#define ADC_RESOLUTION   1024.0
#define POLL_TIME        1000

// Pin definitions
#define TEMP_IN          PIN_A0
#define HEATING_PORT     8
#define COOLING_PORT     9
#define SWITCH_OFF_PORT  10

#define SMOOTHING        10

#define DESIRED_TEMP     22
float current_temp;

void setOutput(int attack);
float readSensor(int sensor);
float smoothInput(int sensor);
float getTemperature(int sensor, int attack);
void switchOffUnit();
void setHeating();
void setCooling();


void setup() {
    Serial.begin(115200);

    pinMode(TEMP_IN, INPUT);
    pinMode(HEATING_PORT, OUTPUT);
    pinMode(COOLING_PORT, OUTPUT);
    pinMode(SWITCH_OFF_PORT, OUTPUT);

    // Attack part
    if (ATTACK) {
        randomSeed(analogRead(PIN_A2));
    }
}

void loop() {
    // Start with current temp
    current_temp = getTemperature(TEMP_IN, ATTACK);
    Serial.print("Temp: ");
    Serial.println(current_temp);

    setOutput(ATTACK); // Defer to separate function

    delay(POLL_TIME);
}

// If attack is set then scramble output
void setOutput(int attack) {
    if (current_temp == DESIRED_TEMP)
        switchOffUnit();

    if (current_temp < DESIRED_TEMP)
        setHeating();

    if (current_temp > DESIRED_TEMP)
        setCooling();
}

float readSensor(int sensor) {
    // convert the ADC value to voltage in millivolt
    float voltage = 5.0 * (analogRead(sensor) / ADC_RESOLUTION);

    // convert the voltage to the temperature in Celsius
    return (voltage - 0.5) * 100.0;
}

// Reading is erratic, smooth it out over multiple reads
float smoothInput(int sensor) {
    float total = 0;

    // Add all reads then take the average from them
    for (size_t i = 0; i < SMOOTHING; i++) {
        total += readSensor(sensor);
        delay(1);
    }

    return total / SMOOTHING;
}

// Return random temp if attacked, otherwise the temp
float getTemperature(int sensor, int attack) {
    if (attack) {
        return random(50);
    }
    
    return smoothInput(sensor);
}

void switchOffUnit() {
    digitalWrite(SWITCH_OFF_PORT, HIGH);
    digitalWrite(HEATING_PORT, LOW);
    digitalWrite(COOLING_PORT, LOW);
}

void setHeating() {
    digitalWrite(SWITCH_OFF_PORT, LOW);
    digitalWrite(HEATING_PORT, HIGH);
    digitalWrite(COOLING_PORT, LOW);
}

void setCooling() {
    digitalWrite(SWITCH_OFF_PORT, LOW);
    digitalWrite(HEATING_PORT, LOW);
    digitalWrite(COOLING_PORT, HIGH);
}