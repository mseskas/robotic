#ifndef INFO_GUI_H
#define INFO_GUI_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>

#include <stdlib.h>
#include <gtk/gtk.h>

#include <thread>

#include <project_constants.h>

using namespace std;

class info_gui
{
    public:
        info_gui();
        void show_image(IplImage * image);
        void build_gui ();
    protected:
    private:
        GtkWidget * _image;
        thread  * _execution_thread;
        GtkWidget * win;
};

#endif // INFO_GUI_H
