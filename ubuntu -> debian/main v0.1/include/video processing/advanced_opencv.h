#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class advanced_opencv
{
    public:
        advanced_opencv();
        ~advanced_opencv();
        IplImage * GetThresholdedImage(IplImage* imgHSV, CvScalar hvs);
        CvScalar * get_bottom_line_pixel_mean(IplImage * img);
        void mark_line(IplImage* img, IplImage* rezimg);
    protected:
    private:

};
