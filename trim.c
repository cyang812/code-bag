/*
* 去除字符串首尾空格
* Trim 不会改变原字符串
* Trim2 会改变原字符串
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t *Trim(uint8_t *strVal)
{
	uint8_t *strptr2, *strptr3;

	if(strVal == 0)
		return 0;
	if(strlen((const char *)strVal) == 0)
		return strVal;

	strptr2 = strVal;
	while(strptr2 != 0 && *strptr2 == ' ')
		strptr2++;

	strptr3 = strptr2 + strlen((const char *)strptr2) -1;
	while(*strptr3 == ' ' && strptr2 != strptr3)
			strptr3--;
	*(strptr3+1) = '0';

	printf("strptr2:%s,strlen:%d\n",strptr2,strlen(strptr2));
	printf("strptr3:%s,strlen:%d\n",strptr3,strlen(strptr3));
	return strptr2;
}


void Trim2(uint8_t *str)
{
	int index=0,i=0;

	//去除串首空格
	while(str[index]==' ') index++;
	for(i=0; i<strlen(str)-index; i++) str[i] = str[i+index];
	str[i]='\0';

	//去除串尾空格
	index= strlen(str);
	while(index>0 && str[index-1]==' ')  index--;
	str[index]='\0';
}

int main()
{
	char str1[50] = "    hello world!    "; // test str
	char *str2;

	printf("str1:%s,strlen:%d\n",str1,strlen(str1));
	//str2 = Trim(str1);
	//printf("str2:%s,strlen:%d\n",str2,strlen(str2));

	Trim2(str1);
	printf("str1:%s,strlen:%d\n",str1,strlen(str1));

	return 0;
}
