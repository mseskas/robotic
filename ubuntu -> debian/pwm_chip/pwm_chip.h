#include <stdlib.h>
#include <wiringPi.h>

#include <wiringPiI2C.h>

class pwm_chip
{
    public:
        pwm_chip(int chip_addr);
        ~pwm_chip();
         void set_pwm(int pwn_no, int on_tick, int off_tick);
         void set_pwm_freq(int frq_Hz);
         int ticks;
    protected:
        int reset();
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
    private:
        int addr;
};
