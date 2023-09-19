// Pin definitions
#define HEATING_IN        8
#define COOLING_IN        9
#define SWITCH_OFF_IN     10

#define HEATING_OUT       5
#define COOLING_OUT       6
#define SWITCH_OFF_OUT    7

#define POLL_TIME         1000

void setup() {
    pinMode(HEATING_IN, INPUT);
    pinMode(COOLING_IN, INPUT);
    pinMode(SWITCH_OFF_IN, INPUT);

    pinMode(HEATING_OUT, OUTPUT);
    pinMode(COOLING_OUT, OUTPUT);
    pinMode(SWITCH_OFF_OUT, OUTPUT);

    // pinMode(LED_BUILTIN, OUTPUT);

    // pinMode(HEATING_OUT, OUTPUT);
    // pinMode(COOLING_OUT, OUTPUT);
    // pinMode(SWITCH_OFF_OUT, OUTPUT);

}

void loop() {
    digitalWrite(HEATING_OUT, digitalRead(HEATING_IN));
    digitalWrite(COOLING_OUT, digitalRead(COOLING_IN));
    digitalWrite(SWITCH_OFF_OUT, digitalRead(SWITCH_OFF_IN));

    // digitalWrite(COOLING_OUT, HIGH);
    // digitalWrite(HEATING_OUT, HIGH);
    // digitalWrite(SWITCH_OFF_OUT, HIGH);

    // delay(POLL_TIME);

    // digitalWrite(COOLING_OUT, LOW);
    // digitalWrite(HEATING_OUT, LOW);
    // digitalWrite(SWITCH_OFF_OUT, LOW);

    delay(POLL_TIME);

}


// struct {
//   int sensedTemp;
//   bool heating;
//   bool cooling;
// } controller;

// struct {
//   int temperature;
//   int outside_air;
// } room;

// struct {
//   bool heater_on;
//   bool cooler_on;
//   bool switched_off;
// } hc_unit;

// struct {
//   struct controller Controller;
//   struct room Room;
//   struct hc_unit Hc_unit;
// } state;

// struct {
//   int source;
//   int destination;
// } transitions;