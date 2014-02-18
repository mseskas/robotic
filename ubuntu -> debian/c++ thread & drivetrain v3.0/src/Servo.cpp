#include "Servo.h"

Servo::Servo(Chip_PCA9685 * chip, int pin_no)
{
    _chip_PCA9685 = chip;
    _pin = pin_no;
    _chip_PCA9685->set_pwm(pin_no, 0, 0);
    _max_angle = 180;
    _max_ticks = (0.002 /(1.0 / (float)_chip_PCA9685->get_pwm_freq())) *  _chip_PCA9685->get_ticks();
    _min_ticks = _max_ticks / 2;

}

Servo::~Servo()
{
    _chip_PCA9685->set_pwm(_pin, 0, 0);
}

void Servo::set_angle(float new_angle)
{

}

 void Servo::turn(int angle)
{
    set_angle(_angle + angle);
}


float Servo::get_angle()
{
    return _angle;
}

float Servo::get_max_angle()
{
    return _max_angle;
}


