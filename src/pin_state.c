#include "pin_state.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// グローバル変数の実体定義
PinState pin_states[MAX_PIN_NUM];
int end_sim_flag;
int pin_count;
int current_time_ms;

/* シミュレーション時間を初期化する関数 */
void initTimems(void) {
    current_time_ms = 0;
}   

/* シミュレーション終了フラグを初期化する関数 */
void initFlag(void) {
    end_sim_flag = 0;
}

/* ピン状態配列を初期化する関数 */
void initPinStates(void) {
    pin_count = 0;
    for (int i = 0; i < MAX_PIN_NUM; i++) {
        pin_states[i].number = -1; /* 未使用ピン */
        pin_states[i].mode = -1;   /* 未設定モード */
        pin_states[i].state_count = -1;
        for (int j = 0; j < MAX_STATE_NUM; j++) {
            pin_states[i].log[j].value = 0.0;
            pin_states[i].log[j].duration = 0;
        }
    }
}

void init() {
    initPinStates();
    initTimems();
    initFlag();
}

void updateSimulationTime(int ms) {
    current_time_ms += ms;
}

int getSimulationTime(void) {
    return current_time_ms;
}

void loadInputPinsStateFromFile(const char* filename) {
    char* filepath = malloc(strlen(PARAM_DIR) + strlen(filename) + 1);
    strcpy(filepath, PARAM_DIR);
    strcat(filepath, filename);
    printf("Loading input pin states from file: %s\n", filepath);
    FILE* file = fopen(filepath, "r");
    char line[LINE_BUFFER_SIZE];
    int line_count = 0;
    if (file == NULL) raiseFileOpenError(filename);
    while (fgets(line, sizeof(line), file) != NULL) {
        int pin;
        float value;
        int duration;
        if (sscanf(line, "%d", &pin) != 1) raiseLoadInputFileError(filename);
        int idx = getIndexOfPin(pin);
        if (idx == -1) raiseNoPinError(pin);
        PinState* state = &pin_states[idx];
        if (state->mode != INPUT) raiseInvalidFuncError(pin, state->mode);
        char c;
        int global_idx = 0;
        int local_idx = 0;
        char buf[PARAM_BUFFER_SIZE];
        int space_count = 0;
        while (global_idx < sizeof(line)) {
            char ch = line[global_idx];
            // puts("-----");
            // printf("ch: '%c' (global_idx=%d, local_idx=%d)\n", ch, global_idx, local_idx);
            if(ch == ' ' || ch == '\n' || ch == '\0') {
                global_idx++;
                if(local_idx == 0) continue;
                if(space_count == 0){
                    int tmp;
                    local_idx=0; // ピン番号はsscanfで読み取り済みなので読み飛ばし
                    memset(buf, 0, sizeof(buf));
                    space_count++;
                }
                else if(space_count % 2){
                    sscanf(buf, "%f", &value);
                    local_idx = 0;
                    memset(buf, 0, sizeof(buf));
                    space_count++;
                }
                else {
                    sscanf(buf, "%d", &duration);
                    int sc = ++state->state_count;

                    if (sc >= MAX_STATE_NUM) break;
                   
                    state->log[sc].value = value;
                    state->log[sc].duration = duration;
                    
                    local_idx = 0;
                    memset(buf, 0, sizeof(buf));
                    space_count++;
                }

                if (ch == '\n' || ch == '\0') break;

            }
            buf[local_idx++] = line[global_idx++];

        } 
    }
}

void registerPin(int pin, int mode) {

    // printf("=================================\n");
    // printf("registerPin: pin=%d, mode=%d\n", pin, mode);

    if(pin <= AVAILABLE_PIN_NUMBER_MIN || pin >= AVAILABLE_PIN_NUMBER_MAX) 
        raiseInvalidPinNumberError(pin);

    if(mode != INPUT && mode != OUTPUT) 
        raiseInvalidModeError(pin, mode);

    PinState* state = &pin_states[pin_count];
    state->number = pin;
    state->mode = mode;
    pin_count++;

    return;
}

void updateDurationForOutputPins(int ms) {
    for (int i = 0; i < pin_count; i++) { 
        int mode = pin_states[i].mode;
        if (mode != OUTPUT) continue;
        PinState* state = &pin_states[i];
        int sc = state->state_count;
        
        // 状態が1つ以上ある場合のみ更新
        if (sc < 0) continue;
        
        // 配列範囲外アクセスを防ぐ
        if (sc >= MAX_STATE_NUM) {
            end_sim_flag = 1;
            continue;
        }

        // 最後に追加された状態（現在の状態）を更新
        State* current_state = &state->log[sc];
        current_state->duration += ms;

        // printf("afterUpdate - value=%.1f, duration=%d\n", current_state->value, current_state->duration);
    }
    return;
}

void updatePinState(int pin, double value) {
    
    int idx = getIndexOfPin(pin);
    if (idx == -1) raiseNoPinError(pin);
    PinState* state = &pin_states[idx];

    int sc = ++state->state_count;

    // 管理可能な状態数に達したらシミュレーション終了
    if (sc >= MAX_STATE_NUM) {
        end_sim_flag = 1;
        return;  // 配列範囲外アクセスを防ぐため、ここで終了
    }

    int mode = state->mode;
    if (mode != OUTPUT) raiseInvalidFuncError(pin, mode);

    state->log[sc].value = value;
    
    return;
}


int getIndexOfPin(int pin) {
    // printf("getIndexOfPin: pin=%d\n", pin);
    for (int i = 0; i < pin_count; i++) {
        // printf("  checking pin_states[%d].number=%d\n", i, pin_states[i].number);
        if (pin_states[i].number == pin) {
            return i;
        }
    }
    return -1; // ピンが見つからない場合
}

double getPinState(int pin) {
    int idx = getIndexOfPin(pin);
    if (idx == -1) raiseNoPinError(pin);
    int current_time_ms = getSimulationTime();
    int time_sum = 0;
    PinState* state = &pin_states[idx];
    for(int sc=0; sc <= state->state_count; sc++) {
        if(current_time_ms > time_sum + state->log[sc].duration) {
            time_sum += state->log[sc].duration;
            continue;
        }
        return state->log[sc].value;
    }
    return LOW; // デフォルト値
}

