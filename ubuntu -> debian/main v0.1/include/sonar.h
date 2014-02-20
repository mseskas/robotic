#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <thread>
#include <sys/time.h>

#include <drivetrain.h>
#include <project_constants.h>

class sonar
{
    public:  sonar(int pin_trigger, int pin_echo);
        ~sonar();
        int get_distance();
        int get_trigger();
        void set_trigger(int pin_trigger);
        int get_echo();
        void set_echo(int pin_echo);
        void set_drivetrain(drivetrain * drv);

    private: int trigger;
        int echo;
        int waitforpin(int pin_value, int timeout_uS);
        int _last_distance;
        std::thread * _execution_thread;
        drivetrain * _drv;
        void measure_distance();
        void constant_distance_measure();
};
