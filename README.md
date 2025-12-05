# Ring Buffer Library / 环形缓冲区库

## English

A header-only, OOP-style ring buffer implementation in C with function pointers.

### Features

- Thread-safe implementation (for single producer/consumer scenarios)
- Header-only library - no separate compilation required
- OOP-style design using function pointers
- No dynamic memory allocation
- MIT licensed

### Usage

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

### Building the Tests

To build and run the tests:

```bash
cd test
make
make run
```

### API Reference

The ring buffer provides the following function pointers:

- `write(data, len)`: Write data to the buffer
- `read(data, len)`: Read data from the buffer
- `available()`: Get available space in the buffer
- `used()`: Get used space in the buffer
- `is_empty()`: Check if the buffer is empty
- `is_full()`: Check if the buffer is full
- `clear()`: Clear the buffer

---

## 中文

一个纯头文件的、使用函数指针实现面向对象风格的C语言环形缓冲区实现。

### 特性

- 线程安全实现（适用于单生产者/消费者场景）
- 纯头文件库 - 无需单独编译
- 使用函数指针的面向对象风格设计
- 无动态内存分配
- MIT许可证

### 使用方法

```c
#include "ring_buffer.h"

// 定义一个容量为32字节的环形缓冲区
RING_BUFF_PRE_INIT(my_buffer, 32)

// 获取环形缓冲区实例
ring_buff_t *rb = my_ring_buff();

// 写入数据
const uint8_t data[] = {1, 2, 3, 4, 5};
int32_t written = rb->write(data, sizeof(data));

// 读取数据
uint8_t buffer[32];
int32_t read = rb->read(buffer, sizeof(buffer));

// 检查状态
bool is_empty = rb->is_empty();
bool is_full = rb->is_full();
uint16_t available = rb->available();
uint16_t used = rb->used();
```

### 构建测试

构建并运行测试：

```bash
cd test
make
make run
```

### API参考

环形缓冲区提供以下函数指针：

- `write(data, len)`: 向缓冲区写入数据
- `read(data, len)`: 从缓冲区读取数据
- `available()`: 获取缓冲区可用空间
- `used()`: 获取缓冲区已用空间
- `is_empty()`: 检查缓冲区是否为空
- `is_full()`: 检查缓冲区是否已满
- `clear()`: 清空缓冲区