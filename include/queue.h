#include <pthread.h>
#include <stdbool.h>

#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 780

// キューの構造体
typedef struct {
    float data[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex; // 構造体の中に鍵を持たせる
} WaveQueue;

// キューの初期化
void InitQueue(WaveQueue *q);

// データを追加
bool Enqueue(WaveQueue *q, float value);

// データを取り出す
bool Dequeue(WaveQueue *q, float *out_value);

// キューから特定のインデックスの値を取得
float WaveQueue_get(WaveQueue *q, int index);

#endif /* QUEUE_H */