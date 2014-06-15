#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <list>
#include <vector>

#include <project_constants.h>

using namespace std;
using namespace cv;

struct Two_points
{
    Point2f start;
    Point2f end;
};

class advanced_opencv
{
public:
    advanced_opencv();
    ~advanced_opencv();
    IplImage * GetThresholdedImage(IplImage* imgHSV, CvScalar hvs);
    CvScalar * get_bottom_line_pixel_mean(IplImage * img);
    CvScalar *  mark_line(IplImage* img, IplImage* rezimg);
    //cv::Point_<float>  get_motion_vector(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
    //                                    std::vector<Two_points> * features);
    IplImage * create_GRAY_by_RGB(IplImage* RGB_img);

    void get_motion_vector(IplImage* rgb, IplImage* prev_gray, IplImage* curr_gray,
                                            std::vector<Point2f> * features);
    float angle, y_distance, x_distance;
protected:
private:
    cv::Point_<float> motion_by_features(std::vector<Point2f> prev, std::vector<Point2f> curr,
        vector<uchar> status, IplImage* rgb);

};
