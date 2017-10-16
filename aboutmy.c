#include <stdio.h>

#define QUARTER   3  //quarter month
#define MIN_MON   1  //January
#define MAX_MON   12 //December
#define BEGIN_MON 7  //the month that begin to work

void get_CombinationAndPrize(int min,int max,int quarter,int begin)
{
	int x,y,z;
	int idx = 0;
	int res = 0; 

	for(int i=min; i<=quarter; i++)
	{
		printf("combination_idx:%02d\n",++idx);
		for(int j=i,q=0; q<max/quarter; j+=3,q++)
		{
			x = j;
			y = j+1;
			z = j+2;
			if(y > max)
				y -= max;
			if(z > max)
				z -= max;

			printf("Q%d:{%02d,%02d,%02d}\n",q+1,x,y,z);

			if(x == begin)
				res = begin + 3;
			else if(x == begin + 1)
				res = begin + 4;	
			else if(x == begin + 2)
				res = begin + 5;
		}
		printf("prize_month : %d\n", res);
		printf("\n");
	}

}

int main(int argc, char const *argv[])
{
	get_CombinationAndPrize(MIN_MON,MAX_MON,QUARTER,BEGIN_MON);
	
	return 0;
}