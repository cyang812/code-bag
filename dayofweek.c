#include "stdio.h"

int dayofweek(int y, int m, int d) /* 0 = Sunday */
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

void main()
{
	int day = 0;
	day = dayofweek(2017,7,3);
	printf("%d\n",day);
}