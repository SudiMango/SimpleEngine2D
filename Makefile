BUILD_DIR := build
NAME ?=
TYPE ?= Debug

.PHONY: all configure build game run editor-build editor-run clean rebuild deps

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(TYPE)

build: configure
	cmake --build $(BUILD_DIR) -j

game:
	@if [ -z "$(NAME)" ]; then echo "Usage: make create-game NAME=<game-name>"; exit 1; fi
	chmod +x ./scripts/create-game.sh && ./scripts/create-game.sh "$(NAME)"

run: build
	@if [ -z "$(NAME)" ]; then echo "Usage: make run NAME=<game-name>"; exit 1; fi
	cmake --build $(BUILD_DIR) -j --target "$(NAME)"
	./$(BUILD_DIR)/games/$(NAME)/$(NAME)

editor-build: build
	cmake --build $(BUILD_DIR) -j --target SimpleEngine2DEditor

editor-run: editor-build
	./$(BUILD_DIR)/editor/SimpleEngine2DEditor

clean:
	rm -rf $(BUILD_DIR)

deps:
	chmod +x ./scripts/install-deps.sh && ./scripts/install-deps.sh

rebuild: clean build
