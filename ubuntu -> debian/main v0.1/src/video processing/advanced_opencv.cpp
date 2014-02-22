

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
    int step =40;

    double h0 = hvs.val[0] - step, s0 = hvs.val[1] - step, v0 = hvs.val[2] - step;
    double h1 = hvs.val[0] + step, s1 = hvs.val[1] + step, v1 = hvs.val[2] + step;

    if (h0 < 0) h0 =0; if (s0 < 0) s0 =0; if (v0 < 0) v0 =0;
    if (h1 > 255) h0 =255; if (s1 > 255) s1 =255; if (v1 > 255) v1 =255;


    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS ( imgHSV, cvScalar(h0, s0, v0),
    cvScalar(h1, s1, v1), imgThresh);
    return imgThresh;
}
