#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include <project_constants.h>

#include <stdlib.h>
#include <gtk/gtk.h>

#include <thread>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>

#include <X11/Xlib.h>

using namespace cv;
using namespace std;

class gui_display
{
    public:
        gui_display();
        void show_image(IplImage * image);
        void set_distance(int data, int index);

        static GtkWidget * _darea;
        static GdkPixbuf * _pixbuf;

        static GtkWidget * _front_dist;
        static GtkWidget * _rear_dist;

        static gint expose_event (GtkWidget *widget, GdkEventExpose *event);
        static gint configure_event (GtkWidget *widget,  GdkEventConfigure *event);
        GtkWidget * get_main_box();
    protected:
    private:
        void build_gui();
        std::thread  * _execution_thread;
        GtkWidget * _fixed_box;

};

#endif // GUI_DISPLAY_H
