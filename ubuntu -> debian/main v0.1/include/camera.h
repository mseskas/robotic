#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

// usb camera, default options :
// video width/height = 320/240
class camera
{
public:
    camera(int camera_no);
    camera(int camera_no, int frame_width, int frame_height);
    ~camera();
    IplImage * get_frame();
protected:
private:
    CvCapture * _capture;
    IplImage * _last_frame;
};
