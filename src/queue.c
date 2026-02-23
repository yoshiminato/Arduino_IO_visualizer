#include "queue.h"

// キューの初期化
void InitQueue(WaveQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
}

// データを追加（スレッドセーフ）
bool Enqueue(WaveQueue *q, float value) {
    pthread_mutex_lock(&q->mutex);

    if (q->count >= QUEUE_SIZE) {
        // ロックを持ったままDequeueのロジックを実行（関数呼び出しだと二重ロックになるため展開するか、ロックなし版を用意する）
        // ここでは簡単に、同じ処理をインライン展開します
        q->head = (q->head + 1) % QUEUE_SIZE;
        q->count--;
    }

    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;

    pthread_mutex_unlock(&q->mutex);
    return true;
}

// データを取り出す（スレッドセーフ）
bool Dequeue(WaveQueue *q, float *out_value) {
    pthread_mutex_lock(&q->mutex);

    if (q->count <= 0) {
        pthread_mutex_unlock(&q->mutex);
        return false; // 空
    }

    *out_value = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;

    pthread_mutex_unlock(&q->mutex);
    return true;
}

float WaveQueue_get(WaveQueue *q, int index) {
    pthread_mutex_lock(&q->mutex);
    if (index < 0 || index >= q->count) {
        pthread_mutex_unlock(&q->mutex);
        return 0.0; // インデックスが範囲外
    }
    int real_index = (q->head + index) % QUEUE_SIZE;
    float value = q->data[real_index];
    pthread_mutex_unlock(&q->mutex);
    return value;
}