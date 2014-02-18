#include "Drivetrain.h"


using namespace std;

Drivetrain::Drivetrain(Chip_PCA9685 * chip)
{
    _chip_PCA9685 = chip;
    set_speed(0.2);
    wheel_left_forward = PIN_WHEEL_LEFT_FORWARD;
    wheel_left_backward = PIN_WHEEL_LEFT_BACKWARD;
    wheel_right_forward = PIN_WHEEL_RIGHT_FORWARD;
    wheel_right_backward = PIN_WHEEL_RIGHT_BACKWARD;
}
Drivetrain::~Drivetrain()
{
    _chip_PCA9685->~Chip_PCA9685();
}
void Drivetrain::set_speed(float speed)
{
    _speed = speed;
}
float Drivetrain::get_speed()
{
    return _speed;
}

void Drivetrain::turn(int direction, float angle_degree)
{
    cout << "Drivetrain::turn (" << direction << ", " << angle_degree << ") - not implemented" << endl;
}

int Drivetrain::drive(float time_seconds, int direction)
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

void Drivetrain::whait_to_finish(int timeout_ms)
{
    execution->join();
}

