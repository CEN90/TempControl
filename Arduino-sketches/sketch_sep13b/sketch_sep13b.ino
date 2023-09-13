int input = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(input, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, digitalRead(input));
  delay(100);
}
