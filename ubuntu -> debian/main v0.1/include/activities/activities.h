
#include <string>
#include <iostream>

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
        void  act(string activity_name);
    protected:
    private:
    sonar      * sonar_front;
    pwm_chip   * chip_16pwm;
    servo      * servo_spare;
    drivetrain * drv;
    camera     * cam_front;

    void dumb_drive();

};
