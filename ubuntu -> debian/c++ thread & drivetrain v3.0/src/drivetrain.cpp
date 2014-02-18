#include "drivetrain.h"


using namespace std;

drivetrain::drivetrain(pwm_chip * chip_PCA9685)
{
    _chip_PCA9685 = chip_PCA9685;
    set_speed(0.12);
    _wheel_left_backward = PIN_WHEEL_LEFT_BACKWARD;
    _wheel_left_forward = PIN_WHEEL_LEFT_FORWARD;
    _wheel_right_forward = PIN_WHEEL_RIGHT_FORWARD;
    _wheel_right_backward = PIN_WHEEL_RIGHT_BACKWARD;
    _stop_execution = false;
}
drivetrain::~drivetrain()
{
    _chip_PCA9685->~pwm_chip();
}
void drivetrain::set_speed(float speed)
{
    if (speed < 0) speed = 0;
    if (speed > 1) speed = 1;
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

void drivetrain::drive(float time_seconds, int direction)
{
    if (direction == FORWARD)
    {
        _chip_PCA9685->set_pwm(_wheel_left_forward, 0, _chip_PCA9685->get_ticks()*get_speed() );
        _chip_PCA9685->set_pwm(_wheel_right_forward, 0, _chip_PCA9685->get_ticks()*get_speed() );
    }
    else
    {
        _chip_PCA9685->set_pwm(_wheel_left_backward, 0, _chip_PCA9685->get_ticks()*get_speed() );
        _chip_PCA9685->set_pwm(_wheel_right_backward, 0, _chip_PCA9685->get_ticks()*get_speed() );
    }

    for (float i = 0; i < time_seconds ; i += 0.01)
    {
        delay(10); // 10ms delay
        if (_stop_execution) break; // break loop
    }
    stop();
    _stop_execution = false;
}


void drivetrain::a_drive(float time_seconds, int direction)
{
     _execution_thread = new thread (&drivetrain::drive, this, time_seconds, direction);

}

void drivetrain::whait_to_finish(int timeout_ms)
{
    _execution_thread->join();
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
    whait_to_finish(1000);
    _chip_PCA9685->set_pwm(_wheel_left_backward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_left_forward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_right_backward, 0, 0 );
    _chip_PCA9685->set_pwm(_wheel_right_forward, 0, 0 );
    _stop_execution = false;
}
