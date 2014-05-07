#include "activities/activities.h"

using namespace cv;

activities::activities()
{
    cout << "NOTE : activities() is disabled" << endl;

    _sonar_front = new sonar(PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);
    _chip_16pwm = new pwm_chip (PWM_CHIP_ADDR);
    _servo_spare = new servo (_chip_16pwm, PIN_SERVO);
    _drv = new drivetrain (_chip_16pwm);
    _sonar_front->set_drivetrain(_drv);


    _cam_front = new camera (USB_FRONT_CAMERA_NO);
    _adv_opencv = new advanced_opencv();
}



activities::~activities()
{
    cout << "NOTE : ~activities() is disabled" << endl;

    force_stop();

    _drv->~drivetrain();
    _sonar_front->~sonar();

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

    case 6 : // init floor
        force_stop();
        _execution_thread = new thread (&activities::init_floor, this);
        _is_executing = true;
        break;

    case 7 : // dumb drive
        force_stop();
        _execution_thread = new thread (&activities::dumb_drive, this);
        _is_executing = true;
        break;

    case 8:
        control_robot();
        break;


    case 9:
        force_stop();
        _execution_thread = new thread (&activities::optical_flow, this);
        _is_executing = true;
        break;

    case 10:
        force_stop();
        _execution_thread = new thread (&activities::temp, this);
        _is_executing = true;
        break;

    default :
        cout << "activities::act() - unknown activity" << endl;
        break;
    }

}

void activities::temp ()
{
    cvNamedWindow("optical flow");
  //  cvNamedWindow("mask");
    cvStartWindowThread();
    std::vector<cv::Point2f>  features;
    IplImage * prev_gray =  _adv_opencv->create_GRAY_by_RGB(_cam_front->get_frame());

    double t = 0;

    _adv_opencv->angle = 0;
    _adv_opencv->y_distance = 0;

    while (true)
    {
        IplImage* rgb = _cam_front->get_frame();
        IplImage * curr_gray =  _adv_opencv->create_GRAY_by_RGB(rgb);

        t = (double)cvGetTickCount();

        _adv_opencv->temp(rgb, prev_gray, curr_gray, &features);

        t = (double)cvGetTickCount() - t;
        printf("detection time = %g ms  ", t/((double)cvGetTickFrequency()*1000));

        prev_gray = curr_gray;

            cvShowImage("optical flow", rgb);


        if (_stop_execution) break;
    }

        cvDestroyWindow("optical flow");
  //  cvDestroyWindow("mask");
    _stop_execution = false;


    _is_executing = false;

}

void activities::print_activities()
{
    cout << "Robot Jerry at your service..." << endl
         << "Possible activities :" << endl
         << "0 - Quit" << endl
         << "1 - Stop everything!!!" << endl
         << "2 - Show distance to front" << endl
         << "3 - Show view front " << "4 - Show view Canny " << "5 - Show view HVS " << "6 - Init floor" << endl
         << "7 - Dumb drive" << endl
         << "8 - Control me" << endl
         << "9 - Optical flow" << endl;
}

void activities::show_front_distance()
{
    while (true)
    {
        cout << "distance to front = " << _sonar_front->get_distance() << endl;
        if (_stop_execution) break;
        delay(100);
    }
    _drv->force_stop();
    _stop_execution = false;
    _is_executing = false;
}


void activities::dumb_drive()
{
    while (true)
    {
        _drv->a_drive(FORWARD, 5);
        if (_stop_execution) break;
        _drv->wait_to_finish(0);
        if (_stop_execution) break;
        _drv->a_turn(TURN_RIGHT, 0.8);
        if (_stop_execution) break;
        _drv->wait_to_finish(0);
        if (_stop_execution) break;
        delay(250);
    }
    _drv->force_stop();
    _stop_execution = false;
    _is_executing = false;
}


void activities::show_front_view()
{
    cvNamedWindow("camera", 1);
    cvStartWindowThread();

    while (true)
    {
        IplImage * frame =  _cam_front->get_frame();

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
        IplImage * frame =  _cam_front->get_frame();

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
        IplImage * frame =  _cam_front->get_frame();

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
    cvNamedWindow("floor");
    //cvNamedWindow("fin");
    cvStartWindowThread();
    while (true)
    {
        IplImage * frame =  _cam_front->get_frame();
        GaussianBlur( (Mat)frame, (Mat)frame, Size(3, 3), 0, 0);

        //erode((Mat)frame, (Mat)frame, NULL, Point(-1, -1), 1);
        //dilate((Mat)frame, (Mat)frame, Mat(), Point(-1,-1), 1);


        cvtColor((Mat)frame, (Mat)frame, CV_BGR2HSV);

        CvScalar * scal = _adv_opencv->get_bottom_line_pixel_mean(frame);

        IplImage * th_img = _adv_opencv->GetThresholdedImage(frame, *scal );

        //IplImage* imgrez=cvCreateImage(cvGetSize(th_img),IPL_DEPTH_8U, 1);

        // val[0] - b, val[1] - a;
        scal = _adv_opencv->mark_line(th_img, th_img);


        double angle = scal->val[0] * B_to_angle_conversion + 90;

        cout << angle << "b - " << scal->val[0] << endl;



        cvShowImage("floor", th_img);
        // cvShowImage("fin", imgrez);

        // cvWaitKey(1);


        if (_stop_execution) break;
        delay(100);
    }
    cvDestroyWindow("floor");
    //  cvDestroyWindow("fin");
    _stop_execution = false;
    _is_executing = false;
}

void activities::control_robot()
{
    int cmd;
    bool repeat = true;
    int arg1 =0;
    float arg2 = 0;
    while (repeat)
    {
        cout << "Move mode, options :" << endl
        << "1 - stop moving!!!" << endl
        << "2 - turn left" << endl
        << "3 - turn right" << endl
        << "4 - drive forward" << endl
        << "5 - drive backward" << endl
        << "6 -  trive left" << endl
        << "7 -  trive right" << endl

        << "8 -  set turn speed" << endl
        << "9 -  set drive speed" << endl
        << "0 - quit to main menu" << endl << "Your option : ";

        cin >> cmd;

        switch (cmd)
        {
        case 1:
            force_stop();
            break;

        case 2:
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_turn(TURN_LEFT, 0.1 * arg1);

            break;

        case 3:
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_turn(TURN_RIGHT, 0.1 * arg1);

            break;

        case 4:
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_drive(FORWARD, 0.1 * arg1);

            break;

        case 5:
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_drive(BACKWARD, 0.1 * arg1);
            break;

        case 6:
            cout << "input left angle [0.0-1.0](float) - ";
            cin >> arg2;
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_trive(1 - arg2, 1, 0.1 * arg1);
            break;

        case 7:
            cout << "input right angle [0.0-1.0](float) - ";
            cin >> arg2;
            cout << "input time (int)x100ms - ";
            cin >> arg1;
            cout << endl;
            _drv->a_trive(1, 1 - arg2, 0.1 * arg1);
            break;

        case 8:
            cout << "input new turn speed - ";
            cin >> arg2;
            cout << endl;
            _drv->set_turn_speed(arg2);
            break;

        case 9:
            cout << "input new drive speed - ";
            cin >> arg2;
            cout << endl;
            _drv->set_drive_speed(arg2);
            break;

        case 0:
            repeat = false;

            break;

        default:
            cout << "Invalid option" << endl;
            break;
        }

    }

}

void activities::optical_flow()
{
  //  cvNamedWindow("optical flow");
    //cvNamedWindow("mask");
    cvStartWindowThread();
    std::vector<Two_points>  features;
    IplImage * prev_gray =  _adv_opencv->create_GRAY_by_RGB(_cam_front->get_frame());

    while (true)
    {
        IplImage* rgb = _cam_front->get_frame();
        IplImage * curr_gray =  _adv_opencv->create_GRAY_by_RGB(rgb);
        Point_<float> vect = _adv_opencv->get_motion_vector(rgb, prev_gray, curr_gray, &features);
        //cout << vect.x << " , " << vect.y << " - motion vector" << endl;
        prev_gray = curr_gray;
       // cvShowImage("optical flow", rgb);
        if (_stop_execution) break;
    }
  //  cvDestroyWindow("optical flow");
    //cvDestroyWindow("mask");
    _stop_execution = false;
    _is_executing = false;
}
