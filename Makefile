BUILD_DIR := build
NAME ?=

.PHONY: all configure build create-game run clean rebuild deps

all: build

configure:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR) -j

game:
	@if [ -z "$(NAME)" ]; then echo "Usage: make create-game NAME=<game-name>"; exit 1; fi
	chmod +x ./scripts/create-game.sh && ./scripts/create-game.sh "$(NAME)"

run: build
	@if [ -z "$(NAME)" ]; then echo "Usage: make run NAME=<game-name>"; exit 1; fi
	cmake --build $(BUILD_DIR) -j --target "$(NAME)"
	./$(BUILD_DIR)/games/$(NAME)/$(NAME)

clean:
	rm -rf $(BUILD_DIR)

deps:
	chmod +x ./scripts/install-deps.sh && ./scripts/install-deps.sh

rebuild: clean build
