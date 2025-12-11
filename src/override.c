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
    // return 0; // デフォルト値
}


float analogRead(int pin) {
    /* 仮実装 */
    return 0.0f; // デフォルト値
}

float millis() {
    // 仮実装: 常に0を返す
    return 0.0f;
}

void delay(int ms) {
    // シミュレーション時間と出力ピン状態の更新を行う
    int dt = 1000 * ms; // マイクロ秒に変換
    updateSimulationTime(dt);
    updateDurationForOutputPins(dt);
}

void delayMicroseconds(int us) {
    // シミュレーション時間と出力ピン状態の更新を行う
    updateSimulationTime(us);
    updateDurationForOutputPins(us);
}

