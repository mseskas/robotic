#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include <string>
#include <iostream>
#include <thread>

#include <project_constants.h>
#include <video processing/advanced_opencv.h>
#include <pwm_chip.h>
#include <drivetrain.h>
#include <servo.h>
#include <sonar.h>
#include <camera.h>

#include <ctime>

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
    void temp();
    void drive(int direction, float time);
    void turn(int direction, float time  );
protected:
private:
    advanced_opencv * _adv_opencv;
    sonar      * _sonar_front;
    pwm_chip   * _chip_16pwm;
    servo      * _servo_spare;
    drivetrain * _drv;
    camera     * _cam_front;
    thread     * _execution_thread;
    bool       _stop_execution;
    bool       _is_executing;
    char _key_on_hold;

private : // separate activities
    void dumb_drive();
    void show_front_distance();
    void show_front_view();
    void canny_edge_view();
    void hvs_view();
    void init_floor();
    void control_robot();
    void optical_flow(bool use_camera, String video_file_url);
    void record_video(bool show_video);
};

#endif // ACTIVITIES_H
