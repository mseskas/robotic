

#include "video processing/advanced_opencv.h"

advanced_opencv::advanced_opencv()
{
    //ctor
}

advanced_opencv::~advanced_opencv()
{
    //dtor
}

IplImage* advanced_opencv::GetThresholdedImage(IplImage* imgHSV, CvScalar hvs)
{
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS ( imgHSV, cvScalar(0,hvs.val[0]-25  ,0),
    cvScalar(255, hvs.val[1]+25  ,255 ), imgThresh);
    return imgThresh;
}
