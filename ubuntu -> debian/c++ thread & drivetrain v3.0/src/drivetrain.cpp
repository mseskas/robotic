#include "drivetrain.h"


using namespace std;

drivetrain::drivetrain(pwm_chip * chip_PCA9685)
{
    _chip_PCA9685 = chip_PCA9685;
    _chip_PCA9685->set_pwm_freq(50);
    set_speed(0.2);
}
drivetrain::~drivetrain()
{
    _chip_PCA9685->~pwm_chip();
}
void drivetrain::set_speed(float speed)
{
    _speed = speed;
}
float drivetrain::get_speed()
{
    return _speed;
}

void drivetrain::turn(int direction, float angle_degree)
{
    cout << "drivetrain::turn (" << direction << ", " << angle_degree << ") - not implemented" << endl;
}

int drivetrain::drive(float time_seconds, int direction)
{
    int left_wheel_direction, right_wheel_direction;
    if (direction == FORWARD)
    {
        left_wheel_direction = 1;
        right_wheel_direction = 2;
    }
    else
    {
        left_wheel_direction = 0;
        right_wheel_direction = 3;
    }

    for (int i = 0; i < time_seconds * 1000; i += 100)
    {
        _chip_PCA9685->set_pwm(left_wheel_direction, 0, _chip_PCA9685->get_ticks()*get_speed() );
        _chip_PCA9685->set_pwm(right_wheel_direction, 0, _chip_PCA9685->get_ticks()*get_speed() );
        delay(100);
    }
    _chip_PCA9685->set_pwm(left_wheel_direction, 0, 0 );
    _chip_PCA9685->set_pwm(right_wheel_direction, 0, 0 );

    return 0;
}

void drivetrain::whait_to_finish(int timeout_ms)
{
    execution->join();
}

