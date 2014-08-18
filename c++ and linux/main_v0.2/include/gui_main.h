#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include <X11/Xlib.h>

#include <thread>

#include <gui_display.h>
#include <gui_control.h>

class gui_main
{
    public:
        gui_main(gui_display * display, gui_control * control);

        void set_display_widget(gui_display * display_gui);
        void set_control_widget(gui_control * widget);

        void build_gui();
    protected:
    private:
        gui_display * _display_widget;
        gui_control * _control_widget;
        GtkWidget * _window;
        GtkWidget * _fixed_box;
        std::thread * _execution_thread;
};

#endif // GUI_MAIN_H
