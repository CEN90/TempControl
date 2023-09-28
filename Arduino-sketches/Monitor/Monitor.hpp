// Pin definitions
#define MAIN_SENSOR_IN    A0
#define AUX_SENSOR_IN     A1
#define HEATING_IN        8
#define COOLING_IN        9
#define SWITCH_OFF_IN     10

#define POLL_TIME         1000
#define ADC_RESOLUTION    1024.0
#define POLL_TIME         1000
#define STATES_LEN        6

String wait = "time";
String names[] = {
        "other",
        "controller.setcurrent",
        "time",
        "hc_unit.setheating",
        "hc_unit.setcooling",
        "hc_unit.switchoff"
};

float getTemperature(int sensor) {
    // convert the ADC value to voltage in millivolt
    float voltage = 5.0 * (analogRead(sensor) / ADC_RESOLUTION);

    // convert the voltage to the temperature in Celsius
    return (voltage - 0.5) * 100;
}

void setPins() {
    pinMode(HEATING_IN, INPUT);
    pinMode(COOLING_IN, INPUT);
    pinMode(SWITCH_OFF_IN, INPUT);
    pinMode(MAIN_SENSOR_IN, INPUT);
    pinMode(AUX_SENSOR_IN, INPUT);
}