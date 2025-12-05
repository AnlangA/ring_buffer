#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "ring_buffer.h"

/*
 * Prepare a ring buffer using the PRE_INIT macro.
 * This provides all function pointer interfaces via name binding.
 * Now uses static buffer allocation.
 */
RING_BUFF_PRE_INIT(test, 16)

/*
 * Declare extern getter using the EXTERN macro for the same name.
 */
RING_BUFF_EXTERN(test)

/* Utility: dump buffer state for debugging */
static void dump_rb(const char *tag, const ring_buff_t *rb) {
    printf("[%s] size=%u head=%u tail=%u full=%d used=%u avail=%u\n",
           tag,
           rb->size,
           rb->head,
           rb->tail,
           rb->is_full ? (rb->is_full() ? 1 : 0) : 0,
           rb->used ? rb->used() : 0,
           rb->available ? rb->available() : 0);
}

/* Test helper: compare memory content */
static void expect_bytes(const uint8_t *actual, const uint8_t *expected, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        assert(actual[i] == expected[i]);
    }
}

/* Tests for PRE_INIT'd ring buffer: exercise all function pointers and macros */
static void test_ring_buff_pre_init_basic(void) {
    printf("=== test_ring_buff_pre_init_basic ===\n");
    /* Acquire instance and initialize storage via name_ring_buff() */
    ring_buff_t *rb = test_ring_buff();
    assert(rb != NULL);
    assert(rb->p_buff != NULL);
    assert(rb->size == 16);

    /* Validate function pointers are assigned */
    assert(rb->write != NULL);
    assert(rb->read != NULL);
    assert(rb->available != NULL);
    assert(rb->used != NULL);
    assert(rb->is_empty != NULL);
    assert(rb->is_full != NULL);
    assert(rb->clear != NULL);
    /* del is removed */

    /* Initial state: capacity is size-1 because one slot is reserved */
    assert(rb->used() == 0);
    assert(rb->available() == rb->size - 1);
    assert(rb->is_empty() == true);
    assert(rb->is_full() == false);

    /* Write some bytes */
    const uint8_t payload1[] = {10, 11, 12, 13, 14};
    int32_t w = rb->write(payload1, sizeof(payload1));
    assert(w == (int32_t)sizeof(payload1));
    assert(rb->used() == sizeof(payload1));
    assert(rb->available() == rb->size - 1 - sizeof(payload1));
    assert(rb->is_empty() == false);
    assert(rb->is_full() == false);

    dump_rb("after write 5", rb);

    /* Read partial */
    uint8_t out1[3] = {0};
    int32_t r = rb->read(out1, sizeof(out1));
    assert(r == 3);
    expect_bytes(out1, (uint8_t[]){10, 11, 12}, 3);
    assert(rb->used() == 2);
    /* capacity is size-1 => available = (size-1-used) */
    assert(rb->available() == rb->size - 1 - 2);
    assert(rb->is_empty() == false);
    assert(rb->is_full() == false);

    dump_rb("after read 3", rb);

    /* Write wrap-around: write 12 bytes */
    uint8_t payload2[12];
    for (int i = 0; i < 12; i++) payload2[i] = (uint8_t)(20 + i);
    w = rb->write(payload2, sizeof(payload2));
    assert(w == 12);
    /* Now used should be 14 (2 + 12), capacity is size-1 (=15) so available=1 */
    assert(rb->used() == 14);
    assert(rb->available() == (rb->size - 1 - 14));
    assert(rb->is_full() == false);

    dump_rb("after write 12", rb);

    /* Overfill by 2 bytes: available=1 < len=2, write should fail entirely */
    const uint8_t payload3[] = {99, 100};
    w = rb->write(payload3, sizeof(payload3));
    assert(w == -1);
    assert(rb->used() == 14);
    assert(rb->available() == 1);
    assert(rb->is_full() == false);
    assert(rb->is_empty() == false);

    dump_rb("after failed overfill 2 bytes", rb);

    /* Write exactly 1 byte to reach full (used = size-1, available = 0) */
    const uint8_t payload4[] = {77};
    w = rb->write(payload4, sizeof(payload4));
    assert(w == 1);
    assert(rb->used() == rb->size - 1);
    assert(rb->available() == 0);
    assert(rb->is_full() == true);
    assert(rb->is_empty() == false);

    dump_rb("after fill to full", rb);

    /* Attempt overflow when full: write should fail and return -1 */
    const uint8_t payload_over[] = {1, 2, 3};
    w = rb->write(payload_over, sizeof(payload_over));
    assert(w == -1);
    assert(rb->is_full() == true);

    /* Read all back: first read 10 bytes, then read rest */
    uint8_t out_all[16] = {0};
    r = rb->read(out_all, 10);
    assert(r == 10);
    assert(rb->is_full() == false); /* reading clears full state */
    /* remaining used = (size-1 - 10) */
    assert(rb->used() == (rb->size - 1 - 10));
    dump_rb("after read 10", rb);

    r = rb->read(out_all + 10, (uint16_t)(rb->size - 1 - 10));
    assert(r == (int32_t)(rb->size - 1 - 10));
    assert(rb->used() == 0);
    assert(rb->is_empty() == true);

    /* Clear should reset pointers and flags */
    rb->clear();
    assert(rb->used() == 0);
    assert(rb->available() == rb->size - 1);
    assert(rb->is_empty() == true);
    assert(rb->is_full() == false);

    dump_rb("after clear", rb);

    printf("OK: PRE_INIT basic behaviors validated\n");
}

/* Test that name_ring_buff_get returns the same static instance address */
static void test_ring_buff_getter(void) {
    printf("=== test_ring_buff_getter ===\n");
    ring_buff_t *rb_get = test_ring_buff_get();
    assert(rb_get != NULL);

    /* Ensure buffer is initialized */
    if (rb_get->p_buff == NULL) {
        ring_buff_t *rb_init = test_ring_buff();
        assert(rb_init == rb_get);
        assert(rb_get->p_buff != NULL);
    }

    /* Basic write/read via getter instance */
    const uint8_t data[] = {42, 43, 44, 45};
    int32_t w = rb_get->write(data, sizeof(data));
    assert(w == 4);
    assert(rb_get->used() == 4);
    /* capacity is size-1 => available = (size-1-used) */
    assert(rb_get->available() == rb_get->size - 1 - 4);

    uint8_t out[4] = {0};
    int32_t r = rb_get->read(out, sizeof(out));
    assert(r == 4);
    expect_bytes(out, data, 4);
    assert(rb_get->used() == 0);
    assert(rb_get->is_empty() == true);

    printf("OK: EXTERN getter returns same instance and supports operations\n");
}

/* Boundary and robustness tests */
static void test_boundary_conditions(void) {
    printf("=== test_boundary_conditions ===\n");
    ring_buff_t *rb = test_ring_buff();
    assert(rb != NULL);

    /* Zero-length writes/reads */
    uint8_t d = 1;
    int32_t w = rb->write(&d, 0);
    assert(w == -1); /* per implementation: reject len==0 */
    int32_t r = rb->read(&d, 0);
    assert(r == 0); /* per implementation: return 0 */

    /* NULL buffer args */
    w = rb->write(NULL, 1);
    assert(w == -1);
    r = rb->read(NULL, 1);
    assert(r == 0);

    /* Fill behavior with size-1 capacity and no partial writes */
    rb->clear();
    uint8_t fill[16];
    for (int i = 0; i < 16; i++) fill[i] = (uint8_t)i;

    /* Request write length > capacity => expect failure (-1), buffer stays empty */
    w = rb->write(fill, 16);
    assert(w == -1);
    assert(rb->is_empty() == true);
    assert(rb->used() == 0);
    assert(rb->available() == rb->size - 1);

    /* Now write exactly capacity (size-1) bytes and verify full state */
    w = rb->write(fill, (uint16_t)(rb->size - 1));
    assert(w == rb->size - 1);
    assert(rb->is_full() == true);
    assert(rb->used() == rb->size - 1);
    assert(rb->available() == 0);

    /* Consume all used bytes and check empty state */
    uint8_t out[16];
    r = rb->read(out, 16);
    assert(r == rb->size - 1);
    expect_bytes(out, fill, (uint16_t)(rb->size - 1));
    assert(rb->is_full() == false);
    assert(rb->is_empty() == true);
    assert(rb->used() == 0);
    assert(rb->available() == rb->size - 1);

    /* Wrap around sequence: write 6, read 4, write 10, verify order */
    const uint8_t seq1[6] = {5, 6, 7, 8, 9, 10};
    w = rb->write(seq1, 6);
    assert(w == 6);
    uint8_t outA[4] = {0};
    r = rb->read(outA, 4);
    assert(r == 4);
    expect_bytes(outA, (uint8_t[]){5, 6, 7, 8}, 4);
    const uint8_t seq2[10] = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
    w = rb->write(seq2, 10);
    assert(w == 10);
    /* Now total used: 2 remaining from seq1 + 10 = 12 */
    assert(rb->used() == 12);

    uint8_t outB[12] = {0};
    r = rb->read(outB, 12);
    assert(r == 12);
    expect_bytes(outB, (uint8_t[]){9,10,50,51,52,53,54,55,56,57,58,59}, 12);
    assert(rb->is_empty() == true);

    printf("OK: boundary and robustness validated\n");
}

/* Test that multiple calls to test_ring_buff() don't reallocate when already allocated */
static void test_multiple_init_calls(void) {
    printf("=== test_multiple_init_calls ===\n");
    ring_buff_t *rb1 = test_ring_buff();
    assert(rb1 != NULL);
    uint8_t *p1 = rb1->p_buff;

    ring_buff_t *rb2 = test_ring_buff();
    assert(rb2 != NULL);
    uint8_t *p2 = rb2->p_buff;

    /* Same static instance and same underlying buffer pointer */
    assert(rb1 == rb2);
    assert(p1 == p2);

    printf("OK: multiple init calls reuse same instance\n");
}

int main(void) {
    printf("Ring buffer tests start\n");

    test_ring_buff_pre_init_basic();
    test_ring_buff_getter();
    test_boundary_conditions();
    test_multiple_init_calls();

    printf("All ring buffer tests passed\n");
    return 0;
}