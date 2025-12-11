.PHONY: all clean out 

all: compile_creator create out

compile_creator: src/execute_file_creator.c
	gcc -o bin/execute_file_creator src/execute_file_creator.c

create:
	./bin/execute_file_creator

out: src/execute_file.c src/override.c src/pin_state.c src/error.c
	gcc -I include -o out src/execute_file.c src/override.c src/pin_state.c src/error.c

clean:
	rm -f out src/execute_file.c bin/execute_file_creator 
