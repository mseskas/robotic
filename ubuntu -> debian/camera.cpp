
#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"


using namespace std;
using namespace cv;

int main()
{
cvNamedWindow("camera", 1);
CvCapture* cap = cvCaptureFromCAM(0);
cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, 320);
cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, 240);



if (!cap)
    cout << "no camera detected!" << endl;

double t;
while(1)
{
	t = (double)cvGetTickCount();

	IplImage* img = cvQueryFrame(cap);

	t = (double)cvGetTickCount() - t;
	printf("query time = %g ms \n", t/((double)cvGetTickFrequency()* 1000.));


	if (img == NULL)
	    cout << "img == null" << endl;

	cvShowImage("camera", img);
	cvWaitKey(1);

}
cvDestroyWindow("camera");

return 0;
}

