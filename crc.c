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


uint16_t Calc(const uint8_t *data, uint8_t len) 
{
	uint16_t crc = 0x0000;

	for(uint8_t offset = 0; offset < len; offset++)
	{
		crc = (crc << 8) ^ crc_lut[(crc >> 8) ^ data[offset]];
	}

	return crc;
}

int main(int argc, char const *argv[])
{
	/* code */
	uint16_t gen_polynom;
//	gen_polynom = 0x1021;  // CalcCRC_CRC16_CCITT dls dab+ 0001 0000 0010 0001 (16, 12, 5, 0)
	// gen_polynom = 0x8005;  // CalcCRC_CRC16_IBM mp2 1000 0000 0000 0101 (16, 15, 2, 0)
	gen_polynom = 0x782f;  // CalcCRC_FIRE_CODE dab+ 0111 1000 0010 1111 (16, 14, 13, 12, 11, 5, 3, 2, 1, 0)

	FillLUT(gen_polynom);

	// for(uint16_t i=0; i<256; i++)
	// {
	// 	if(i%8==0 && i!=0)
	// 	{
	// 		printf("\n");
	// 	}
	// 	printf("0x%04x, ", crc_lut[i]);
	// }

	// printf("\n");

	uint8_t seq1[9] = {1,2,3,4,5,6,7,8,9};
	uint8_t seq2[9] = {9,1,2,3,4,5,6,7,8};
	// uint8_t seq2[9] = {1,1,2,2,2,2,2,2,2};

	uint16_t crc1, crc2;

	crc1 = Calc(seq1, sizeof(uint8_t));
	crc2 = Calc(seq2, sizeof(uint8_t));

	printf("crc1 = 0x%04x\n", crc1);
	printf("crc2 = 0x%04x\n", crc2);

	return 0;
}