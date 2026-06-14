#include "override.h"
#include "pin_state.h"
#include "error.h"
#include <stdio.h>

/* ピンモードの設定関数 */
void pinMode(int pin, int mode) {
    registerPin(pin, mode);
}

/* 出力ピンの出力値設定関数 */
void digitalWrite(int pin, int value) {
    updatePinState(pin, (double)value);
}
void analogWrite(int pin, int value) {
    updatePinState(pin, (double)value);
}

/* 入力ピンの状態確認のための関数 */
int digitalRead(int pin) {
    double value = getPinState(pin);
    return (int)value;
}
float analogRead(int pin) {
    return 0.0f;
}

/* 時間計測関数 */
unsigned long millis() {
    long long current_time_us = getSimulationTimeus();
    return (unsigned long)(current_time_us / 1000);
}
unsigned long micros() {
    return (unsigned long)getSimulationTimeus();
}


/* delay関数 */
void delay(int ms) {
    int dt = 1000 * ms;
    myDelay(dt);
}
void delayMicroseconds(int us) {
    myDelay(us);
}

