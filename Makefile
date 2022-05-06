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

lint:
	cppcheck --quiet --enable=warning,style,performance,portability src

.PHONY: build gen clean re lint
