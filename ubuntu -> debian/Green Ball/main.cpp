#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>



#include <ctype.h>


using namespace std;
using namespace cv;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(30,128,128), cvScalar(100,256,256), imgThresh);  // wide green
       return imgThresh;
}

Point getCentroid(IplImage img)
{
    Point rez;
    int columnSum =0, Quant =0, rowSum=0;
    bool found = false;

    char* p = img.imageData;
    for(int row = 0; row < img.height; row++)
    {
        for(int col = 0; col < img.width; col++)
        {
            if (*p == -1)  // -1 == 11111111
            {
                found = true;
                columnSum += col; // rowSum += row;
                Quant++;
            }
            p++;
        }
    } // main iteration
    if (Quant > 0)
    {
        rez.x = columnSum / Quant;
        rez.y = 0;//rowSum / Quant;
    }
    if (found) cout << "center - (" << rez.x << "," << rez.y << ")" << endl;
    return rez;
}

int main(){
      CvCapture* capture =0;

      capture = cvCaptureFromCAM(0);

      IplImage* frame=0;
      cvNamedWindow("Video");
      cvNamedWindow("Ball");
        double t = 0;
      //iterate through each frames of the video
      while(true){
            t = (double)cvGetTickCount();
            frame = cvQueryFrame(capture);
            t = (double)cvGetTickCount() - t;
            printf( "capture time = %8g ms ", t/((double)cvGetTickFrequency()*1000) );

            t = (double)cvGetTickCount();
            frame=cvCloneImage(frame);

     //       GaussianBlur(Mat(frame, false), Mat(frame, false), cvSize(3,3), 1.5, 1.5);

            IplImage * imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);

            cvtColor( (Mat)frame, (Mat)imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV


            IplImage * imgThresh = GetThresholdedImage(imgHSV);

            Point p = getCentroid(*imgThresh);



            cvShowImage("Ball", imgThresh);
            cvShowImage("Video", frame);

            t = (double)cvGetTickCount() - t;
            printf( "process time = %8g ms\n", t/((double)cvGetTickFrequency()*1000) );

            //Clean up used images
            cvReleaseImage(&imgHSV);
            cvReleaseImage(&imgThresh);
            cvReleaseImage(&frame);


            int c = cvWaitKey(1);
            //If 'ENTER' is pressed, break the loop
            if((char)c==10 ) return 0;
      }

      cvDestroyAllWindows() ;
      cvReleaseCapture(&capture);

      return 0;
}
