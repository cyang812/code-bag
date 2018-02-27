/*
* cyang 2018/2/27
* mcu printf float value
*/

#include <stdio.h>

void printf_float(float a)
{
	char tmp[8]={0};
	int i;
	sprintf(tmp, "%f", a);
	for(i=0; i<8; i++)
		printf("%c", tmp[i]);
	printf("\n");
}

void PrintFloat(float value)
{
    int tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;
    tmp = (int)value;
    tmp1=(int)((value-tmp)*10)%10;
    tmp2=(int)((value-tmp)*100)%10;
    tmp3=(int)((value-tmp)*1000)%10;
    tmp4=(int)((value-tmp)*10000)%10;
    tmp5=(int)((value-tmp)*100000)%10;
    tmp6=(int)((value-tmp)*1000000)%10;
    printf("f-value=%d.%d%d%d%d%d%d\r\n",tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6);
}

int main(int argc, char const *argv[])
{
	/* code */
	float a = 2.354954;
	printf("a = %f\n", a);

	printf_float(a);
	PrintFloat(a);

	return 0;
}