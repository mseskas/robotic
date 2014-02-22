#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


class advanced_opencv
{
    public:
        advanced_opencv();
        ~advanced_opencv();
        IplImage* GetThresholdedImage(IplImage* imgHSV, CvScalar hvs);
    protected:
    private:
};
