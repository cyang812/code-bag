#include "find_syncWord.h"
#include <stdio.h>
#include <stdint.h>


uint16_t Calc_FIRE_CODE(const uint8_t *data, uint32_t len) 
{
	uint16_t crc = 0x0000;

	for(uint32_t offset = 0; offset < len; offset++)
	{	
		crc = (crc << 8) ^ crc_fire_code_tab[(crc >> 8) ^ data[offset]];
		printf("crc = 0x%04x\n", crc);
	}

	return crc;
}

uint16_t find_sync_word(uint8_t *data, uint32_t data_len)
{
	uint8_t *ptr;
	uint16_t crc_stored,crc_calced;

	ptr = data;

	for(uint32_t i=0; i<data_len-9; i++)
	{
		crc_stored = ptr[0]<<8 | ptr[1];
		crc_calced = Calc_FIRE_CODE((uint8_t *)&ptr[2], 9);

		if( (crc_stored != 0x0000) && (crc_stored == crc_calced) )
		{
			printf("crc check ok! crc1 = 0x%04x,crc2 = 0x%04x\n", crc_stored,crc_calced);
			return i;
		}

		ptr++;
	}

	return 0xffff;
}

void find_test()
{
	uint16_t idx = 0;	
	idx = find_sync_word(super_frame_buf, sizeof(super_frame_buf));
	printf("idx = %d\n", idx);
	if(idx != 0xffff)
	{
		printf("super_frame_buf[%d] = 0x%02x\n",idx,super_frame_buf[idx]);
		printf("super_frame_buf[%d] = 0x%02x\n",idx+1,super_frame_buf[idx+1]);
	}
}

void check_test()
{
	uint8_t seq1[] = {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t seq2[] = {0x95, 0xFF, 0x70, 0x24, 0xA3, 0xBF, 0x80, 0x34, 0x66, 0x02, 0xD0};

	uint16_t crc;

	crc = seq1[0]<<8 | seq1[1];
	printf("crc_stored = 0x%04x\n",crc);
	crc = Calc_FIRE_CODE((uint8_t *)&seq1[2],9);
	printf("crc_calced = 0x%04x\n",crc);
}

int main(int argc, char const *argv[])
{
	/* code */

	// printf("sizeof(buf) = %d\n", sizeof(super_frame_buf1));
	// find_test();

	check_test();	
	return 0;
}