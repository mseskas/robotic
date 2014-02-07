#include "pwm_chip.h"

pwm_chip::pwm_chip(int chip_addr)
{
    ticks = 4095;
    __SUBADR1      = 0x02;
    __SUBADR3      = 0x04;
    __MODE1        = 0x00;
    __PRESCALE     = 0xFE;
    __LED0_ON_L    = 0x06;
    __LED0_ON_H    = 0x07;
    __LED0_OFF_L   = 0x08;
    __LED0_OFF_H   = 0x09;
    __ALLLED_ON_L  = 0xFA;
    __ALLLED_ON_H  = 0xFB;
    __ALLLED_OFF_L = 0xFC;
    __ALLLED_OFF_H = 0xFD;
	
    wiringPiSetup ();
    addr = wiringPiI2CSetup (chip_addr) ;
    reset();
}

pwm_chip::~pwm_chip()
{
    reset();
}

int pwm_chip::reset()
{
    return wiringPiI2CWriteReg8 (addr, 0x00, 0x00) ;
}

void pwm_chip::set_pwm(int pwn_no, int on_tick, int off_tick)
{
    wiringPiI2CWriteReg8(addr, __LED0_ON_L + 4 * pwn_no, on_tick & 0xFF);
    wiringPiI2CWriteReg8(addr, __LED0_ON_H + 4 * pwn_no, on_tick >> 8);
    wiringPiI2CWriteReg8(addr, __LED0_OFF_L + 4 * pwn_no, off_tick & 0xFF);
    wiringPiI2CWriteReg8(addr, __LED0_OFF_H + 4 * pwn_no, off_tick >> 8);
}

// 40 and 1000 Hz
void pwm_chip::set_pwm_freq(int frq_Hz)
{
    float prescaleval = 25000000.0;   // 25MHz
    prescaleval /= 4096.0;     // 12-bit
    prescaleval /= (float)frq_Hz;
    prescaleval -= 1.0;
    int prescale = (int)(prescaleval+ 0.5);
    int oldmode = wiringPiI2CReadReg8(addr,__MODE1);
    int newmode = (oldmode & 0x7F) | 0x10;  // 4 bit is sleep mode
    wiringPiI2CWriteReg8(addr, __MODE1, newmode); // go to sleep
    wiringPiI2CWriteReg8(addr, __PRESCALE, prescale);
    wiringPiI2CWriteReg8(addr, __MODE1, oldmode);
    delayMicroseconds(500);  // 500 us to stabilize oscillator
    wiringPiI2CWriteReg8(addr, __MODE1, oldmode | 0x80);

}


