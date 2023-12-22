#include <TMP36.h>
#include <Wire.h>

#define RESOLUTION 100
#define TEMP_MAIN  PIN_A0
#define TEMP_AUX   PIN_A2

#define VOLT       5.0

struct temp_t {
  byte main;
  byte aux;
};

boolean changed = false;

temp_t temp = { 0, 0 };

TMP36 temp_main(TEMP_MAIN, VOLT); // 5.0 volt sensor
TMP36 temp_aux(TEMP_AUX, VOLT); 

void setup() {
    Serial.begin(115200);
    Wire.begin(2);
    Wire.onRequest(onRequestTemp);
}

void loop() {
    readTemp();
    delay(100);
}

void readTemp(){
    float read_main = 0.0;
    float read_aux = 0.0;

    for (int i = 0; i < RESOLUTION; i++) {
        read_main += temp_main.getTempC();
        read_aux += temp_aux.getTempC();
        delay(10);
    }

    float t = (int) round(read_main / 100.0);

    if (t == temp.main) {
        changed = false;
    } else {
        changed = true;
    }
    
    temp.main = t;
    temp.aux = (int) round(read_aux / 100.0);
}

void onRequestTemp() {
    Wire.write((byte*)&temp, 2);

    if (changed) {
        Serial.print("Main: ");
        Serial.print(temp.main);
        Serial.print(" aux: ");
        Serial.println(temp.aux);
    }
}
