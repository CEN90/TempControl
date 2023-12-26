#include <TMP36.h>
#include <Wire.h>

#define RESOLUTION      100
#define RESOLUTION_F    100.0
#define TEMP_MAIN       PIN_A0
#define TEMP_AUX        PIN_A2
#define ATTACK_BUTTON   13
#define RANDOM_SEED     PIN_A5 // Unused analog port

#define VOLT            5.0

struct temp_t {
  byte main;
  byte aux;
};

boolean changed = false;
boolean isHacked = false;

temp_t temp = { 0, 0 };
int tmp_replace = 0;

TMP36 temp_main(TEMP_MAIN, VOLT); // 5.0 volt sensor
TMP36 temp_aux(TEMP_AUX, VOLT); 

void setup() {
    Serial.begin(115200);
    Wire.begin(2);
    Wire.onRequest(onRequestTemp);
    pinMode(ATTACK_BUTTON, INPUT_PULLUP);
    Serial.println("Reading");
}

void loop() {
    if (!isHacked && digitalRead(ATTACK_BUTTON)) {
        isHacked = true;
        randomSeed(analogRead(RANDOM_SEED));
        tmp_replace = random(-5, 5);
        Serial.println("Oh, no. I too have been hacked!");
    }

    readTemp();
    delay(100);
}

void readTemp(){
    int smooth_main = 0;
    int smooth_aux = 0;

    for (int i = 0; i < RESOLUTION; i++) {
      smooth_main += round(temp_main.getTempC());
      smooth_aux += round(temp_aux.getTempC());
      delay(10);
    }

    temp.main = smooth_main / RESOLUTION;
    temp.aux = smooth_aux / RESOLUTION;

    if (isHacked) {
        temp.main += tmp_replace;
    }
    
}


void onRequestTemp() {
    Wire.write((byte*)&temp, 2);

    Serial.print(temp.main);
    Serial.print(", ");
    Serial.println(temp.aux);
}
