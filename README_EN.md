# Ring Buffer Library

A header-only, OOP-style ring buffer implementation in C with function pointers.

## Language / 语言

[English](README.md) | [中文](README_CN.md)

## Features

- Thread-safe implementation (for single producer/consumer scenarios)
- Header-only library - no separate compilation required
- OOP-style design using function pointers
- No dynamic memory allocation
- MIT licensed

## Usage

```c
#include "ring_buffer.h"

// Define a ring buffer with 32 bytes capacity
RING_BUFF_PRE_INIT(my_buffer, 32)

// Get the ring buffer instance
ring_buff_t *rb = my_ring_buff();

// Write data
const uint8_t data[] = {1, 2, 3, 4, 5};
int32_t written = rb->write(data, sizeof(data));

// Read data
uint8_t buffer[32];
int32_t read = rb->read(buffer, sizeof(buffer));

// Check status
bool is_empty = rb->is_empty();
bool is_full = rb->is_full();
uint16_t available = rb->available();
uint16_t used = rb->used();
```

## Building the Tests

To build and run the tests:

```bash
cd test
make
make run
```

## API Reference

The ring buffer provides the following function pointers:

- `write(data, len)`: Write data to the buffer
- `read(data, len)`: Read data from the buffer
- `available()`: Get available space in the buffer
- `used()`: Get used space in the buffer
- `is_empty()`: Check if the buffer is empty
- `is_full()`: Check if the buffer is full
- `clear()`: Clear the buffer

## License

MIT License - see [LICENSE](LICENSE) file for details.