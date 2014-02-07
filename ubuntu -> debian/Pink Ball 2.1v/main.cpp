#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include <wiringPi.h>

#include <ctype.h>


using namespace std;
using namespace cv;

int frame_width = 320;
int frame_height = 240;
int camera_no = 0;
bool found_ball = false;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS ( imgHSV, cvScalar(150,80,180), cvScalar(180,180,255), imgThresh);  // wide green
       return imgThresh;
}

Point getCentroid(IplImage img)
{
    Point rez;
    rez.x = 0; rez.y = 0;
    int columnSum =0, Quant =0, rowSum=0;
    bool found = false;

    char* p = img.imageData;
    for(int row = 0; row < img.height; row++)
    {
        for(int col = 0; col < img.width; col++)
        {
            if (*p == 255)  // -1 == 11111111
            {
                found = true;
                columnSum += col;  rowSum += row;
                Quant++;
            }
            p++;
        }
    } // main iteration
    if (Quant > 100)
    {
        rez.x = columnSum / Quant;
        rez.y = rowSum / Quant;
    }
    return rez;
}

int main(int argc, char * argv[]){
      wiringPiSetup();  // initialize library

      pinMode(0, OUTPUT);  // forward left
      pinMode(2, OUTPUT);  // backward left
      pinMode(1, OUTPUT);  // forward right
      pinMode(3, OUTPUT);  // backward right

      CvCapture* capture =0;

      capture = cvCaptureFromCAM(camera_no);
      if (capture == NULL)
      {
	cout << "CvCapture == null\n";
	return 0; 
      }
      cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, frame_width);
      cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, frame_height);

      IplImage* frame=0;
   //   cvNamedWindow("Video");
   //   cvNamedWindow("Ball");
        double t = 0;
      //iterate through each frames of the video
      while(true){
       //     t = (double)cvGetTickCount();
            frame = cvQueryFrame(capture);
	    if (frame == NULL)
	    {
		cout << "frame == null\n";
		continue; 
	    }
       //     t = (double)cvGetTickCount() - t;
      //      printf( "capture time = %8g ms ", t/((double)cvGetTickFrequency()*1000) );

       //     t = (double)cvGetTickCount();
            frame=cvCloneImage(frame);

        //    GaussianBlur(Mat(frame, false), Mat(frame, false), cvSize(3,3), 1.5, 1.5);

         //   IplImage * imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);

            cvtColor( (Mat)frame, (Mat)frame, CV_BGR2HSV); //Change the color format from BGR to HSV


            IplImage * imgThresh = GetThresholdedImage(frame);

            Point pnt = getCentroid(*imgThresh);


	    

            if ((pnt.x > 0) && (argc == 1))
            {
		found_ball = true;
             //   cout << "center of ball - (" << pnt.x << "," << pnt.y << ")" << endl;

                if (pnt.x < frame_width / 3) // turn left
                {
                    // turn on - right wheel for 0.5s ( pulse 2/10 )
                    float pulse = 15;
                //    for (int i = 0; i < 50; i++)
                    {
                        digitalWrite(1, HIGH);
			digitalWrite(2, HIGH);
                        delay(pulse);
                        digitalWrite(1, LOW);
			digitalWrite(2, LOW);
                 //       delay(10 - pulse);
                    }
                }
                else if (pnt.x > ((frame_width / 3) * 2)) // turn right
                {
                    // turn on - left wheel for 0.5s ( pulse 2/10 )
                    float pulse = 15;
               //     for (int i = 0; i < 50; i++)
                    {
                        digitalWrite(0, HIGH);
			digitalWrite(3, HIGH);
                        delay(pulse);
                        digitalWrite(0, LOW);
			digitalWrite(3, LOW);
                  //      delay(10 - pulse);
                    }
                } else // in middle - go forward
		{
		  float pulse = 15;
		  digitalWrite(0, HIGH);
		  digitalWrite(1, HIGH);
                  delay(pulse);
                  digitalWrite(0, LOW);
		  digitalWrite(1, LOW);
		}
            } 
            else 
	    {
		found_ball = false;
	    }

	if (found_ball == false) // look for a ball
	{
	     float pulse = 15;
	   //for (int i = 0; i < 50; i++)
	     {
	       digitalWrite(1, HIGH);
	       digitalWrite(2, HIGH);
	       delay(pulse);
	       digitalWrite(1, LOW);
	       digitalWrite(2, LOW);
	       //delay(10 - pulse);
	     }
	}

     //       cvShowImage("Ball", imgThresh);
     //       cvShowImage("Video", frame);

     //       t = (double)cvGetTickCount() - t;
     //       printf( "process time = %8g ms\n", t/((double)cvGetTickFrequency()*1000) );

            //Clean up used images
        //   cvReleaseImage(&imgHSV);
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


