# Ring Buffer Library

A header-only, OOP-style ring buffer implementation in C with function pointers.

## Language / 语言

[English](README_EN.md) | [中文](README_CN.md)

## Quick Start

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

## Features

- Thread-safe implementation (for single producer/consumer scenarios)
- Header-only library - no separate compilation required
- OOP-style design using function pointers
- No dynamic memory allocation
- MIT licensed

## Building the Tests

```bash
cd test
make
make run
```

## License

MIT License - see [LICENSE](LICENSE) file for details.