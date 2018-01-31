#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
	/* code */
	uint16_t a = 0x0108;

	/*correct*/
	printf("0x%02x\n",(uint8_t)(a>>8));
	printf("0x%02x\n",(uint8_t)a );

	/*error*/
	printf("0x%02x\n",(uint8_t)a>>8 );
	printf("0x%02x\n",(uint8_t)a );

	return 0;
}