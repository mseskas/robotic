

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

IplImage* advanced_opencv::GetThresholdedImage(IplImage* imgHSV, CvScalar hsv)
{
    int Hstep =10;
    int Sstep = 25;
    int Vstep = 25;

    double h0 = hsv.val[0] - Hstep, s0 = hsv.val[1] - Sstep, v0 = hsv.val[2] - Vstep;
    double h1 = hsv.val[0] + Hstep, s1 = hsv.val[1] + Sstep, v1 = hsv.val[2] + Vstep;

   // if (h0 < 0) h0 =0; if (s0 < 0) s0 =0; if (v0 < 0) v0 =0;
    //if (h1 > 180) h0 =180; if (s1 > 255) s1 =255; if (v1 > 255) v1 =255;

    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS ( imgHSV, cvScalar(h0, s0, v0), cvScalar(h1, s1, v1), imgThresh);
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
            pix1 += *p; p++;
            pix2 += *p; p++;
            pix3 += *p; p -= 2;
            quantity++;
        }
        scal.val[0] = pix1 / quantity;
        scal.val[1] = pix2 / quantity;
        scal.val[2] = pix3 / quantity;

        return &scal;
}

// image must bi single channel
void advanced_opencv::mark_line(IplImage * img, IplImage* rezimg)
{
    int curr_y = img->height-1;
        for (int x = 1; x < img->width ; x += 1 )
        {
            for (int y = curr_y; y > 0; y -= 1) // downgrade loop
            {
                if ((CV_IMAGE_ELEM(img, uchar, y,x) == 0) && (CV_IMAGE_ELEM(img, uchar, y-1,x) == 0)
                && (CV_IMAGE_ELEM(img, uchar, y-2,x) == 0))
                {// at(y, x)
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


                    break;
                }
                else
                {

                }
            }

        }
}
