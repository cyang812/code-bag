#include "data_io.h"
#include <stdio.h>
#include <string.h>

void create_ringBuffer(ringbuffer_t *ringBuf,uint8_t *buf,uint32_t buf_len)
{
	ringBuf->br 		= 0;
	ringBuf->bw 		= 0;
	ringBuf->btoRead 	= 0;
	ringBuf->source 	= buf;
	ringBuf->length 	= buf_len;
	printf("create ringBuffer->length = %d\n",ringBuf->length);
}

void clear_ringBuffer(ringbuffer_t *ringBuf)
{
	ringBuf->br 	= 0;
	ringBuf->bw 	= 0;
	ringBuf->btoRead = 0;
	
	//no need do this casue r_ptr and w_prt have changed
//	memset((uint8_t *)ringBuf->source, 0, ringBuf->length); 
}

uint32_t write_ringBuffer(uint8_t *buffer, uint32_t size, ringbuffer_t *ringBuf)
{
	uint32_t len 			= 0;
	uint32_t write_cnt 		= 0;
	uint32_t ringBuf_bw	 	= ringBuf->bw;
	uint32_t ringBuf_len	= ringBuf->length;
	uint8_t *ringBuf_source = ringBuf->source;
	uint32_t bCanWrite      = ringBuf->length - ringBuf->btoRead;
	uint32_t real_bytes		= size < bCanWrite ? size : bCanWrite; 

	#if 0
	if( (ringBuf_bw % ringBuf_len + size) <= ringBuf_len )
	{
	    for(uint32_t i=0; i<size; i++)
	    {
	        ringBuf->source[ringBuf_bw % ringBuf_len + i] = buffer[i];
	    }
	    ringBuf->bw += size;
		write_cnt = size;
	}
	else
	{
		len = ringBuf_len - (ringBuf_bw % ringBuf_len);
		for(uint32_t i=0; i<len; i++)
		{
			ringBuf->source[ringBuf_bw % ringBuf_len + i] = buffer[i];
		}
		
		for(uint32_t j=0; j<size-len; j++)
		{
			ringBuf->source[j] = buffer[len+j];
		}
		ringBuf->bw += size;
		write_cnt = size;
	}

	ringBuf->btoRead += size;
    return write_cnt;
    #else
	if( (ringBuf_bw + real_bytes) <= ringBuf_len )
	{
		memcpy(ringBuf_source + ringBuf_bw, buffer, real_bytes);
	}
	else
	{
		len = ringBuf_len - ringBuf_bw;
		memcpy(ringBuf_source + ringBuf_bw, buffer, len);
		memcpy(ringBuf_source, buffer + len, real_bytes - len);
	}

    ringBuf->bw = (ringBuf_bw + real_bytes) % ringBuf_len;
	ringBuf->btoRead += real_bytes;
	
    return real_bytes;
    #endif
}

uint32_t read_ringBuffer(uint8_t *buffer, uint32_t size, ringbuffer_t *ringBuf)
{
	uint32_t len 			= 0;
	uint32_t read_cnt 		= 0;
	uint32_t ringBuf_br 	= ringBuf->br;
	uint32_t ringBuf_len 	= ringBuf->length;
	uint8_t *ringBuf_source = ringBuf->source;
	uint32_t real_bytes     = size < ringBuf->btoRead ? size : ringBuf->btoRead;  
	
	#if 0
	if( (ringBuf_br % ringBuf_len + size) <= ringBuf_len )
	{
	    for(uint32_t i=0; i<size; i++)
	    {
	        buffer[i] = ringBuf->source[ringBuf_br % ringBuf_len + i];
	    }
	    ringBuf->br += size;
		read_cnt = size;
	}
	else
	{
		len = ringBuf_len - (ringBuf_br % ringBuf_len);
		for(uint32_t i=0; i<len; i++)
		{
			buffer[i] = ringBuf->source[ringBuf_br % ringBuf_len + i];
		}
		
		for(uint32_t j=0; j<size-len; j++)
		{
			buffer[len+j] = ringBuf->source[j];
		}
		ringBuf->br += size;
		read_cnt = size;
	}

	ringBuf->btoRead -= size;
	return read_cnt;
	#else
	if( (ringBuf_br + real_bytes) <= ringBuf_len )
	{
		memcpy(buffer, ringBuf_source + ringBuf_br, real_bytes);
	}
	else
	{
		len = ringBuf_len - ringBuf_br;
		memcpy(buffer, ringBuf_source + ringBuf_br, len);
		memcpy(buffer + len, ringBuf_source, real_bytes - len);
	}

	ringBuf->br = (ringBuf_br + real_bytes) % ringBuf_len;
	ringBuf->btoRead -= real_bytes;

	return real_bytes;
	#endif
}

uint32_t get_ringBuffer_btoRead(ringbuffer_t *ringBuf)
{
	return ringBuf->btoRead;
}

uint32_t get_ringBuffer_length(ringbuffer_t *ringBuf)
{
	return ringBuf->length;
}

uint32_t get_ringBuffer_bCanWrite(ringbuffer_t * ringBuf)
{
	return ringBuf->length - ringBuf->btoRead;
}

