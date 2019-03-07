/*
* @Author: cyang
* @Date:   2019-03-01 10:37:13
* @Last Modified by:   cyang
* @Last Modified time: 2019-03-01 11:09:22
*/

#include "user_sls.h"
#include "silan_printf.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BMP160_120   0
#define BMP280_210   1

typedef struct 
{
	uint8_t *data;
	uint16_t height;
	uint16_t width;
}NEW_SIZE_BMP;

static void encodeToBMP(uint8_t *bmp_header, uint32_t xsize, uint32_t ysize)
{
	uint8_t header[54] =
	{
    	0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    
    uint32_t file_size = (uint32_t)xsize * (uint32_t)ysize * 3 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    uint32_t width = xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    uint32_t height = ysize;
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

	memmove(bmp_header, header, sizeof(header));
}

/*
 * convert rgb888 to rgb565
 * color_src:     bmp[54], skip bmp header
 * color_src_len: height * width * 3
 * return:        height * width * 2
 */
static uint32_t convertToRGB565(uint8_t *color_src, uint32_t color_src_len)
{
	uint16_t n565Color = 0;
	uint8_t r, g, b;
	uint32_t idx = 0;
	uint16_t *out_src = (uint16_t *)color_src; 

	uint32_t i = 0;
	for(i = 0; i < color_src_len; i += 3)
	{
		r = color_src[i];
		g = color_src[i + 1];
		b = color_src[i + 2];
		
		n565Color = (uint16_t)((((uint32_t)(r) << 8) & 0xf800) |   
 	          		    	   (((uint32_t)(g) << 3) & 0x07E0) |  
	    	       				 (((uint32_t)(b) >> 3)));  

	    out_src[idx++] = n565Color; 	        
	}

	return idx - 1; 
}

void bmp_shrink_1(NEW_SIZE_BMP *new_bmp, const uint8_t *raw_data)
{
	uint8_t *pColorDataMid = new_bmp->data;
	const uint8_t *pColorData = raw_data;

	uint32_t idx = 0;
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 320*3; j+=3)
		{
			if(i%2 && j%(2*3))
			{
				pColorDataMid[idx++] = pColorData[i*3*320 + j+0];
				pColorDataMid[idx++] = pColorData[i*3*320 + j+1];
				pColorDataMid[idx++] = pColorData[i*3*320 + j+2];
			}
		}
	}
}

void bmp_shrink_2(NEW_SIZE_BMP *new_bmp, const uint8_t *raw_data)
{
	uint8_t *pColorDataMid = new_bmp->data;
	const uint8_t *pColorData = raw_data;

	uint32_t idx = 0;
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 320*3; j+=3)
		{
			if(i%8 && j%(8*3))
			{
				pColorDataMid[idx++] = pColorData[i*3*320 + j+0];
				pColorDataMid[idx++] = pColorData[i*3*320 + j+1];
				pColorDataMid[idx++] = pColorData[i*3*320 + j+2];
			}
		}
	}
}

uint32_t BMP_resize(uint8_t resize_type, uint8_t *new_src, const uint8_t *raw_data)
{
	NEW_SIZE_BMP new_bmp;
	new_bmp.data = &new_src[54]; //for bmp header

	//bmp shrink
	if(resize_type == BMP160_120)
	{
		new_bmp.height = 120;
		new_bmp.width = 160;
		bmp_shrink_1(&new_bmp, raw_data);
	}
	else //BMP280_210
	{
		new_bmp.height = 210;
		new_bmp.width = 280;
		bmp_shrink_2(&new_bmp, raw_data);
	}

	//bmp encode
	encodeToBMP(new_src, new_bmp.width, new_bmp.height);

	//convert rgb565
	uint32_t convert_size = 0;
	convert_size = convertToRGB565(new_bmp.data, new_bmp.width * new_bmp.height * 3);

	return convert_size;
}

//module test
#if 0
int main(int argc, char const *argv[])
{
	FILE *pfile, *wfile;

	char strFile[50] = "06.bmp";//打开图像路径，BMP图像必须为24位真彩色格式  
	char strFilesave[50] = "16.bmp";//处理后图像存储路径  
	pfile = fopen(strFile, "rb");//文件打开图像  
	wfile = fopen(strFilesave, "wb");//打开文件为存储修改后图像做准备  
									   //读取位图文件头信息  

	char bmp_header[54];

	char *raw_data = (char *)malloc(320*240*3);
	if(raw_data == NULL)
	{
		printf("malloc for raw data fail\n");
		return 0;
	}
	else
	{
		fread(bmp_header, 1, sizeof(bmp_header), pfile);
		fread(raw_data, 1, 320*240*3, pfile);
	}

	NEW_SIZE_BMP new_bmp;
	new_bmp.height = 120;
	new_bmp.width = 160;
	new_bmp.data = (uint8_t *)malloc(new_bmp.height * new_bmp.width * 3);
	if(new_bmp.data == NULL)
	{
		printf("malloc fail\n");
		return 0;
	}

	encodeToBMP((uint8_t *)bmp_header, new_bmp.width, new_bmp.height);

	fwrite(bmp_header, 1, sizeof(bmp_header), wfile);

	bmp_shrink(&new_bmp, (const uint8_t *)raw_data);

	fwrite(new_bmp.data, 1, new_bmp.height * new_bmp.width * 3, wfile);

	if(raw_data)
	{
		free(raw_data);
		raw_data = NULL;
	}

	if(new_bmp.data)
	{
		free(new_bmp.data);
		new_bmp.data = NULL;
	}
	fclose(pfile);
	fclose(wfile);
	return 0;
}
#endif
