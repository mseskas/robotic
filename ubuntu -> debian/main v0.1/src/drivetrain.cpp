#include "drivetrain.h"


using namespace std;

drivetrain::drivetrain(pwm_chip * chip_PCA9685)
{
    _chip_PCA9685 = chip_PCA9685;
    set_drive_speed(default_drivetrain_drive_speed);
    set_turn_speed(default_drivetrain_turn_speed);
    _wheel_left_backward = PIN_WHEEL_LEFT_BACKWARD;
    _wheel_left_forward = PIN_WHEEL_LEFT_FORWARD;
    _wheel_right_forward = PIN_WHEEL_RIGHT_FORWARD;
    _wheel_right_backward = PIN_WHEEL_RIGHT_BACKWARD;
    _stop_execution = false;
    _is_executing = false;
}

drivetrain::~drivetrain()
{
    force_stop();
    _chip_PCA9685->~pwm_chip();
}

void drivetrain::set_drive_speed(float speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1) speed = 1;
    _drive_speed = speed;
}

float drivetrain::get_drive_speed()
{
    return _drive_speed;
}

void drivetrain::set_turn_speed(float speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1) speed = 1;
    _turn_speed = speed;
}

float drivetrain::get_turn_speed()
{
    return _turn_speed;
}

void drivetrain::a_turn(int direction, float time_seconds)
{
    force_stop();
    if (direction == TURN_LEFT)
    {
        _execution_thread = new thread (&drivetrain::drive, this, time_seconds, get_turn_speed(), -1, 1);
    }
    else
    {
        _execution_thread = new thread (&drivetrain::drive, this, time_seconds, get_turn_speed(), 1, -1);
    }
    _is_executing = true;
}

void drivetrain::drive(float time_seconds, float speed, float left_wheel_percentage, float right_wheel_percentage)
{
    if (left_wheel_percentage != 0)
    {
        if (left_wheel_percentage > 0)
        {
            _chip_PCA9685->set_pwm(_wheel_left_forward, 0,
                                   _chip_PCA9685->get_ticks()*speed * left_wheel_percentage );
        }
        else
        {
            _chip_PCA9685->set_pwm(_wheel_left_backward, 0,
                                    _chip_PCA9685->get_ticks()*speed * fabs(left_wheel_percentage));
        }
    }

    if (right_wheel_percentage != 0)
    {
        if (right_wheel_percentage > 0)
        {
            _chip_PCA9685->set_pwm(_wheel_right_forward, 0,
                                   _chip_PCA9685->get_ticks()*speed * right_wheel_percentage );
        }
        else
        {
            _chip_PCA9685->set_pwm(_wheel_right_backward, 0,
                                    _chip_PCA9685->get_ticks()*speed * fabs(right_wheel_percentage));
        }
    }

    for (float i = 0; i < time_seconds ; i += 0.01)
    {
        delay(10); // 10ms delay
        if (_stop_execution) break; // break loop
    }
    stop();
    _stop_execution = false;
    _is_executing = false;
}



void drivetrain::a_drive(float time_seconds, int direction)
{
    force_stop();
    if (direction == FORWARD)
    {
        _execution_thread = new thread (&drivetrain::drive, this, time_seconds, get_drive_speed(), 1, 1);
    }
    else
    {
        _execution_thread = new thread (&drivetrain::drive, this, time_seconds, get_drive_speed(), -1, -1);
    }
    _is_executing = true;
}

void drivetrain::wait_to_finish(int timeout_ms)
{
    while (true)
    {
        if (!_is_executing ) break;
        delay(5);
    }
}

void drivetrain::stop()
{
    _chip_PCA9685->set_pwm(_wheel_left_backward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_left_forward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_right_backward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_right_forward, 0, 0 );
}

void drivetrain::force_stop()
{
    _stop_execution = true;
    wait_to_finish(0);
    _stop_execution = false;
}
