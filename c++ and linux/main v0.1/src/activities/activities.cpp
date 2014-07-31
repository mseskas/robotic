#include "activities/activities.h"

using namespace cv;

activities::activities()
{
    cout << "NOTE : activities() is disabled" << endl;

    _drv = NULL;
    _sonar_front = NULL;
    _chip_16pwm = NULL;
    _servo_spare = NULL;


    _sonar_front = new sonar(PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);
    _chip_16pwm = new pwm_chip (PWM_CHIP_ADDR);


    _servo_spare = new servo (_chip_16pwm, PIN_SERVO);
    _drv = new drivetrain (_chip_16pwm);
    _sonar_front->set_drivetrain(_drv);


    _cam_front = new camera (USB_FRONT_CAMERA_NO);
    _adv_opencv = new advanced_opencv();

    _stop_execution = false;
    _is_executing = false;
}

activities::~activities()
{
    cout << "NOTE : ~activities() is disabled" << endl;
    force_stop();

    //_drv->~drivetrain();
    //_sonar_front->~sonar();

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

void bla();

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
        optical_flow(true, "");
        //_execution_thread = new thread (&activities::optical_flow, this, true, ""); // using camera - real time
        _is_executing = true;
        break;

    case 10:
    {
        force_stop();

        string str;
        cout << "Video file url = ";
        cin >> str;

        _execution_thread = new thread (&activities::optical_flow, this, false, str); // use video file - NOT real time
        _is_executing = true;
    }
    break;

    case 11:
    {
        force_stop();
        int arg2 = 0;
        cout << "press 1 to show video" << endl;
        cin >> arg2;

       // bla();

       //_execution_thread = new thread (&activities::record_video, this, false);

        if (arg2 == 1)
            _execution_thread = new thread (&activities::record_video, this, true);
        else
           _execution_thread = new thread (&activities::record_video, this, false);

        _is_executing = true;
    }
    break;

    default :
        cout << "activities::act() - unknown activity" << endl;
        break;
    }

}


void activities::record_video(bool show_video)
{
    if (show_video)
    {
     cvNamedWindow("Capture");
        cvStartWindowThread();
    }

    Mat frame;

    time_t ti = time(0);   // get time now
    struct tm * now = localtime( & ti );

    string datetime = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-"+ to_string(now->tm_mday)
                + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min)  + ":" + to_string(now->tm_sec);




    //string dir = std::string("/root/Desktop/capture.avi") ;

    string dir = std::string("/root/Desktop/capture.avi"); //.append(datetime).append(".avi");

    //string dir = std::string("/home/minde/Desktop/").append(datetime).append(".avi");



    int fps = 15;


    VideoWriter video;


    video.open(dir, CV_FOURCC('D','I','V','X'),  fps,
                        cvSize((int)CAPTURE_FRAME_WIDTH, (int)CAPTURE_FRAME_HEIGHT));

    if(!video.isOpened())
    {
        cout << " Could not create video." << endl;
        return;
    }

    cout << "Press Esc to stop recording." << endl;

    double t = 0;

    while(true)
    {
        t = (double)cvGetTickCount();  // mark when started capture
        frame = Mat(_cam_front->get_frame());

        if(!frame.data)
        {
            cout << "Could not retrieve frame.";
            return;
        }

        video << frame;

        if (show_video)
            imshow("Capture", frame);

     //   t = (double)cvGetTickCount() - t;  // count time past

     //   t = t/((double)cvGetTickFrequency()*1000); // in ms



        // 1 sec / fps  -  time past in capture  = time to wait till next frame
   //     t = (double)(1000/fps) - t;

      //  if (t <= 0 )
       //     t = 1;

       // int k =  0xFF & waitKey(t);  // 1111 1111  &  ???? ???? 1010 1010  == 1010 1010


//        if(k == 27)
         //   break;


        if (_stop_execution) break;
    }

    if (show_video)
        cvDestroyWindow("Capture");

     _stop_execution = false;
    _is_executing = false;

}

void activities::optical_flow (bool use_camera, String video_file_url)
{

// map =============================================================
    cvNamedWindow("map");

    _adv_opencv->angle = 90;
    _adv_opencv->y_distance = 650;
    _adv_opencv->x_distance = 350;

    Mat mapas = Mat::zeros(700, 700, CV_8UC1);

    circle(mapas, cvPoint( _adv_opencv->x_distance, _adv_opencv->y_distance), 1, cvScalar(255) );


    imshow("map", mapas);

// map =============================================================



    cvNamedWindow("optical flow");
  //  cvNamedWindow("mask");
    cvStartWindowThread();
    std::vector<cv::Point2f>  features;

    IplImage * prev_gray = NULL;
    VideoCapture * v = NULL;

    if (use_camera)
    {
        prev_gray = _adv_opencv->create_GRAY_by_RGB(_cam_front->get_frame());
    }
    else
    {
        v = new VideoCapture();
        //bool success = v->open("/home/minde/Desktop/a.webm");
        bool success = v->open(video_file_url);
        if (!success)
        {
            cout << "Failed to open video file!" << endl;
            // kill process  =====================
             cvDestroyWindow("optical flow");
           //cvDestroyWindow("map");
           //cvDestroyWindow("mask");
             _stop_execution = false;
             _is_executing = false;
             // kill process  =====================
            return;
        }
        Mat  prev_mat ;
        v->read(prev_mat);
        prev_gray = _adv_opencv->create_GRAY_by_RGB( new IplImage(prev_mat));
    }

    IplImage* rgb = NULL;
    IplImage * curr_gray = NULL;

    double t = 0;

    while (true)
    {
        if (use_camera)
        {
            rgb = _cam_front->get_frame();
            curr_gray =  _adv_opencv->create_GRAY_by_RGB(rgb);
        }
        else
        {
            Mat  curr_mat;
            if ( v->read(curr_mat) == NULL)
            {
                print_activities();
                break;
            }
            rgb = new IplImage(curr_mat);
            curr_gray = _adv_opencv->create_GRAY_by_RGB(rgb);
            //delay(250);
        }

        t = (double)cvGetTickCount();
        _adv_opencv->get_motion_vector(rgb, prev_gray, curr_gray, &features);

        t = (double)cvGetTickCount() - t;
		printf( "  detection time = %g ms  ", t/((double)cvGetTickFrequency()*1000) );


        prev_gray = curr_gray;

        cvShowImage("optical flow", rgb);
// mapas =============================================================



        circle(mapas, cvPoint( _adv_opencv->x_distance, _adv_opencv->y_distance), 1, cvScalar(255) );
        imshow("map", mapas);
// mapas =============================================================

        if (_stop_execution) break;
    }

     cvDestroyWindow("optical flow");
   //  cvDestroyWindow("map");
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
         << "9 - Optical flow" << endl
         << "10 - Optical flow from file" << endl
         << "11 - record video" << endl;
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

        if (frame == NULL)
            cout << "Can't get frame!" << endl;

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

void activities::drive(int direction, float time  )
{

    force_stop();

    if (_drv != NULL)
        _drv->a_drive(direction, time);
    else
        cout << "a_drive(" << direction << ", " << time <<" )" << " - _drv is NULL" << endl;


}

void activities::turn(int direction, float time )
{

    force_stop();

    if (_drv != NULL)
        _drv->a_turn(direction, time);
    else
        cout << "a_turn(" << direction << ", " << time <<" );" << " - _drv is NULL" << endl;

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
