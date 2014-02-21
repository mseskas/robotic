
#include <string>
#include <iostream>
#include <thread>

#include <project_constants.h>
#include <pwm_chip.h>
#include <drivetrain.h>
#include <servo.h>
#include <sonar.h>
#include <camera.h>

using namespace std;

class activities
{
    public:
        activities();
        ~activities();
        void  act(int activity_no);
        void force_stop();
        void wait_to_finish(int timeout_ms);
        void print_activities();
    protected:
    private:
    sonar      * sonar_front;
    pwm_chip   * chip_16pwm;
    servo      * servo_spare;
    drivetrain * drv;
    camera     * cam_front;
    thread     * _execution_thread;
    bool       _stop_execution;
    bool       _is_executing;
    void       dumb_drive();
    void       show_front_distance();
    void       show_front_view();
};
