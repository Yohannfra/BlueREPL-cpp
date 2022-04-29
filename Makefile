BIN_NAME=blerepl

build:
	@cd build && \
			make && \
			cp ./compile_commands.json .. && \
			cp ./$(BIN_NAME) ..

gen:
	@mkdir -p build && \
			cd build && \
			cmake ..

clean:
	@$(RM) -r build/

re: clean all

.PHONY: all clean re build
