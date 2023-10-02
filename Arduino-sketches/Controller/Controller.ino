#define ADC_RESOLUTION   1024.0
#define POLL_TIME        1000

// Pin definitions
#define TEMP_IN          A0
#define HEATING_PORT     8
#define COOLING_PORT     9
#define SWITCH_OFF_PORT  10

#define SMOOTHING         100

#define DESIRED_TEMP     22
int current_temp;

void setup() {
    Serial.begin(115200);

    pinMode(TEMP_IN, INPUT);
    pinMode(HEATING_PORT, OUTPUT);
    pinMode(COOLING_PORT, OUTPUT);
    pinMode(SWITCH_OFF_PORT, OUTPUT);
}

void loop() {
    // Start with current temp
    current_temp = getTemperature(TEMP_IN);
    Serial.print("Temp: ");
    Serial.println(current_temp);

    if (current_temp == DESIRED_TEMP)
        switchOffUnit();

    if (current_temp < DESIRED_TEMP)
        setHeating();

    if (current_temp > DESIRED_TEMP)
        setCooling();

    delay(POLL_TIME);
}

float readSensor(int sensor) {
    // convert the ADC value to voltage in millivolt
    float voltage = 5.0 * (analogRead(sensor) / ADC_RESOLUTION);

    // convert the voltage to the temperature in Celsius
    return (voltage - 0.5) * 100;
}

int smoothInput(int sensor) {
    int readings[SMOOTHING];
    int total = 0;

    for (size_t i = 0; i < SMOOTHING; i++)
    {
        readings[i] = readSensor(sensor);
        total += readings[i];
        delay(1);
    }

    return total / SMOOTHING;
}

int getTemperature(int sensor) {
    return smoothInput(sensor);
}

// float getTemperature() {
//     // convert the ADC value to voltage in millivolt
//     float voltage = 5.0 * (analogRead(TEMP_IN) / ADC_RESOLUTION);

//     // convert the voltage to the temperature in Celsius
//     return (voltage - 0.5) * 100;
// }

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