CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g

BUILD_DIR = build
TARGET = $(BUILD_DIR)/deadlock
SRCS = deadlock.c
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET) exec

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c deadlock.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

exec: 
	./$(TARGET)


clean:
	rm -rf $(BUILD_DIR) *.o

.PHONY: all clean
