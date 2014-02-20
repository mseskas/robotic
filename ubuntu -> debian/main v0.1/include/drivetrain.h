
#include <thread>
#include <iostream>
#include <math.h>
#include <pwm_chip.h>

#include <project_constants.h>

#ifndef	__drivetrain__
#define	__drivetrain__

class drivetrain
{

    public :
        drivetrain(pwm_chip * chip_PCA9685);
        ~drivetrain();
        void a_turn(int direction, float time_seconds);
        void a_drive(float time_seconds, int  direction);
        void set_drive_speed(float speed);
        float get_drive_speed();
        void set_turn_speed(float speed);
        float get_turn_speed();
        void wait_to_finish(int timeout_ms);
        void force_stop();
    private :
        void drive(float time_seconds, float speed, float left_wheel_percentage,
                   float right_wheel_percentage);
        void stop();
        pwm_chip * _chip_PCA9685;
        float _drive_speed;  // from 0.0 to 1.0
        float _turn_speed;  // from 0.0 to 1.0
        std::thread * _execution_thread;
        int _wheel_left_forward;
        int _wheel_left_backward;
        int _wheel_right_forward;
        int _wheel_right_backward;
        bool _stop_execution;
        bool _is_executing;


};

#endif
