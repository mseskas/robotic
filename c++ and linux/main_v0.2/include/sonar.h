#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <thread>
#include <sys/time.h>

#include <drivetrain.h>
#include <project_constants.h>

#include <gui_display.h>

class sonar
{
public:
    sonar(int pin_trigger, int pin_echo);
    sonar(bool virtual_mode);
    ~sonar();
    int get_distance();
    int get_trigger();
    void set_trigger(int pin_trigger);
    int get_echo();
    void set_echo(int pin_echo);
    void set_drivetrain(drivetrain * drv);
    void force_stop();
    void wait_to_finish(int timeout_ms);
    void set_displ_gui(gui_display * gui, int label_index);
private:
    gui_display * _gui_disp;
    int _gui_label_index;
    int trigger;
    int echo;
    int waitforpin(int pin_value, int timeout_uS);
    int _last_distance;
    std::thread * _execution_thread;
    drivetrain * _drv;
    void measure_distance();
    void constant_distance_measure();
    void virt_constant_distance_measure();
    void stop();
    bool _stop_execution;
    bool _is_executing;
};
