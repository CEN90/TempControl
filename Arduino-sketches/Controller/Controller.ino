#include "controller.hpp"

boolean hacked_prev = false;
boolean isHeating = false;
boolean isCooling = false;

void setup() {
    Serial.begin(BAUD);

    Wire.begin();

    setPins();
}

void loop() {
    if (digitalRead(ATTACK_BUTTON) && !hacked_prev) {
        hacked_prev = true;
        hacked();
    }

    // Start with current temp
    current_temp = getTemperature();
    Serial.print("Temp: ");
    Serial.println(current_temp);
    
    setOutput(); // Set output based on temp

    delay(POLL_TIME);
}

// If attack is set then scramble output
void setOutput() {
    boolean isDesiredRange = current_temp >= THRESHOLD_LOW && current_temp <= THRESHOLD_HIGH;
    boolean tempHighHeating = current_temp >= THRESHOLD_HIGH && isHeating;
    boolean tempLowCooling = current_temp <= THRESHOLD_LOW && isCooling;

    boolean tempLow = current_temp <= THRESHOLD_LOW;
    boolean tempHigh = current_temp >= THRESHOLD_HIGH;        

    if (isDesiredRange) 
    {
        if (tempHighHeating || tempLowCooling) {
            switchOffUnit();
            isCooling = false;
            isHeating = false;
        } 
    } 
    else if (tempLow) 
    {
        setHeating();
        isHeating = true;
    } 
    else if (tempHigh) 
    {
        setCooling();
        isCooling = true;
    } 
    


    // if (isDesiredRange && )
    // {
    //     switchOffUnit();
    //     isCooling = false;
    //     isHeating = false;
    // } 
    // else if (current_temp < DESIRED_TEMP)
    // {
    //     setHeating();
    //     isHeating = true;
    // } 
    // else if (current_temp > DESIRED_TEMP)
    // {
    //     setCooling();
    //     isCooling = true;
    // }


    // if (current_temp == DESIRED_TEMP)
    //     switchOffUnit();

    // if (current_temp < DESIRED_TEMP)
    //     setHeating();

    // if (current_temp > DESIRED_TEMP)
    //     setCooling();
}

int getTemperature() {
    int main = 0;
    int aux = 0;

    Wire.requestFrom(SENSOR, SENSOR_DATA_SIZE);

    while (Wire.available()) { // peripheral may send less than requested
        main = Wire.read(); 
        aux = Wire.read(); 
    }    

    // Serial.print("main: ");
    // Serial.print(main);
    // Serial.print(" aux: ");
    // Serial.println(aux);

    if (aux >= SHUTOFF_TEMP) {
        emergencyswitchoff();
    }

    return main;
}
