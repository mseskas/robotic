#include <stdio.h>
#include <stdlib.h>

#include "pwm_chip.cpp"

int main (int argc, char * argv[] )
{
    int chip_pwm_id = 0x40;
    pwm_chip chip(chip_pwm_id);
    chip.set_pwm_freq(60);

    while (true)
    {
        chip.set_pwm(0, 0, 150);//chip.ticks/20);  // 4096 ~ 20ms, 4096/20 ~ 1ms - 0 degree
	printf("set to 150");
        delay(2000);
        chip.set_pwm(0, 0, 600);//chip.ticks/10);  // 4096 ~ 20ms, 4096/10 ~ 2ms - 180 degree
	printf("set to 600");
        delay(2000);
    }

    return 0;
}
