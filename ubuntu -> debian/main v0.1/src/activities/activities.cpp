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
    force_stop();
    drv->~drivetrain();
    sonar_front->~sonar();
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

void activities::act(int activity_no)
{
    switch (activity_no)
    {
        case 1 : // stop everything
            force_stop();
        break;

        case 2 : // show distance to front
            force_stop();
            _execution_thread = new thread (&activities::show_front_distance, this);
            _is_executing = true;
        break;

        case 3 : // show front view
            force_stop();
            _execution_thread = new thread (&activities::show_front_view, this);
            _is_executing = true;
        break;

        case 4 : // dumb drive
            force_stop();
            _execution_thread = new thread (&activities::dumb_drive, this);
            _is_executing = true;
        break;

        default :
            cout << "activities::act() - unknown activity" << endl;
        break;
    }

}

void activities::print_activities()
{
    cout << "Robot Jerry at your service..." << endl
        << "Possible activities :" << endl
        << "0 - Quit" << endl
        << "1 - Stop everything!!!" << endl
        << "2 - Show distance to front" << endl
        << "3 - Show front view" << endl
        << "4 - Dumb drive" << endl;
}

void activities::show_front_distance()
{
    while (true)
    {
        cout << "distance to front = " << sonar_front->get_distance() << endl;
        if (_stop_execution) break;
        delay(100);
    }
    drv->force_stop();
    _stop_execution = false;
    _is_executing = false;
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
            delay(250);
        }
        drv->force_stop();
        _stop_execution = false;
        _is_executing = false;
}


void activities::show_front_view()
{
    cvNamedWindow("front view");
    while (true)
    {
        IplImage * frame =  cam_front->get_frame();

        cvShowImage("front view", frame);
        //cvWaitKey(1);
        if (_stop_execution) break;
        cvWaitKey(1);
    }
    cvDestroyWindow("front view");
    _stop_execution = false;
    _is_executing = false;
}




