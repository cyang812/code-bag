#ifndef DATA_IO_H
#define DATA_IO_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//volatile 

typedef struct {
    uint8_t *source;
    uint32_t br;
    uint32_t bw;
    uint32_t btoRead;
	uint32_t length;
}ringbuffer_t;

extern void create_ringBuffer(ringbuffer_t * ringBuf, uint8_t * buf, uint32_t buf_len);
extern void clear_ringBuffer(ringbuffer_t *ringBuf);
extern uint32_t write_ringBuffer(uint8_t * buffer, uint32_t size, ringbuffer_t * ringBuf);
extern uint32_t read_ringBuffer(uint8_t *buffer, uint32_t size, ringbuffer_t *ringBuf);
extern uint32_t get_ringBuffer_btoRead(ringbuffer_t *ringBuf);
extern uint32_t get_ringBuffer_length(ringbuffer_t *ringBuf);
extern uint32_t get_ringBuffer_bCanWrite(ringbuffer_t * ringBuf);

#endif

