#ifndef GUI_H
#define GUI_H


#include <stdlib.h>
#include <gtk/gtk.h>

#include <thread>

#include "activities/activities.h"

#include <X11/Xlib.h>

using namespace std;

class gui_control
{
    public:
        gui_control(activities * main_act);
        void build_gui (activities * main_act);
        static void drive_forward (GtkWidget *wid, gpointer user_data);
        static void drive_backward (GtkWidget *wid, gpointer user_data);
        static void on_key_press(GtkWidget *wid, GdkEventKey *event, gpointer user_data);
        static void on_key_release(GtkWidget *wid, GdkEventKey *event, gpointer user_data);
        static void turn_right (GtkWidget *wid, gpointer user_data);
        static void turn_left (GtkWidget *wid, gpointer user_data);
        static char _key_on_hold;
        GtkWidget * get_main_box();
        static void toggle_button_callback (GtkWidget *widget, gpointer data);

        GtkWidget * drive_scale;
        GtkWidget * turn_scale;

    protected:
    private:
        thread  * _execution_thread;

        static GtkWidget * _checkbox_stop;
        static activities * _main_act;
        GtkWidget * _fixed_box;
};

#endif // GUI_H
