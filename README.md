# 概要
Arduino IDE 環境のプログラム(inoファイル)を実行した際のIOの可視化を行う
![gif](https://github.com/yoshiminato/Arduino_IO_visualizer/blob/main/assets/videos/demo.gif)

---

# 環境構築

- リポジトリのクローン
```bash 
cd ~/
git clone git@github.com:yoshiminato/Arduino_IO_visualizer.git
```

- 依存ライブラリの準備
``` bash

# クローン
cd Arduino_IO_visualizer
git clone git@github.com:raysan5/raylib.git
# ビルド
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
# インストール
sudo make install
mkdir --parents --verbose /usr/local/lib
mkdir --parents --verbose /usr/local/include
cp --update --verbose ../src/libraylib.a /usr/local/lib/libraylib.a
```

- 仮想環境の作成&構築
```bash
cd Arduino_IO_visualizer
python3 -m venv .venv
source .venv/bin/activate
pip install python-docx
```

---

# クイックスタート
```bash
# ビルド
cd Arduino_IO_visualizer
make
# 実行
./bin/executor
```
![gif](https://github.com/yoshiminato/Arduino_IO_visualizer/blob/main/assets/videos/demo.gif)

### 動作デモ
デフォルトでは `/data/test/test1.docx` に記述された以下のプログラムが実行されます。

```c
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  static int led = 10;
  int sw = digitalRead(2);
  static int dir = 1;

  int currentDir = (sw == LOW) ? -dir : dir;
  led += currentDir;
  if (led > 13) led = 10;
  if (led < 10) led = 13;

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(led, HIGH);
  delay(500);
}
```

### 操作方法
* **挙動:** I/O 10〜13ピンに順にパルスが立ち、そのシフト方向はI/O 2ピンからの入力（スイッチ状態）によって反転します。
* **キーボード操作:** 入力信号はキーボードによってシミュレート可能です。ソースコード内で `pinMode` で定義された入力ピンの順に、以下のキーが自動的に割り当てられます。
  * **対応キー:** `q`, `w`, `e`, `r`, `t`, `y`, `u`, `i`, `o`, `p`
  * (例: 上記プログラムの場合、最初に定義されている入力ピン `2` が `q` に対応します)

---



# ファイル構造
```text
.
├── Makefile
├── README.md
├── bin
│   ├── execute_file_creator
│   └── executor
├── data
│   └── test
│       └── test1.docx
├── include
│   ├── error.h
│   ├── override.h
│   └── pin_state.h
├── params
│   └── all_low
├── scripts
│   └── convert_docx2txt.py
└── src
    ├── check.c
    ├── error.c
    ├── execute_file.c
    ├── execute_file_creator.c
    ├── override.c
    └── pin_state.c
```
