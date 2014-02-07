
#include <thread>
#include <iostream>

#include <pwm_chip.h>

#define DIRECTION_LEFT  0
#define DIRECTION_RIGHT 1

class drivetrain
{

    public :
        drivetrain();
        ~drivetrain();
        void turn(int direction, float angle_degree);
        int drive(int distance_cm);
        void set_speed(float speed);
        float get_speed();
        void whait_to_finish(int timeout_ms);
    private :
        pwm_chip * chip;
        float _speed;  // from 0.0 to 1.0
        std::thread * execution;
};

