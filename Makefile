BIN_NAME=bluerepl

compile: gen
	@cmake --build build
	@cp ./build/compile_commands.json .
	@cp ./build/$(BIN_NAME) .

gen: build
	@cmake -S . -B build

build:
	mkdir build

clean:
	@$(RM) -r build/

re: clean gen compile

run_tests:
	cmake -S . -B build
	@cd tests/ && \
			cmake -S . -B build && \
			cmake --build build && \
			cd build && \
			./bluerepl_test

lint:
	@cppcheck --quiet --enable=warning,style,performance,portability src

format:
	@clang-format -i \
			src/*/**.cpp \
			src/*/**.hpp \
			src/*/**.h \
			tests/src/*.cpp


.PHONY: compile gen clean re lint run_tests format
