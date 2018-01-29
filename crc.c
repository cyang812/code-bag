/*
*	cyang 
*	2018/1/29 
*/

#include <stdio.h>
#include <stdint.h>

uint16_t crc_lut[256];

void FillLUT(uint16_t gen_polynom) 
{
	for(int value = 0; value < 256; value++) 
	{
		uint16_t crc = value << 8;

		for(int i = 0; i < 8; i++) 
		{
			if(crc & 0x8000)
				crc = (crc << 1) ^ gen_polynom;
			else
				crc = crc << 1;
		}

		crc_lut[value] = crc;
	}
}


int main(int argc, char const *argv[])
{
	/* code */
	uint16_t gen_polynom;
//	gen_polynom = 0x1021;  // 0001 0000 0010 0001 (16, 12, 5, 0)
	gen_polynom = 0x782f;  // 0111 1000 0010 1111 (16, 14, 13, 12, 11, 5, 3, 2, 1, 0)

	FillLUT(gen_polynom);

	for(uint16_t i=0; i<256; i++)
	{
		if(i%8==0 && i!=0)
		{
			printf("\n");
		}
		printf("0x%04x, ", crc_lut[i]);
	}

	printf("\n");
	return 0;
}