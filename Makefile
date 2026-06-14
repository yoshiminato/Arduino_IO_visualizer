.PHONY: all clean out 

all: compile_creator create out

# 実行ファイル作成プログラムのコンパイル
compile_creator: src/execute_file_creator.c
	gcc -o bin/execute_file_creator src/execute_file_creator.c

# 実行ファイル作成プログラムの実行
create:
	./bin/execute_file_creator

# 最終実行プログラムのコンパイル
out: src/execute_file.c src/override.c src/pin_state.c src/error.c src/draw.c src/queue.c src/input.c
	gcc -I include -o bin/executor src/execute_file.c src/override.c src/pin_state.c src/error.c src/draw.c src/queue.c src/input.c -lraylib -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f bin/executor src/execute_file.c bin/execute_file_creator 
