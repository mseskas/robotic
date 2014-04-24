#include "video processing/advanced_opencv.h"

using namespace std;
using namespace cv;

advanced_opencv::advanced_opencv()
{
    //ctor
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

cv::Point_<float> nu(std::vector<Point2f> prev, std::vector<Point2f> curr, vector<uchar> status)
{
    Point_<float> p;


    for (int i = 0; i < status.size(); i++)
    {
        if (status.at(i) == 1)
        {
            p.x += curr.at(i).x - prev.at(i).x;
            p.y += curr.at(i).y - prev.at(i).y;
        }

    }
    p.x = p.x / status.size();
    p.y = p.y / status.size();
    return p;
}

void advanced_opencv::temp(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
                                            std::vector<Point2f> * features)
{
    vector<uchar> status;
    vector<float> err;

    std::vector<cv::Point2f> features_prev, features_curr;

    if ( features->size() > 0)
    {
        cv::calcOpticalFlowPyrLK((Mat)prev_gray, (Mat)curr_gray, *features, features_curr, status, err);

        Point_<float> p = nu(*features, features_curr, status);
        cout << p.x << " , " << p.y << " - vector" << endl;
    }
    features->clear();
    cv::goodFeaturesToTrack((Mat)curr_gray, *features, 50, 0.01, 10 );

    for(int i = 0; i < features_curr.size(); i ++)
        {
            cvCircle(rgb, features_curr.at(i), 2, cvScalar(0, 255, 0) );
            cvLine(rgb, features->at(i), features_curr.at(i), cvScalar( 255, 0, 255));
        }


}

cv::Point_<float> advanced_opencv::get_motion_vector(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
                                            std::vector<Two_points> * features)
{

    std::vector<cv::Point2f> features_prev, features_curr;

    vector<uchar> status;
    vector<float> err;

    if (features->size() > 0)
    {
        for (int i = 0; i < features->size(); i++)
            features_prev.push_back(features->at(i).end);
    }

    std::vector<Two_points> new_tracks;
    cv::Point_<float> p;

    if ( features_prev.size() > 0)
    {
        cv::calcOpticalFlowPyrLK((Mat)prev_gray, (Mat)curr_gray, features_prev, features_curr, status, err);

        p = nu(features_prev, features_curr, status);
        cout << p.x << " , " << p.y << " - vector" << endl;

        for (int i = 0; i < status.size(); i++)
        {
            if (status.at(i) == 1)
            {
                Two_points p;
                p.start = features->at(i).start;
                p.end = features_curr.at(i);

                new_tracks.push_back(p);
            }
        }
        features->clear();
        for (int i = 0; i < new_tracks.size(); i++)
            features->push_back(new_tracks.at(i));
    }

    if ( features_prev.size() < OPTICAL_FLOW_MAX_FEATURES)
    {
        IplImage * mask = cvCreateImage(cvGetSize(curr_gray ),IPL_DEPTH_8U, 1);

        cvSet(mask, cvScalar(255));
        for (int i = 0; i < features->size(); i++)
            cvCircle(mask, features->at(i).start, 5, cvScalar(0), -1);

        //cvShowImage("mask", mask);

        cv::goodFeaturesToTrack((Mat)curr_gray, features_curr, 50, 0.01, 10, (Mat)mask );  // need to add mask
        // add all feature to prev_features
        for (int i = 0; i < features_curr.size(); i++)
        {
            Two_points point;
            point.start = features_curr.at(i);
            point.end = features_curr.at(i);
            features->push_back(point);
        }
    }

   /* for(int i = 0; i < features->size(); i ++)
        {
            //cvCircle(curr_gray, features_prev[i], 2, cvScalar(120, 120, 120) );
            cvCircle(rgb, features->at(i).end, 2, cvScalar(0, 255, 0) );
            cvLine(rgb, features->at(i).start, features->at(i).end, cvScalar( 255, 0, 255));
        }
*/

    return  p;
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
