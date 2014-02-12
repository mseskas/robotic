#include <stdlib.h>
#include <wiringPi.h>

#include <wiringPiI2C.h>

#include <project_constants.h>

#ifndef	__Chip_PCA9685_H__
#define	__Chip_PCA9685_H__

class Chip_PCA9685
{
    public:
         ~Chip_PCA9685();
         Chip_PCA9685(int chip_addr);
         void set_pwm(int pwn_no, int on_tick, int off_tick);
         void set_pwm_freq(int frq_Hz);
         int  get_pwm_freq();
         int get_ticks();

    protected:
        int reset();
    private:
        int _ticks;
        int _work_frequence;
        int __SUBADR1;
        int __SUBADR2;
        int __SUBADR3;
        int __MODE1;
        int __PRESCALE;
        int __LED0_ON_L;
        int __LED0_ON_H;
        int __LED0_OFF_L;
        int __LED0_OFF_H;
        int __ALLLED_ON_L;
        int __ALLLED_ON_H;
        int __ALLLED_OFF_L;
        int __ALLLED_OFF_H;
        int _addr;
};

#endif
