#include "activities.h"

using namespace cv;

activities::activities()
{
 cout << "please uncomment me - activities::activities()" << endl;
 /*   sonar_front = new sonar(PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);
    chip_16pwm = new pwm_chip (PWM_CHIP_ADDR);
    servo_spare = new servo (chip_16pwm, PIN_SERVO);
    drv = new drivetrain (chip_16pwm);


    sonar_front->set_drivetrain( drv); */

    cam_front = new camera (USB_FRONT_CAMERA_NO);
}



activities::~activities()
{
    cout << "please uncomment me -activities::~activities()" << endl;
   /* force_stop();
    drv->~drivetrain();
    sonar_front->~sonar(); */
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

        case 4 : // show canny view
            force_stop();
            _execution_thread = new thread (&activities::canny_edge_view, this);
            _is_executing = true;
        break;

        case 5 : // dumb drive
            force_stop();
            _execution_thread = new thread (&activities::dumb_drive, this);
            _is_executing = true;
        break;

        case 6 : // show hvs view
            force_stop();
            _execution_thread = new thread (&activities::hvs_view, this);
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
        << "4 - Show front Canny view" << endl
        << "5 - Dumb drive" << endl
        << "6 - Show front HVS view" << endl;
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
    cvNamedWindow("camera", 1);
    cvStartWindowThread();

    while (true)
    {
        IplImage * frame =  cam_front->get_frame();

        cvShowImage("camera", frame);
        //cvWaitKey(1);
        if (_stop_execution) break;
        cvWaitKey(1);
    }
    cvDestroyWindow("camera");

    _stop_execution = false;
    _is_executing = false;
}


void activities::canny_edge_view()
{
    namedWindow("Canny");
    startWindowThread();

    while(true)
    {
        IplImage * frame =  cam_front->get_frame();

        //frame=cvCloneImage(frame);
       // cvtColor((Mat)frame, (Mat)frame, CV_BGR2HSV);

        IplImage *canny = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);

        cvtColor((Mat)frame, (Mat)canny,  CV_RGB2GRAY);

        cv::Canny ((Mat)canny, (Mat)canny, 10, 60 );

        cvShowImage("Canny", canny);
        cvWaitKey(1);

        if (_stop_execution) break;
    }
    destroyWindow("Canny");

    _stop_execution = false;
    _is_executing = false;
}

void activities::hvs_view()
{

    namedWindow("HVS");

    startWindowThread();

    while(true)
    {
        IplImage * frame =  cam_front->get_frame();

      // frame=cvCloneImage(frame);

        cvtColor((Mat)frame, (Mat)frame, CV_BGR2HSV);

       // IplImage *canny = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
       // cvtColor((Mat)frame, (Mat)canny,  CV_RGB2GRAY);
       // cv::Canny ((Mat)canny, (Mat)canny, 10, 60 );

        cvShowImage("HVS", frame);
        cvWaitKey(1);

        if (_stop_execution) break;
    }
    destroyWindow("HVS");

    _stop_execution = false;
    _is_executing = false;
}
