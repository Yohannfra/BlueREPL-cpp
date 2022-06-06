BIN_NAME=bluerepl

compile: gen
	@cmake --build build
	@cp ./build/compile_commands.json .
	@cp ./build/$(BIN_NAME) .

gen: build
	@cmake -S . -B build

clean:
	@$(RM) -r build/

re: clean gen build

run_tests:
	cmake -S . -B build
	@cd tests/ && \
			cmake -S . -B build && \
			cmake --build build && \
			./build/bluerepl_test

lint:
	cppcheck --quiet --enable=warning,style,performance,portability src

.PHONY: compile gen clean re lint run_tests
