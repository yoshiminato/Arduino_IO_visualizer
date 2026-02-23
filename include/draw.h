#include "pin_state.h"
#include "queue.h"

#ifndef DRAW_H
#define DRAW_H

#define PUBLISH_INTERVAL_US 10000 // 描画更新の間隔（マイクロ秒）
#define MARGIN_X 10
#define WAVE_OFFSET 20
#define BOX_HEIGHT 100
#define BOLT_PER_PIXEL 40
#define TEXT_MARGIN_Y 10
#define FONT_SIZE 20


/* 描画に用いるデータのバッファ */
extern WaveQueue dataBuffer[MAX_PIN_NUM];

/* 描画スレッドの関数 */
void* draw(void* arg);

/* ピン状態を配信する関数 */
void publishPinStatesToDraw(void);

/* ピン状態を配信するスレッドの関数 */
void* publishThreadFunction(void* arg);

#endif /* DRAW_H */