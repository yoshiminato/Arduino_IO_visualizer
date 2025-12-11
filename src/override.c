#include "override.h"
#include "pin_state.h"
#include "error.h"
#include <stdio.h>

void pinMode(int pin, int mode) {
    registerPin(pin, mode);
}

void digitalWrite(int pin, int value) {
    updatePinState(pin, (double)value);
}

void analogWrite(int pin, int value) {
    updatePinState(pin, (double)value);
}

int digitalRead(int pin) {
    double value = getPinState(pin);
    return (int)value;
}


float analogRead(int pin) {
    return 0.0f; // デフォルト値
}

unsigned long millis() {
    long long current_time_us = getSimulationTimeus();
    return (unsigned long)(current_time_us / 1000);
}

unsigned long micros() {
    return (unsigned long)getSimulationTimeus();
}

void delay(int ms) {
    int dt = 1000 * ms; // マイクロ秒に変換
    myDelay(dt);
}

void delayMicroseconds(int us) {
    myDelay(us);
}

