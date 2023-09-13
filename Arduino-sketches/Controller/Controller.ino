#define TEMP_IN          A0
#define HEATING_PORT     7
#define COOLING_PORT     7
#define SWITCH_OFF_PORT  7
#define ADC_RESOLUTION   1024.0

#define DESIRED_TEMP     21

int current_temp;

void setup() {
    Serial.begin(115200);

    pinMode(TEMP_IN, INPUT);
    pinMode(HEATING_PORT, OUTPUT);
    pinMode(COOLING_PORT, OUTPUT);
    pinMode(SWITCH_OFF_PORT, OUTPUT);
}

void loop() {
    // convert the ADC value to voltage in millivolt
    float voltage = 5.0 * (analogRead(TEMP_IN) / ADC_RESOLUTION);

    // convert the voltage to the temperature in Celsius
    float tempC = (voltage - 0.5) * 100;

    Serial.println((int)tempC);
    delay(100);
}