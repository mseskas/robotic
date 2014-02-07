#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

int main (int argc, char * argv[] )
{

	wiringPiSetup ();
	pinMode (0, OUTPUT);

	int i = 501, direction = 100;
	
	if (argc > 1) i = atoi(argv[1]);

	printf("servo is running at %i us/20ms\n", i);

	while (1)
	{
		for (int j =0; j < 50; j++)
		{
			digitalWrite (0, HIGH);

			delayMicroseconds(i);

			digitalWrite (0, LOW);

			delayMicroseconds(20000 - i);
		}
		if ((i >=2000) || (i <= 500)) direction = - direction;
 		i += direction;
	}
	return 0 ;
}
