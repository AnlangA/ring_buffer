# Ring Buffer

A simple and efficient ring buffer (circular buffer) implementation in C.

## Features

- Thread-safe operations
- Fixed size circular buffer
- O(1) push and pop operations
- Memory efficient
- Easy to integrate

## Usage

```c
#include "ring_buffer.h"

// Create a ring buffer of size 10
ring_buffer_t buffer;
ring_buffer_init(&buffer, 10);

// Push items
ring_buffer_push(&buffer, 42);
ring_buffer_push(&buffer, 123);

// Pop items
int value;
ring_buffer_pop(&buffer, &value);

// Clean up
ring_buffer_cleanup(&buffer);
```

## Building

```bash
cd test
make
./test
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.