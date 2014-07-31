#include "servo.h"

servo::servo(pwm_chip * chip_PCA9685, int pin_no)
{
    _chip_PCA9685 = chip_PCA9685;
    _pin = pin_no;
    _chip_PCA9685->set_pwm(pin_no, 0, 0);
    _max_ticks = 675;
    _min_ticks = 150;
    _angle = 0;
}

servo::~servo()
{
    _chip_PCA9685->set_pwm(_pin, 0, 0);
}

void servo::set_angle(float new_angle)
{
    if ((new_angle >= 0.0) && (new_angle <= 1.0))
        _chip_PCA9685->set_pwm(_pin, 0, _min_ticks + ( (_max_ticks - _min_ticks) * new_angle) );
}

void servo::turn(float angle)
{
    _angle += angle;
    if (_angle < 0.0) _angle = 0.0;

    if (_angle > 1.0) _angle = 1.0;
    set_angle(_angle);
}


float servo::get_angle()
{
    return _angle;
}



