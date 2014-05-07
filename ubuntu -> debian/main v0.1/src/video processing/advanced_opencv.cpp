#include "video processing/advanced_opencv.h"

using namespace std;
using namespace cv;

advanced_opencv::advanced_opencv()
{
    angle = 0;
    y_distance = 0;
}

advanced_opencv::~advanced_opencv()
{
    //dtor
}

IplImage * advanced_opencv::create_GRAY_by_RGB(IplImage* RGB_img)
{
    IplImage * gray = cvCreateImage(cvGetSize(RGB_img ),IPL_DEPTH_8U,1);

    cvtColor((Mat)RGB_img, (Mat)gray,  CV_RGB2GRAY);
    return gray;
}

cv::Point_<float> advanced_opencv::motion_by_features(std::vector<Point2f> prev, std::vector<Point2f> &curr, vector<uchar> status, IplImage* rgb)
{
    Point_<float> p;

    std::vector<Point2f> new_trac;

    for (int i = 0; i < status.size(); i++)
    {
        if (status.at(i) == 1)
        {
            p.x += curr.at(i).x - prev.at(i).x;
            p.y += curr.at(i).y - prev.at(i).y;
            new_trac.push_back(curr.at(i));
        //    cvLine(rgb, prev.at(i), curr.at(i), cvScalar( 255, 0, 255));
        }

    }
    curr = new_trac;
    p.x = p.x / status.size();
    p.y = p.y / status.size();
    angle += p.x;
    y_distance += p.y;
    cout << "angle = " << angle << ",  " << y_distance << " - vector" << endl;
    return p;
}

void advanced_opencv::get_motion_vector(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
                                            std::vector<Point2f> * features)
{
    vector<uchar> status;
    vector<float> err;

    std::vector<cv::Point2f> features_new, features_curr;

    if ( features->size() > 0)
    {
        cv::calcOpticalFlowPyrLK((Mat)prev_gray, (Mat)curr_gray, *features, features_curr, status, err);

        Point_<float> p = motion_by_features(*features, features_curr, status, rgb);
    }
    *features = features_curr;

   // cout << features_curr.size() << endl;
    if (features_curr.size() < OPTICAL_FLOW_MAX_FEATURES)
    {
        IplImage * mask = cvCreateImage(cvGetSize(curr_gray ),IPL_DEPTH_8U, 1);

        cvSet(mask, cvScalar(255));
        for (int i = 0; i < features->size(); i++)
            cvCircle(mask, features->at(i), 5, cvScalar(0), -1);

        cvRectangle(mask, cvPoint(0, CAPTURE_FRAME_HEIGHT /3 ), cvPoint(CAPTURE_FRAME_WIDTH,
            CAPTURE_FRAME_HEIGHT /3 *2), cvScalar(0), -1);

        //cvShowImage("mask", mask);
        cv::goodFeaturesToTrack((Mat)curr_gray, features_new, 50, 0.01, 10, (Mat)mask );
    }

    for(int i = 0; i < features_new.size(); i ++)
        features->push_back(features_new.at(i));

  //  for(int i = 0; i < features->size(); i ++)
   //     cvCircle(rgb, features->at(i), 2, cvScalar(0, 255, 0) );

}

IplImage* advanced_opencv::GetThresholdedImage(IplImage* imgHSV, CvScalar hsv)
{
    int Hstep =10;
    int Sstep = 25;
    int Vstep = 45;

    double h0 = hsv.val[0] - Hstep, s0 = hsv.val[1] - Sstep, v0 = hsv.val[2] - Vstep;
    double h1 = hsv.val[0] + Hstep, s1 = hsv.val[1] + Sstep, v1 = hsv.val[2] + Vstep;

    //cout << hsv.val[0] << " , " << hsv.val[1] << " , " << hsv.val[2] << endl;

    // if (h0 < 0) h0 =0; if (s0 < 0) s0 =0; if (v0 < 0) v0 =0;
    //if (h1 > 180) h0 =180; if (s1 > 255) s1 =255; if (v1 > 255) v1 =255;

    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS ( imgHSV, cvScalar(0, s0, v0), cvScalar(180, s1, v1), imgThresh);
    return imgThresh;
}


CvScalar * advanced_opencv::get_bottom_line_pixel_mean(IplImage * img)
{
    CvScalar scal;
    long pix1 = 0, pix2 = 0, pix3 = 0;
    int quantity = 0 ;
    uchar * p = Mat(img, false).ptr( img->height - 1);

    for (int i = 10; i < img->width; i += 10 )
    {
        p += 30;
        pix1 += *p;
        //cout << (int)(*p);
        //cout << ",";
        p++;
        pix2 += *p;
        //cout << (int)(*p);
        //cout << ",";
        p++;
        pix3 += *p;
        //cout << (int)(*p);
        //cout << " ";
        p -= 2;
        quantity++;
    }
    //cout << endl;
    scal.val[0] = pix1 / quantity;
    scal.val[1] = pix2 / quantity;
    scal.val[2] = pix3 / quantity;

    return &scal;
}

// image must bi single channel  result scalar ( b- xcoeficient, a - offset from y)
CvScalar * advanced_opencv::mark_line(IplImage * img, IplImage* rezimg)
{

    double sumX, sumXX, sumY, sumXY;


    int curr_y = img->height-1;
    for (int x = 0; x < img->width ; x += 1 )
    {
        //curr_y = img->height-1;
        for (int y = curr_y; y > 2; y -= 1) // downgrade loop
        {
            if ((CV_IMAGE_ELEM(img, uchar, y,x) == 0) && (CV_IMAGE_ELEM(img, uchar, y-1,x) == 0)
                    && (CV_IMAGE_ELEM(img, uchar, y-2,x) == 0))
            {
                // at(y, x)
                CV_IMAGE_ELEM(rezimg, uchar, y,x) = 128;
                CV_IMAGE_ELEM(rezimg, uchar, y-1,x) = 128;
                CV_IMAGE_ELEM(rezimg, uchar, y-2,x) = 128;

                if (y + 1 > img->height-1)
                {
                    curr_y = img->height-1;
                }
                else
                {
                    curr_y = y +1 ;
                }

                sumX += x;
                sumXX += x*x;
                sumXY += x*y;
                sumY += y;


                break;
            }
        }

    }
    int n = img->width;

    float b = (sumXY - ((sumX * sumY)/n) ) / (sumXX - ((sumX*sumX)/n) );

    float mean_x = sumX /n, mean_y = sumY /n;

    float a = mean_y - b * mean_x;

    cvLine(rezimg, cvPoint(0, (int)a), cvPoint(img->width-1, a + (b * (img->width-1))), cvScalar(50)
           ,3);

    CvScalar scal = cvScalar(b, a);

    return & scal;
}
