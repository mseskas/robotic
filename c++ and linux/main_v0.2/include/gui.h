#ifndef GUI_H
#define GUI_H


#include <stdlib.h>
#include <gtk/gtk.h>

#include <thread>

#include "activities/activities.h"

using namespace std;

class gui
{
    public:
        gui(activities * main_act);
        void build_gui (activities * main_act);
        static void drive_forward (GtkWidget *wid, gpointer user_data);
        static void drive_backward (GtkWidget *wid, gpointer user_data);
        static void on_key_press(GtkWidget *wid, GdkEventKey *event, gpointer user_data);
        static void on_key_release(GtkWidget *wid, GdkEventKey *event, gpointer user_data);
        static void turn_right (GtkWidget *wid, gpointer user_data);
        static void turn_left (GtkWidget *wid, gpointer user_data);

        static char _key_on_hold;
    protected:
    private:
        thread  * _execution_thread;

        static GtkWidget * _checkbox_stop;

};

#endif // GUI_H
