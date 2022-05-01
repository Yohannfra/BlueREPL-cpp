BIN_NAME=blerepl

build:
	@cmake --build build
	@cp ./build/compile_commands.json .
	@cp ./build/$(BIN_NAME) .

gen:
	@cmake -S . -B build

clean:
	@$(RM) -r build/

re: clean gen build

.PHONY: build gen clean re
