#include "drivetrain.h"


using namespace std;

drivetrain::drivetrain()
{
    chip = new pwm_chip(0x40);
    chip->set_pwm_freq(50);
    set_speed(9);
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

int drivetrain::drive(int distance_cm)
{
    chip->set_pwm(0, 0, chip->get_ticks()*get_speed() );
    chip->set_pwm(2, 0, chip->get_ticks()*get_speed() );


    cout << "drivetrain::drive (" << distance_cm << ") - not implemented" << endl;
    return 0;
}

void drivetrain::whait_to_finish(int timeout_ms)
{
    execution->join();
}

