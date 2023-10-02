// Pin definitions
#define MAIN_SENSOR_IN    A4
#define AUX_SENSOR_IN     A8
#define HEATING_IN        8
#define COOLING_IN        9
#define SWITCH_OFF_IN     10

#define POLL_TIME         100
#define MUL_TIME          1000
#define STATES_LEN        6
#define ADC_RESOLUTION    1024.0

#define SMOOTHING         100

const int setcurrent = 1;
const int time = 2;
const int heating = 3;
const int cooling = 4;
const int switchoff = 5;

String names[] = {
        "other",
        "controller.setcurrent",
        "time",
        "hc_unit.setheating",
        "hc_unit.setcooling",
        "hc_unit.switchoff"
};

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

void setPins() {
    pinMode(HEATING_IN, INPUT);
    pinMode(COOLING_IN, INPUT);
    pinMode(SWITCH_OFF_IN, INPUT);
    pinMode(MAIN_SENSOR_IN, INPUT);
    pinMode(AUX_SENSOR_IN, INPUT);
}

void giveUp(String msg) {
    Serial.println(msg);
    delay(POLL_TIME);
    exit(EXIT_FAILURE);
}