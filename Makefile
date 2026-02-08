CC := gcc
BUILD_DIR := ./bin
SRC_DIR := ./src

.PHONY: all
all: ccwc

ccwc: $(SRC_DIR)/ccwc/main.c buildDir
	$(CC) $< -o $(BUILD_DIR)/@

.PHONY: buildDir
buildDir:
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
