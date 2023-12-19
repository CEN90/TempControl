#include <TMP36.h>
#include <Wire.h>

#define RESOLUTION 10
#define TEMP_MAIN  PIN_A0
#define TEMP_AUX   PIN_A2

struct temp_t {
  byte main;
  byte aux;
};

temp_t temp = { 0, 0 };

TMP36 temp_main(TEMP_MAIN, 5.0); // 5.0 volt sensor
TMP36 temp_aux(TEMP_AUX, 5.0); 

void setup() {
    Wire.begin(2);
    Wire.onRequest(onRequestTemp);
}

void loop() {
    readTemp();
    delay(10);
}

void readTemp(){
    float read_main = 0.0;
    float read_aux = 0.0;

    for (int i = 0; i < RESOLUTION; i++) {
        read_main += temp_main.getTempC();
        read_aux += temp_aux.getTempC();
    }

    temp.main = (int) round(read_main / 10.0);
    temp.aux = (int) round(read_aux / 10.0);
}

void onRequestTemp() {
    Wire.write((byte*)&temp, 2);
}
