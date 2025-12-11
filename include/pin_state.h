#ifndef PIN_STATE_H
#define PIN_STATE_H

#define square(x) ((x)*(x))
#define MAX_STATE_NUM 40//square(100)
#define MAX_PIN_NUM 20

/* arduinoの定数定義 */
#define HIGH 1
#define LOW  0
#define INPUT 1
#define INPUT_PULLUP 1
#define OUTPUT 0

#define AVAILABLE_PIN_NUMBER_MIN 1
#define AVAILABLE_PIN_NUMBER_MAX 19

#define TIMESTEP 100
#define LINE_BUFFER_SIZE 2048
#define PARAM_DIR "./params/"
#define MAX_PATH_LENGTH 256
#define PARAM_BUFFER_SIZE 16

/* 状態の構造体 */
typedef struct {
    float value;
    int duration; //ms
} State;

/* ピン状態の構造体 */
typedef struct {
    int number; /* ピン番号 */
    int mode;   /* ピンモード(input or output) */
    int state_count; /* 保存中の状態数 */
    State log[MAX_STATE_NUM];
} PinState;

/* 状態初期化関数*/
void init();

/* シミュレーション時間を更新する関数 */
void updateSimulationTime(int ms);

/* シミュレーション時間を取得する関数 */
int getSimulationTime(void);

/* パラメータファイルから入力ピンの状態を読み込む関数 */
void loadInputPinsStateFromFile(const char* filename);

/* ピンを登録する関数 */
void registerPin(int pin, int mode);

/* 全ピンのdurationを更新する関数 */
void updateDurationForOutputPins(int ms);

/* 指定ピンの状態を更新する関数 */
void updatePinState(int pin, double value);

/* ピン状態配列におけるピンのインデックスを取得する関数 */
int getIndexOfPin(int pin);

/* 指定ピンの最新の状態を取得する関数 */
double getPinState(int pin);

// ピン状態配列
extern PinState pin_states[MAX_PIN_NUM];

// 登録されたピンの数
extern int pin_count;

extern int end_sim_flag;

extern int current_time_ms;

#endif /* PIN_STATE_H */