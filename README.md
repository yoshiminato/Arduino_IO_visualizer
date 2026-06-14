# 概要
Arduino IDE 環境のプログラム(inoファイル)を実行した際のIOの可視化を行う
[!gif](https://github.com/yoshiminato/Arduino_IO_visualizer/blob/main/assets/videos/demo.gif)

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


# クイックスタート
```bash
# ビルド
cd Arduino_IO_visualizer
make
# 実行
./bin/executor
```



# ファイル構造
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
