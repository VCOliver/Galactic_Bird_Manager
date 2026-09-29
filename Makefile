CC := gcc
CFLAGS := -Wall -Wextra -Iinclude

TARGET := gbm
BUILD_DIR := build

SRCS := $(shell find src -name '*.c')
OBJS := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean