#include "drivetrain.h"


using namespace std;

drivetrain::drivetrain()
{
    chip = new pwm_chip(0x40);
    chip->set_pwm_freq(50);
    set_speed(0.2);
}
drivetrain::~drivetrain()
{
    chip->~pwm_chip();
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

int drivetrain::drive(int distance_cm, int direction)
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

    for (int i = 0; i < distance_cm; i++)
    {
       chip->set_pwm(left_wheel_direction, 0, chip->get_ticks()*get_speed() );
       chip->set_pwm(right_wheel_direction, 0, chip->get_ticks()*get_speed() );
        //chip->set_pwm(2, 0, chip->get_ticks()*get_speed() );
        delay(100);
    }
    chip->set_pwm(left_wheel_direction, 0, 0 );
    chip->set_pwm(right_wheel_direction, 0, 0 );

    return 0;
}

void drivetrain::whait_to_finish(int timeout_ms)
{
    execution->join();
}

