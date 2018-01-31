#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
	/* code */
	int a[10], i;

	/* 死循环 
	* i的地址在数组a 后面，
	* 数组的越界操作导致i 被重置为0
	*/
	for(i=1; i<=10; i++)
	{
		a[i] = 0;
	}
	printf("%d\n", i);

	return 0;
}