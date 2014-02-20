#include "camera.h"

using namespace std;

camera::camera(int camera_no)
{
    _capture = NULL;
    _capture = cvCaptureFromCAM(camera_no);
    if (_capture == NULL)
      {
        cout << "CvCapture == NULL cam no.:" << camera_no << endl;
        return;
      }
    cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH, 320);
    cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
}

camera::camera(int camera_no, int frame_width, int frame_height)
{
    _capture = NULL;
    _capture = cvCaptureFromCAM(camera_no);
    if (_capture == NULL)
      {
        cout << "CvCapture == NULL cam no.:" << camera_no << endl;
        return;
      }
    cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH, frame_width);
    cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT, frame_height);
}

camera::~camera()
{
    cvReleaseCapture(&_capture);
}

IplImage * camera::get_frame()
{
    _last_frame = cvQueryFrame(_capture);
    return _last_frame;
}
