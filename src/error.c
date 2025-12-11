#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void raiseNoPinError(int pin) {
    fprintf(stderr, "Error: ピン %d は定義されていません\n", pin);
    exit(EXIT_FAILURE);
}

void raiseInvalidFuncError(int pin, int mode) {
    fprintf(stderr, "Error: ピン %d はモード %s で定義されていますが無効な関数が呼ばれました\n", 
        pin, 
        (mode == INPUT) ? "INPUT" : (mode == OUTPUT) ? "OUTPUT" : "UNKNOWN"
    );
    exit(EXIT_FAILURE);
}

void raiseInvalidModeError(int pin, int mode) {
    fprintf(stderr, "Error: ピン %d に無効なモード %d が設定されました\n", pin, mode);
    exit(EXIT_FAILURE);
}

void raiseInvalidPinNumberError(int pin) {
    fprintf(stderr, "Error: ピン番号 %d は無効です\n", pin);
    exit(EXIT_FAILURE);
}

void raiseLoadInputFileError(const char* filename) {
    fprintf(stderr, "Error: 入力ピン状態ファイル '%s' の読み込みに失敗しました\n", filename);
    exit(EXIT_FAILURE);
}

void raiseFileOpenError(const char* filename) {
    fprintf(stderr, "Error: ファイル '%s' を開くことができませんでした\n", filename);
    exit(EXIT_FAILURE);
}