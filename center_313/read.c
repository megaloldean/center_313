#include <stdio.h>

#include "center313.h"

void main(void)
{
	printf("Start reading\n");
	
	if (-1 == center313_init("/dev/ttyUSB0"))
	{
		printf("center313 init error\n");
		return;
	}
	
	float temper = 0.0f;
	float humidity = 0.0f;
	
	while (1)
	{
		if (-1 == center313_getValues(&temper, &humidity))
		{
			printf("center313_getValues error\n");
			break;
		}
		printf("%f %f\n", temper, humidity);
	}
	
	center313_close();
}