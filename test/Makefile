# Makefile for Ring Buffer Test

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
SRC_DIR = ../src
TEST_DIR = .
TARGET = test_ring_buffer
TARGET_EXE = $(TARGET).exe

# Source files
SOURCES = $(TEST_DIR)/test.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET_EXE)

# Build the test executable
$(TARGET_EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -I$(SRC_DIR)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(SRC_DIR)

# Clean build artifacts
clean:
	del /Q *.o $(TARGET_EXE) 2>nul || true

# Run the test
run: $(TARGET_EXE)
	./$(TARGET_EXE)

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET_EXE)

.PHONY: all clean run debug