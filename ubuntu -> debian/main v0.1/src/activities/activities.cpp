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
        activities::dumb_drive();
    }
}

void activities::dumb_drive()
{
    while (true)
        {
            drv->a_drive(5, FORWARD);
            drv->wait_to_finish(0);
            drv->a_turn(TURN_RIGHT, 0.8);
            drv->wait_to_finish(0);
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
