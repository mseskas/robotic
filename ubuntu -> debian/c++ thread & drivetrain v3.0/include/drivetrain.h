
#include <thread>
#include <iostream>

#include <pwm_chip.h>

#include <project_constants.h>

class drivetrain
{

    public :
        drivetrain(pwm_chip * chip_PCA9685);
        ~drivetrain();
        void turn(int direction, float angle_degree);
        void a_drive(float time_seconds, int  wheel_no);
        void set_speed(float speed);
        float get_speed();
        void whait_to_finish(int timeout_ms);
        void force_stop();
    private :
        void drive(float time_seconds, int  wheel_no);
        void stop();
        pwm_chip * _chip_PCA9685;
        float _speed;  // from 0.0 to 1.0
        std::thread * _execution_thread;
        int _wheel_left_forward;
        int _wheel_left_backward;
        int _wheel_right_forward;
        int _wheel_right_backward;
        bool _stop_execution;
        void constant_distance_measure();

};

