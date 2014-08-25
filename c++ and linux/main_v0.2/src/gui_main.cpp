#include "gui_main.h"

gui_main::gui_main( gui_display * display, gui_control * control)
{
    _display_widget = display;
    _control_widget = control;

    _execution_thread = new std::thread (&gui_main::build_gui, this);
}


void gui_main::set_control_widget(gui_control * widget)
{

}


void gui_main::set_display_widget(gui_display * display_gui)
{
    _display_widget = display_gui;
    _window = NULL;
}

void gui_main::build_gui()
{
    gdk_threads_enter();
    XInitThreads();
    gtk_init( 0, NULL );

    /* Create the main window */
    _window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (_window), 8);
    gtk_window_set_title (GTK_WINDOW (_window), "JerryPi main window");
    //gtk_window_set_default_size(GTK_WINDOW(_window), 400, 300);
    gtk_window_set_position (GTK_WINDOW (_window), GTK_WIN_POS_CENTER);

    _fixed_box = gtk_fixed_new();
    gtk_widget_set_usize(_fixed_box, 900, 400);


    GtkWidget * frame = gtk_frame_new("Display");
    gtk_container_set_border_width(GTK_CONTAINER (_display_widget->get_main_box()), 10);
    gtk_container_add(GTK_CONTAINER (frame), _display_widget->get_main_box());


    gtk_fixed_put(GTK_FIXED (_fixed_box), frame, 0, 0);


    frame = gtk_frame_new("Control");
    gtk_container_set_border_width(GTK_CONTAINER (_control_widget->get_main_box()), 10);
    gtk_container_add(GTK_CONTAINER (frame), _control_widget->get_main_box());
    gtk_fixed_put(GTK_FIXED (_fixed_box), frame, 460, 0);


    gtk_container_add (GTK_CONTAINER (_window), _fixed_box);

    /* Enter the main loop */
    gtk_widget_show_all (_window);
    gtk_main ();

    /* Release gtk's global lock */
    gdk_threads_leave();
}
