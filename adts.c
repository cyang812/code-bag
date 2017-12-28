#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct adts_fixed_header 
{  
 unsigned int syncword:12;//同步字0xfff，说明一个ADTS帧的开始
 unsigned char ID:1;//ID比较奇怪,标准文档中是这么说的”MPEG identifier, set to ‘1’. See ISO/IEC 11172-3″,但我写0了,也可以
 unsigned char layer:2;//一般设置为0
 unsigned char protection_absent:1;//是否误码校验
 unsigned char profile:2;//表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AACLC
 unsigned char sampling_frequency_index:4;//表示使用的采样率下标0x3 48k ,0x4 44.1k, 0x5 32k
 unsigned char private_bit:1;//一般设置为0
 unsigned char channel_configuration:3;// 表示声道数
 unsigned char original_copy:1;//一般设置为0
 unsigned char home:1;//一般设置为0
}adts_fixed;


typedef struct adts_variable_header
{  
 unsigned char copyright_identification_bit:1;//一般设置为0
 unsigned char copyright_identification_start:1;//一般设置为0
 unsigned int frame_length:13;// 一个ADTS帧的长度包括ADTS头和raw data block
 unsigned int adts_buffer_fullness:11;// 0x7FF 说明是码率可变的码流
 unsigned char number_of_raw_data_blocks_in_frame:2;//表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.
}adts_variable;

static int adts_v(void);

unsigned char adts_t[25];
unsigned char adts[7]={0xff,0xf9,0x60,0x40,0x10,0x1f,0xfc};
unsigned char adts_s[7];

static int adts_v(void)
{
adts_fixed *fixed;
adts_variable *variable;
fixed = (adts_fixed *)malloc(sizeof(adts_fixed) *1);
variable = (adts_variable *)malloc(sizeof(adts_variable) *1);


fixed->syncword = ((adts[0]<<4) |(adts[1]>>4));
fixed->ID = ((adts[1]>>3)&0x1);
fixed->layer = ((adts[1]>>1)&0x3);
fixed->protection_absent = ((adts[1])&0x1);
fixed->profile = ((adts[2]>>6)&0x3);
fixed->sampling_frequency_index = ((adts[2]>>2)&0xF);
fixed->private_bit = ((adts[2]>>1)&0x1);
fixed->channel_configuration =(((adts[2]&0x1)<<2)|((adts[3]>>6)&0x3)); 
fixed->original_copy = ((adts[3]>>5)&0x1);
fixed->home = ((adts[3]>>4)&0x1);

variable->copyright_identification_bit =((adts[3]>>3)&0x1);
variable->copyright_identification_start =((adts[3]>>2)&0x1);
variable->frame_length =((((adts[3])&0x3)<<11)|(adts[4])<<3|((adts[5]>>5)&0x7));
variable->adts_buffer_fullness =(((adts[5]&0x1F)<<6)|((adts[6]>>2)&0x3F));
variable->number_of_raw_data_blocks_in_frame =(adts[6]&0x3);



printf("***********fixed*******************************\n");

printf("fixed->syncword = 0x%x\n",fixed->syncword);
printf("fixed->ID = 0x%x\n",fixed->ID);
printf("fixed->layer = 0x%x\n",fixed->layer);
printf("fixed->protection_absent = 0x%x\n",fixed->protection_absent);
printf("fixed->profile = 0x%x\n",fixed->profile);
printf("fixed->sampling_frequency_index = 0x%x\n",fixed->sampling_frequency_index);
printf("fixed->private_bit = 0x%x\n",fixed->private_bit);
printf("fixed->channel_configuration = 0x%x\n",fixed->channel_configuration);
printf("fixed->original_copy = 0x%x\n",fixed->original_copy);
printf("fixed->home = 0x%x\n",fixed->home);

printf("***********variable*******************************\n");

printf("variable->copyright_identification_bit = 0x%x\n",variable->copyright_identification_bit);
printf("variable->copyright_identification_start = 0x%x\n",variable->copyright_identification_start);
printf("variable->frame_length = 0x%x\n",variable->frame_length);
printf("variable->adts_buffer_fullness = 0x%x\n",variable->adts_buffer_fullness);
printf("variable->number_of_raw_data_blocks_in_frame = 0x%x\n",variable->number_of_raw_data_blocks_in_frame);

return 0;
}

static int set_adts_v(int sample,int channel,int len)
{
adts_fixed *fixed;
adts_variable *variable;
memset(adts_s,0,7);
fixed = (adts_fixed *)malloc(sizeof(adts_fixed) *1);
variable = (adts_variable *)malloc(sizeof(adts_variable) *1);


fixed->syncword = 0xfff;
fixed->ID = 0x1;
fixed->layer =0x0;
fixed->protection_absent =0x1;
fixed->profile = 0x1;
fixed->sampling_frequency_index =0x08 ;//sample;
fixed->private_bit = 0x0;
fixed->channel_configuration =0x1;//channel; 
fixed->original_copy = 0x0;
fixed->home = 0x0;

variable->copyright_identification_bit =0x0;
variable->copyright_identification_start =0x0;
variable->frame_length =0x80;//len;
variable->adts_buffer_fullness =0x7ff;
variable->number_of_raw_data_blocks_in_frame =0x0;
adts_s[0] = (fixed->syncword>>4);
adts_s[1] = (((fixed->syncword & 0xF)<<4)|(fixed->ID<<3)|(fixed->layer<<1)|(fixed->protection_absent));
adts_s[2] = ((fixed->profile<<6)|(fixed->sampling_frequency_index<<2)|(fixed->private_bit<<1)|((fixed->channel_configuration>>2)&0x1));
adts_s[3] = (((fixed->channel_configuration&0x3)<<6)|(fixed->original_copy<<5)|(fixed->home<<4)|(variable->copyright_identification_bit<<3)|(variable->copyright_identification_start<<2)|((variable->frame_length>>11)&0x3));
adts_s[4] =((variable->frame_length>>3)&0xff);
adts_s[5] = (((variable->frame_length&0x7)<<5)|((variable->adts_buffer_fullness>>6)&0x1f));
adts_s[6] = (((variable->adts_buffer_fullness&0x3F)<<2)|(variable->number_of_raw_data_blocks_in_frame));
printf(" 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",adts_s[0],adts_s[1],adts_s[2],adts_s[3],adts_s[4],adts_s[5],adts_s[6]);

printf("***********fixed*******************************\n");

printf("fixed->syncword = 0x%x\n",fixed->syncword);
printf("fixed->ID = 0x%x\n",fixed->ID);
printf("fixed->layer = 0x%x\n",fixed->layer);
printf("fixed->protection_absent = 0x%x\n",fixed->protection_absent);
printf("fixed->profile = 0x%x\n",fixed->profile);
printf("fixed->sampling_frequency_index = 0x%x\n",fixed->sampling_frequency_index);
printf("fixed->private_bit = 0x%x\n",fixed->private_bit);
printf("fixed->channel_configuration = 0x%x\n",fixed->channel_configuration);
printf("fixed->original_copy = 0x%x\n",fixed->original_copy);
printf("fixed->home = 0x%x\n",fixed->home);

printf("***********variable*******************************\n");

printf("variable->copyright_identification_bit = 0x%x\n",variable->copyright_identification_bit);
printf("variable->copyright_identification_start = 0x%x\n",variable->copyright_identification_start);
printf("variable->frame_length = 0x%x\n",variable->frame_length);
printf("variable->adts_buffer_fullness = 0x%x\n",variable->adts_buffer_fullness);
printf("variable->number_of_raw_data_blocks_in_frame = 0x%x\n",variable->number_of_raw_data_blocks_in_frame);

return 0;
}

int main(int argv, char *argc[])
{
	int i;
	set_adts_v(1,1,1);
	memset(adts_t,0x0,25);
	memset(adts_t,0xff,7);
	memcpy(adts_t+7,adts_t,7);
	memcpy(adts_t,adts_s,7);
	for( i=0;i<25;i++)
	{
 		printf("adts_t[%d] = 0x%x\n",i,adts_t[i]);
	}
	adts_v();

	printf("asd!\n");

	return 0;
}

