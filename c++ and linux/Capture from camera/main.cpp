#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>



using namespace cv;
using namespace std;

int main2(CvCapture* cap);

int main()
{
    CvCapture* cap = cvCreateCameraCapture(0);
    cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, 320);
    cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, 240);

    cvNamedWindow("first");
    double t = 0;

    while(1)
    {
		t = (double)cvGetTickCount();
        IplImage* img = cvQueryFrame(cap );



		t = (double)cvGetTickCount() - t;
		printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000) );
        cvShowImage("first", img);
        cvWaitKey(1);
    }
	cvReleaseCapture(&cap);
    cvDestroyWindow("first");
    return 0;
}
