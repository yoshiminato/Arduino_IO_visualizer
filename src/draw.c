#include "raylib.h"
#include "draw.h"
#include "pin_state.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


#define MAX_SAMPLES 780 // 描画する波形のサンプル数

WaveQueue dataBuffer[MAX_PIN_NUM]; // データを溜める配列

// 描画関数
void* draw(void* arg) {

    InitWindow(800, pin_count * BOX_HEIGHT, "waveform");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
       
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int p = 0; p < pin_count; p++) {

            // ピン番号を描画
            DrawText(
                TextFormat("Pin %d", pin_states[p].number), 
                MARGIN_X, 
                p * BOX_HEIGHT + TEXT_MARGIN_Y, 
                FONT_SIZE, 
                DARKGRAY
            );
            // 波形を描画
            for (int i = 0; i < MAX_SAMPLES - 1; i++) {
                float data1 = WaveQueue_get(&dataBuffer[p], i);
                float data2 = WaveQueue_get(&dataBuffer[p], i + 1);
                DrawLine(
                    MARGIN_X + i, 
                    p * BOX_HEIGHT + (BOX_HEIGHT - WAVE_OFFSET) - (int)data1 * BOLT_PER_PIXEL, 
                    MARGIN_X + i + 1, 
                    p * BOX_HEIGHT + (BOX_HEIGHT - WAVE_OFFSET) - (int)data2 * BOLT_PER_PIXEL, BLUE
                );
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

// ピン状態を描画用バッファに配信する関数
void publishPinStatesToDraw(void) {
    for (int p = 0; p < pin_count; p++) {
        PinState* state = &pin_states[p];
        int sc = state->state_count;
        if (sc >= MAX_SAMPLES) sc = MAX_SAMPLES - 1; // バッファサイズを超えないようにする
        Enqueue(&dataBuffer[p], state->log[sc].value);
    }
}

// ピン状態を描画用バッファに配信するスレッド関数
void* publishThreadFunction(void* arg) {
    while (!end_sim_flag) {
        publishPinStatesToDraw();
        myDelay(PUBLISH_INTERVAL_US);
    }
    return NULL;
}