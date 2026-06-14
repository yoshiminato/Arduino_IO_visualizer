#include "pin_state.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* グローバル変数の実体定義 */
PinState pin_states[MAX_PIN_NUM];    // ピン状態配列
int end_sim_flag;                    // シミュレーション終了フラグ
int pin_count;                       // 登録されたピンの数                      
long long simulation_start_time_us;  // シミュレーションの開始時刻
long long end_simulation_time_us;    // シミュレーションの終了時刻（実行時に指定）


/* 現在の時刻をマイクロ秒単位で取得する関数 */
long long getCurrentTimeus(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

/* シミュレーション時間（マイクロ秒単位）を取得する関数 */
long long getSimulationTimeus(void) {
    long long current_time_us = getCurrentTimeus();
    return current_time_us - simulation_start_time_us;
}

/* シミュレーション時間を初期化する関数 */
void initTimeus(void) {
    simulation_start_time_us = getCurrentTimeus();
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
        pin_states[i].state_count = 0;
        for (int j = 0; j < MAX_STATE_NUM; j++) {
            pin_states[i].log[j].value = 0.0;
            pin_states[i].log[j].duration = 0;
        }
    }
}

/* 状態の初期化関数 */
void init() {
    initPinStates();
    initTimeus();
    initFlag();
}

/* delay関数 */
void myDelay(int us) {
    usleep(us);
}

/* パラメータファイルから入力ピンの状態を読み込む関数 */ //現状未使用
void loadInputPinsStateFromFile(const char* filename) {

    // ファイルパスの構築
    char* filepath = malloc(strlen(PARAM_DIR) + strlen(filename) + 1);
    strcpy(filepath, PARAM_DIR);
    strcat(filepath, filename);

    // ファイルが存在しなければエラーを発生
    FILE* file = fopen(filepath, "r");
    if (file == NULL) raiseFileOpenError(filename);

    // 入力ファイル読み込みのためのバッファ
    char line[LINE_BUFFER_SIZE]; 

    // 1行ずつ入力ファイルを読み込む
    while (fgets(line, sizeof(line), file) != NULL) {

        int pin;
        float value;
        long long duration;

        // 数値(ピン番号)が読み取れなければ異常終了
        if (sscanf(line, "%d", &pin) != 1) raiseLoadInputFileError(filename);
        
        // 無効なピン番号、モードの場合のガード節
        int idx = getIndexOfPin(pin);
        if (idx == -1) raiseNoPinError(pin);
        PinState* state = &pin_states[idx];
        if (state->mode != INPUT) raiseInvalidFuncError(pin, state->mode);

        int global_idx = 0;           // グローバルインデックス(行の中で何文字目か)
        int local_idx = 0;            // ローカルインデックス(解析中の単語の文字数)
        char buf[PARAM_BUFFER_SIZE];  // 解析中の単語バッファ
        int space_count = 0;          // スペース区切りの単語数
        
        // 行を1文字づつ解析
        while (global_idx < sizeof(line)) {

            // 現在の文字を取得
            char ch = line[global_idx];

            // スペース、改行、終端文字の場合(データが途切れた場合)の処理
            if(ch == ' ' || ch == '\n' || ch == '\0') {
                

                global_idx++;
                if(local_idx == 0) continue;

                // 1つ目のデータは読み飛ばす(既にループ前にsscanfで読み取り済み)
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
                    sscanf(buf, "%lld", &duration);
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


/* ピンを登録する関数 */
void registerPin(int pin, int mode) {

    // 無効なピン番号、モード指定の場合のガード節
    if(pin <= AVAILABLE_PIN_NUMBER_MIN || pin >= AVAILABLE_PIN_NUMBER_MAX) 
        raiseInvalidPinNumberError(pin);
    if(mode != INPUT && mode != OUTPUT) 
        raiseInvalidModeError(pin, mode);

    // ピン状態配列に新しいピンを登録
    PinState* state = &pin_states[pin_count];
    state->number = pin;
    state->mode = mode;
    pin_count++;

    return;
}

/* 指定ピンの状態を更新する関数 */
void updatePinState(int pin, double value) {
    
    // 存在しないピンが指定された場合のガード節
    int idx = getIndexOfPin(pin);
    if (idx == -1) raiseNoPinError(pin);

    PinState* state = &pin_states[idx];

    // 今の状態と同じなら状態更新しない
    double current_value = state->log[state->state_count].value;
    if(current_value == value) return;

    int sc = ++state->state_count;

    // 管理可能な状態数に達したらシミュレーション終了
    if (sc >= MAX_STATE_NUM) 
        raiseStateOverflowError(pin);

    // 前の状態の終了時刻(遷移してきた全状態の経過時間の総和)を計算
    long long last_state_start_time_us = 0;
    for (int i = 0; i < sc; i++)
        last_state_start_time_us += state->log[i].duration;

    if(sc > 0){
        // 時刻を取得し前状態の経過時間を算出 & 記録
        int prev_sc = sc - 1;
        long long current_simulation_time_us = getSimulationTimeus();
        state->log[prev_sc].duration = current_simulation_time_us - last_state_start_time_us;
    }

    // 状態を更新
    state->log[sc].value = value;

    return;
}

int getIndexOfPin(int pin) {
    for (int i = 0; i < pin_count; i++)
        if (pin_states[i].number == pin)
            return i;
            
    return -1; // ピンが見つからない場合
}

/* 指定ピンの最新の状態を取得する関数 */
double getPinState(int pin) {

    // 存在しないピンが指定された場合のガード節
    int idx = getIndexOfPin(pin);
    if (idx == -1) raiseNoPinError(pin);

    // 現シミュレーション時間を取得
    long long current_simulation_time_us = getSimulationTimeus();

    // 入力ピンの状態配列から現在の状態を取得
    int timeus_sum = 0;
    PinState* state = &pin_states[idx];
    // for(int sc=0; sc <= state->state_count; sc++) {
    //     if(current_simulation_time_us > timeus_sum + state->log[sc].duration) {
    //         timeus_sum += state->log[sc].duration;
    //         continue;
    //     }


    //     return state->log[sc].value;
    // }

    int sc = state->state_count;
    float value = state->log[sc].value;
    return value;   

}

/* シミュレーション終了時に残った状態を補完する関数 */
void cleanupSimulation(void) {
    for(int i=0; i < pin_count; i++) {
        PinState* state = &pin_states[i];
        int pin = state->number;
        int sc = state->state_count;

        int mode = state->mode;
        if (mode != OUTPUT) continue;

        // 前の状態の終了時刻(遷移してきた全状態の経過時間の総和)を計算
        long long prev_state_start_time_us = 0;
        for (int j = 0; j < sc; j++)
            prev_state_start_time_us += state->log[j].duration;

        long long rest_time_us = end_simulation_time_us - prev_state_start_time_us;
        state->log[sc].duration = rest_time_us;
    }
    return;
}

