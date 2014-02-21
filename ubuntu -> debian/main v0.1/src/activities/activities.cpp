#include "activities.h"

activities::activities()
{
    sonar_front = new sonar(PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);
    chip_16pwm = new pwm_chip (PWM_CHIP_ADDR);
    servo_spare = new servo (chip_16pwm, PIN_SERVO);
    drv = new drivetrain (chip_16pwm);
    cam_front = new camera (USB_FRONT_CAMERA_NO);

    sonar_front->set_drivetrain( drv);
}

activities::~activities()
{
    drv->~drivetrain();
    sonar_front->~sonar();
}

void activities::act(string activity_name)
{

    if (activity_name == "dumb drive")
    {
        force_stop();
        _execution_thread = new thread (&activities::dumb_drive, this);
        _is_executing = true;
    }
}

void activities::dumb_drive()
{
    while (true)
        {
            drv->a_drive(5, FORWARD);
                if (_stop_execution) break;
            drv->wait_to_finish(0);
                if (_stop_execution) break;
            drv->a_turn(TURN_RIGHT, 0.8);
                if (_stop_execution) break;
            drv->wait_to_finish(0);
                if (_stop_execution) break;
        }
        drv->force_stop();

        _stop_execution = false;
        _is_executing = false;
}

void activities::force_stop()
{
    _stop_execution = true;
    wait_to_finish(0);
    _stop_execution = false;
}

void activities::wait_to_finish(int timeout_ms)
{
    while (true)
    {
        if (!_is_executing ) break;
        delay(5);
    }
}


/*

    //
// ===================================================

   camera cam_front(USB_FRONT_CAMERA_NO);
    cvNamedWindow("cam");
    while (true)
    {
        IplImage * frame =  cam_front.get_frame();

        cvShowImage("cam", frame);
        cvWaitKey(1);
    }

*/
