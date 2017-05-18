#include <stdio.h>
#include "stdint.h"

#define DLS_LEN		(uint8_t)129
#define DWBYTE(b3, b2, b1, b0) (((uint32_t)((uint8_t)(b3) << 24)) | ((uint8_t)(b2) << 16) | ((uint8_t)(b1) << 8) | ((uint8_t)(b0)))

void UTF8ToUnicode(uint8_t *UTF8,uint16_t *Unicode)
{
	uint16_t i=0,j=0;
	uint8_t buf[4];
	
	while(UTF8[i])
	{
		if((UTF8[i]&0x80)==0x00)
		{
			Unicode[j]=(uint16_t)DWBYTE(0,0,0,UTF8[i]);
			i+=1;
		}
		else if((UTF8[i]&0xe0)==0xc0)
		{
			buf[1]=(UTF8[i]&0x1c)>>2;
			buf[0]=(UTF8[i]<<6)|(UTF8[i+1]&0x3f);
			Unicode[j]=(uint16_t)DWBYTE(0,0,buf[1],buf[0]);
			i+=2;
		}
		else if((UTF8[i]&0xf0)==0xe0)
		{
			buf[1]=(UTF8[i]<<4)|((UTF8[i+1]&0x3c)>>2);
			buf[0]=(UTF8[i+1]<<6)|(UTF8[i+2]&0x3f);
			Unicode[j]=(uint16_t)DWBYTE(0,0,buf[1],buf[0]);
			i+=3;
		}
		else if((UTF8[i]&0xf8)==0xf0)
		{
			buf[2]=((UTF8[i]&0x07)<<2)|((UTF8[i+1]&0x30)>>4);
			buf[1]=((UTF8[i+1]&0x0f)<<4)|((UTF8[i+2]&0x3c)>>2);
			buf[0]=(UTF8[i+2]<<6)|(UTF8[i+3]&0x3f);
			Unicode[j]=(uint16_t)DWBYTE(0,buf[2],buf[1],buf[0]);
			i+=4;
		}
		else
		{
			Unicode[j] = (uint16_t)UTF8[i];
			i++;
		}
		if(Unicode[j]<0x20)
			Unicode[j]=0x20;
		j++;
		if(i>=(DLS_LEN-1))
			break;
	}
	if(j>=(DLS_LEN-1))
		Unicode[DLS_LEN-1]=0x00;
	else
		Unicode[j]=0x00;
}

int main()
{
	uint8_t xx=0;

	uint8_t utf8[256] = {0xE6,0xA2,0x81,0xE9,0x9D,0x99,0xE8,0x8C,0xB9};
	uint16_t unicode[256];
	
	UTF8ToUnicode(utf8,unicode);

	for(xx; xx<20; xx++)
	{
		printf("unicode[%d]:0x%x\n",xx,unicode[xx]);
	}
	return 0;
}