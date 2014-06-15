#include "video processing/advanced_opencv.h"

using namespace std;
using namespace cv;

advanced_opencv::advanced_opencv()
{
    this->angle = 90;
    this->y_distance = 0;
    this->x_distance = 0;
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

#include <math.h>

double lambda(double x, double y)
{
    double atan2 =   y/ ( sqrt(x * x + y * y) + x ) ;

    return atan2 / M_PI * 180;
}

double histogram_voting(vector<double> data)
{
    if (data.size() == 0)
        return 0;

    vector<double> rez;
    //double curr = data.at(0);

    for (int i = 0; i < data.size(); i++)
    {
        rez.push_back( round(data.at(i) * 10) / 10  );
    }

    std::sort (rez.begin(), rez.end());

    std::map<int,int> dict;

    double curr = rez.at(0);
    int quantity = 1;

    int max_quantity = 1;
    double curr_max = curr;

    for (int i = 1; i < rez.size(); i++)
    {
        if (rez.at(i)  == curr)
            quantity++;
        else
        {
            dict[curr] = quantity;
            if (max_quantity < quantity)
            {
                max_quantity = quantity;
                curr_max = curr;
            }
          //  cout << curr << " - apeared " << dict[curr] << " times  " << endl;
            quantity = 1;
            curr = rez.at(i);
        }
    }

     cout << curr_max << " most accured time   " << max_quantity <<endl;
     // waitKey(0);


    return curr_max;
}



double pasisukimas(std::vector<Point2f> curr, std::vector<Point2f> prev)
{
    double Cx = CAPTURE_FRAME_WIDTH / 2;
    double Fx = CAPTURE_FRAME_HEIGHT;

    vector<double> rez;

    int top_horizont_line = CAPTURE_FRAME_HEIGHT /3;

  //  int counter = 0;

    for (int i = 0; i < curr.size(); i++)
    {
        if (curr.at(i).y < top_horizont_line && prev.at(i).y < top_horizont_line)
        {
            double pasisukimas = lambda(curr.at(i).x - Cx, Fx ) -
                                    lambda(prev.at(i).x - Cx, Fx );

            rez.push_back(pasisukimas);
         //   counter++;
           // cout << "counter : " << counter << endl;
        }

      // cout << pasisukimas << "\t";
    }
    return histogram_voting(rez);
 /*   for (int i = 0; i < curr.size(); i++)
    {
        printf("%12f", rez.at(i));
    }   */
  //  cout << "rotation : " << rez.at(rez.size()/ 2) << endl;
  // waitKey(0);

   //return rez.at(rez.size()/ 2);
}

std::pair<double, double>   paslinkimas(std::vector<Point2f> curr, std::vector<Point2f>prev, double rotation)
{
    double Cx = CAPTURE_FRAME_WIDTH / 2;
    double Fx = CAPTURE_FRAME_HEIGHT;


    std::pair<double, double> rez;

    std::vector<Point2f> corrected;

    double c = cos(rotation * M_PI / 180.0);
    double s = sin(rotation * M_PI / 180.0);

    for (int i = 0; i < curr.size(); i++)
    {
        Point2f p(0,0);

        p.x =  (c * (curr.at(i).x - Cx) - s * (Fx - curr.at(i).y) )  +  Cx;
        p.y =  Fx - (s * (curr.at(i).x - Cx) + c * (Fx - curr.at(i).y))  ;

        corrected.push_back(p);
    }

    double dx = 0, dy = 0;
    double counter = 0;

     int bottom_horizont_line = CAPTURE_FRAME_HEIGHT /3 * 2;

    for (int i = 0; i < corrected.size(); i++)   // skaiciuojam poslinki tik tu kurie pavirsiaus regione
    {
         if (corrected.at(i).y > bottom_horizont_line && corrected.at(i).y > bottom_horizont_line)
        {

            dx += corrected.at(i).x - prev.at(i).x;
            dy += corrected.at(i).y - prev.at(i).y;
            counter++;
        }
    }

    dx /= counter;
    dy /= counter;

    return std::pair<double, double>(dx, dy);
}

cv::Point_<float> advanced_opencv::motion_by_features(std::vector<Point2f> prev, std::vector<Point2f> curr,
                                                        vector<uchar> status, IplImage* rgb)
{
    Point_<float> p;

  /*  std::vector<Point2f> new_trac;
    std::vector<Point2f> prev_trac;

    for (int i = 0; i < status.size(); i++)
    {
        if (status.at(i) == 1)
        {
            //p.x += curr.at(i).x - prev.at(i).x;
            //p.y += curr.at(i).y - prev.at(i).y;

            new_trac.push_back(curr.at(i));
            prev_trac.push_back(prev.at(i));

            //cvLine(rgb, prev.at(i), curr.at(i), cvScalar( 255, 0, 255));

        }

    }

    for (int i = 0; i < new_trac.size(); i++)
    {
        cvLine(rgb, prev_trac.at(i), new_trac.at(i), cvScalar( 255, 0, 255));
    }

    //if (curr.size() != new_trac.size())
      //   cvCircle(rgb, cvPoint(100, 100), 50, cvScalar(100, 100, 100), -1);

    curr = new_trac;
    prev = prev_trac;

    */

    double rotation = pasisukimas(curr, prev);

    std::pair<double, double>  paslink = paslinkimas(curr, prev, rotation);



    //p.x = p.x / status.size();
    //p.y = p.y / status.size();
    //angle += p.x;

    double dx = std::get<0>(paslink);
    double dy = std::get<1>(paslink);

    double c = cos(rotation * M_PI / 180.0);
    double s = sin(rotation * M_PI / 180.0);

    double gDeltaX = dx * c - dy * s;
    double gDeltaY = dx * s + dy * c;

 /*   x_distance += gDeltaX;
    y_distance += gDeltaY;

    angle += rotation;
*/

    this->angle += rotation / 2;

    double Gc = cos(angle * M_PI / 180.0);
    double Gs = sin(angle * M_PI / 180.0);

    x_distance = x_distance + Gc * (-gDeltaY) / 10;   // : 10 for map purpose
    y_distance = y_distance + Gs * (-gDeltaY) / 10;   // : 10 for map purpose


    cout << "angle = " << this->angle << endl;
    cout << "global x = " << x_distance  << endl;
    cout << "global y = " << y_distance  << endl;

    //cout << "rotation = " << rotation << ",  dx = " <<  std::get<0>(paslink) << "    dy=" << std::get<1>(paslink)  << endl;
    return p;
}



void advanced_opencv::get_motion_vector(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
                                            std::vector<Point2f> * features)
{
    vector<uchar> status;
    vector<float> err;

    std::vector<cv::Point2f> features_new, features_curr;

    std::vector<Point2f> new_trac;
    std::vector<Point2f> prev_trac;

    if ( features->size() > 0)
    {
        cv::calcOpticalFlowPyrLK((Mat)prev_gray, (Mat)curr_gray, *features, features_curr, status, err);

 // ================================================= poru sritis ==================================

        for (int i = 0; i < status.size(); i++)
        {
            if (status.at(i) == 1)
            {
                //p.x += curr.at(i).x - prev.at(i).x;
                //p.y += curr.at(i).y - prev.at(i).y;

                new_trac.push_back(features_curr.at(i));
                prev_trac.push_back(features->at(i));

                //cvLine(rgb, prev.at(i), curr.at(i), cvScalar( 255, 0, 255));

            }

        }

        for (int i = 0; i < new_trac.size(); i++)
        {
            cvLine(rgb, prev_trac.at(i), new_trac.at(i), cvScalar( 255, 0, 255));
        }


// ================================================= poru sritis ==================================

        Point_<float> p = motion_by_features(prev_trac, new_trac, status, rgb);
    }
    *features = new_trac;

   // cout << features_curr.size() << endl;
    if (features_curr.size() < OPTICAL_FLOW_MAX_FEATURES)
    {
        IplImage * mask = cvCreateImage(cvGetSize(curr_gray ),IPL_DEPTH_8U, 1);

        cvSet(mask, cvScalar(255));
        for (int i = 0; i < features->size(); i++)
            cvCircle(mask, features->at(i), 10, cvScalar(0), -1);

        cvRectangle(mask, cvPoint(0, CAPTURE_FRAME_HEIGHT /3 ), cvPoint(CAPTURE_FRAME_WIDTH,
            CAPTURE_FRAME_HEIGHT /3 *2), cvScalar(0), -1);

       // cvShowImage("mask", mask);
        cv::goodFeaturesToTrack((Mat)curr_gray, features_new, 100, 0.01, 10, (Mat)mask, 5 );
    }

    for(int i = 0; i < features_new.size(); i ++)
        features->push_back(features_new.at(i));


    for(int i = 0; i < features->size(); i ++)
        cvCircle(rgb, features->at(i), 2, cvScalar(0, 255, 0) );

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
