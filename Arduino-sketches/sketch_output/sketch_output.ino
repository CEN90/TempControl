int switch_off = 8;

void setup() {
  pinMode(output, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(output, HIGH); 
  delay(100);                      
  digitalWrite(output, LOW);   
  delay(100);                      
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