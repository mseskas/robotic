#include "servo.h"

servo::servo(pwm_chip * chip_PCA9685, int pin_no)
{
    _chip_PCA9685 = chip_PCA9685;
    _pin = pin_no;
    _chip_PCA9685->set_pwm(pin_no, 0, 0);
    _max_angle = 180;
    _max_ticks = (0.002 /(1.0 / (float)_chip_PCA9685->get_pwm_freq())) *  _chip_PCA9685->get_ticks();
    _min_ticks = _max_ticks / 2;

}

servo::~servo()
{
    _chip_PCA9685->set_pwm(_pin, 0, 0);
}

void servo::set_angle(float new_angle)
{

}

 void servo::turn(int angle)
{
    set_angle(_angle + angle);
}


float servo::get_angle()
{
    return _angle;
}

float servo::get_max_angle()
{
    return _max_angle;
}


