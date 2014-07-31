#include "gui.h"

/*
    Static fields must be declared in header file
    and initialized in source file
*/
char gui::_key_on_hold = 0;

gui::gui(activities main_act)
{
    _execution_thread = new thread (&gui::build_gui, this, &main_act);
}

static void gui::drive_forward (GtkWidget *wid, gpointer user_data)
{

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->drive(FORWARD, 1);

}

static void gui::drive_backward (GtkWidget *wid, gpointer user_data)
{
    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->drive(BACKWARD, 1);
    _key_on_hold = 's';
}

static void gui::turn_left (GtkWidget *wid, gpointer user_data)
{
    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->turn(TURN_LEFT, 1);
}

static void gui::turn_right (GtkWidget *wid, gpointer user_data)
{

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->turn(TURN_RIGHT, 1);

}

static void gui::on_key_press(GtkWidget *wid, GdkEventKey *event, gpointer user_data)
{
    if (_key_on_hold != 0)
    {
        cout << "on hold " << (char)event->keyval << endl;
        return;
    }

    cout << "key presed " << (char)event->keyval << endl;

    switch (event->keyval)
    {
        case 'w' :
        _key_on_hold = 'w';

        drive_forward(wid, user_data);

        break;
        case 's' :
        drive_backward(wid, user_data);
        _key_on_hold = 's';
        break;
        case 'a' :
        turn_left(wid, user_data);
        _key_on_hold = 'a';
        break;
        case 'd' :
        turn_right(wid, user_data);
        _key_on_hold = 'd';
        break;
    }
}

static void gui::on_key_release(GtkWidget *wid, GdkEventKey *event, gpointer user_data)
{
    _key_on_hold = 0;
    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }
    activities * main_act = user_data;
    cout << "  stop!" << endl;
    main_act->force_stop();
}

void gui::build_gui (activities * main_act)
{
  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (0, NULL);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Hello World");
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);

  gtk_widget_realize (win);
  //g_signal_connect (win, "destroy", G_CALLBACK (&destroy), NULL);
  //g_signal_connect (win, "delete-event", G_CALLBACK (&delete_event), NULL);

  g_signal_connect (G_OBJECT (win), "key_press_event", G_CALLBACK (gui::on_key_press), (gpointer)main_act);
  g_signal_connect (G_OBJECT (win), "key_release_event", G_CALLBACK (gui::on_key_release), (gpointer)main_act);

  /* Create a vertical box with buttons */

//  vbox = gtk_hbox_new(FALSE, 6);
  vbox = gtk_fixed_new(); // fixed container
  gtk_container_add (GTK_CONTAINER (win), vbox);


  int x = 250, y = 50; // WSAD position
  int wid = 40, hei = 30; // WSAD buttons size
  int shift_h = 40, shift_v = 30; // WSAD spaces

  // forward button
  button = gtk_button_new_with_label ("W");
  g_signal_connect (button, "clicked", G_CALLBACK(gui::drive_forward), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(vbox), button, x, y);

  // backward button
  button = gtk_button_new_with_label ("S");
  g_signal_connect (button, "clicked", G_CALLBACK (gui::drive_backward), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(vbox), button, x, y+shift_v);

  // turn left button
  button = gtk_button_new_with_label ("A");
  g_signal_connect (button, "clicked", G_CALLBACK (gui::turn_left), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(vbox), button, x-shift_h, y+shift_v);

  // turn right button
  button = gtk_button_new_with_label ("D");
  g_signal_connect (button, "clicked", G_CALLBACK (gui::turn_right), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(vbox), button, x+shift_h, y+shift_v);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();

}
