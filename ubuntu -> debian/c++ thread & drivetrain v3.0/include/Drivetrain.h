
#include <thread>
#include <iostream>

#include <Chip_PCA9685.h>

#include <project_constants.h>

class Drivetrain
{

    public :
        Drivetrain(Chip_PCA9685 * chip);
        ~Drivetrain();
        void turn(int direction, float angle_degree);
        int drive(float time_seconds, int  wheel_no);
        void set_speed(float speed);
        float get_speed();
        void whait_to_finish(int timeout_ms);
    private :
        Chip_PCA9685 * _chip_PCA9685;
        float _speed;  // from 0.0 to 1.0
        std::thread * execution;
        int wheel_left_forward;
        int wheel_left_backward;
        int wheel_right_forward;
        int wheel_right_backward;
};

