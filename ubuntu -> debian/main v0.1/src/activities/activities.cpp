#include "activities/activities.h"

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
    adv_opencv = new advanced_opencv();
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

        case 5 : // show hvs view
            force_stop();
            _execution_thread = new thread (&activities::hvs_view, this);
            _is_executing = true;
        break;


        case 6 : // dumb drive
            force_stop();
            _execution_thread = new thread (&activities::dumb_drive, this);
            _is_executing = true;
        break;

        case 7 : // init floor
            force_stop();
            _execution_thread = new thread (&activities::init_floor, this);
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
        << "3 - Show view front" << endl
        << "4 - Show view Canny" << endl
        << "5 - Show view HVS" << endl
        << "6 - Dumb drive" << endl
        << "7 - init floor" << endl;
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

// using gaussian blur 3/3
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

        GaussianBlur( (Mat)canny, (Mat)canny, Size(3,3), 1.5, 1.5);


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

// using gaussian blur 3/3
void activities::init_floor()
{
    cvNamedWindow("floor", 1);
    cvStartWindowThread();

    while (true)
    {
        IplImage * frame =  cam_front->get_frame();
        GaussianBlur( (Mat)frame, (Mat)frame, Size(3,3), 1.5, 1.5);
        cvtColor((Mat)frame, (Mat)frame, CV_BGR2HSV);

        long h = 0, v = 0, s = 0;
        int quantity = 0 ;
        uchar * p = Mat(frame, false).ptr(frame->height -5 );

        for (int i = 10; i < frame->width; i += 10 )
        {
            p += i;
            h += *p; p++;
            v += *p; p++;
            s += *p; p -= 2;
            quantity++;
        }
        h /= quantity;
        s /= quantity;
        v /= quantity;
        cout << "FLOOR h/v/s - " << h << " / " << v << " / " << s << " / "
        << "by " << quantity << " pixels" << endl;

        IplImage * th_img = adv_opencv->GetThresholdedImage(frame, cvScalar(h,v,s));

        cvShowImage("floor", th_img);
        cvWaitKey(1);


        if (_stop_execution) break;
        delay(100);
    }
    cvDestroyWindow("floor");
    _stop_execution = false;
    _is_executing = false;
}


