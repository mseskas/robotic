#include "gui_control.h"

/*
    Static fields must be declared in header file
    and initialized in source file
*/
char gui_control::_key_on_hold = 0;
GtkWidget * gui_control::_checkbox_stop = NULL;
activities * gui_control::_main_act =NULL;


gui_control::gui_control(activities * main_act)
{
    _main_act = main_act;
    build_gui(main_act);
}

static void gui_control::drive_forward (GtkWidget *wid, gpointer user_data)
{
    cout << " drive forward " << endl;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->drive(FORWARD, 10);

}

static void gui_control::drive_backward (GtkWidget *wid, gpointer user_data)
{
    cout << " drive backward " << endl;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->drive(BACKWARD, 10);
    _key_on_hold = 's';
}

static void gui_control::turn_left (GtkWidget *wid, gpointer user_data)
{
    cout << " turn left " << endl;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->turn(TURN_LEFT, 10);
}

static void gui_control::turn_right (GtkWidget *wid, gpointer user_data)
{
    cout << " turn right " << endl;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }

    activities * main_act = user_data;

    main_act->turn(TURN_RIGHT, 10);

}

static void gui_control::on_key_press(GtkWidget *wid, GdkEventKey *event, gpointer user_data)
{
    if (_key_on_hold == event->keyval)
    {
        cout << "on hold " << (char)event->keyval << endl;
        return;
    }

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
        case 65362 :
        _key_on_hold = 'UP';
        drive_forward(wid, user_data);
        break;
        case 65364 :
        drive_backward(wid, user_data);
        _key_on_hold = 'DOWN';
        break;
        case 65361 :
        turn_left(wid, user_data);
        _key_on_hold = 'LEFT';
        break;
        case 65363 :
        turn_right(wid, user_data);
        _key_on_hold = 'RIGHT';
        break;
        case 65456 :  // 0 pressed
            cout << "  stop!" << endl;
            activities * main_act = user_data;
            _key_on_hold = 0;
            main_act->stop_drive();
        break;
    }
}

static void gui_control::on_key_release(GtkWidget *wid, GdkEventKey *event, gpointer user_data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON( _checkbox_stop)) == TRUE )  // release ignored, waiting for space
        return;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }
    activities * main_act = user_data;

    _key_on_hold = 0;
    cout << "  stop!" << endl;
    main_act->stop_drive();
}

void gui_control::toggle_button_callback (GtkWidget *widget, gpointer data)
{
    /* If  toggle button is down */
    if (GTK_TOGGLE_BUTTON (widget)->active)
    {
        if (data == NULL)
            return;
        char * radio = data;
        string str(radio);


        if ( str.compare("none") == 0)
        {
            _main_act->force_stop();
        }
        else if ( str.compare("RGB") == 0)
        {
            _main_act->act(3);
        }
        else if ( str.compare("canny") == 0)
        {
            _main_act->act(4);
        }
        else if ( str.compare("HSV") == 0)
        {
            _main_act->act(5);
        }
        else if ( str.compare("of") == 0)
        {
            _main_act->act(9);
        }




    }
}

void gui_control::build_gui (activities * main_act)
{
  _fixed_box = gtk_fixed_new();

  g_signal_connect (_fixed_box, "key_press_event", G_CALLBACK (gui_control::on_key_press), (gpointer)main_act);
  g_signal_connect (_fixed_box, "key_release_event", G_CALLBACK (gui_control::on_key_release), (gpointer)main_act);


  int x = 250, y = 50; // WSAD position
  int wid = 80, hei = 30; // WSAD buttons size
  int shift_h = 80, shift_v = 30; // WSAD spaces

  // forward button
  GtkWidget * button = gtk_button_new_with_label ("W/up");
  g_signal_connect (button, "clicked", G_CALLBACK(gui_control::drive_forward), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(_fixed_box), button, x, y);

  // backward button
  button = gtk_button_new_with_label ("S/down");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::drive_backward), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(_fixed_box), button, x, y+shift_v);

  // turn left button
  button = gtk_button_new_with_label ("A/left");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::turn_left), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(_fixed_box), button, x-shift_h, y+shift_v);

  // turn right button
  button = gtk_button_new_with_label ("D/right");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::turn_right), (gpointer)main_act);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(_fixed_box), button, x+shift_h, y+shift_v);

  // checkbox
  _checkbox_stop = gtk_check_button_new_with_label("stop on '0' key");
  gtk_fixed_put(GTK_FIXED(_fixed_box), _checkbox_stop, x, y-30);

  //redio buttons
  GtkWidget * radio_none = gtk_radio_button_new_with_label(NULL, "none");
  GSList * group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_none));
  g_signal_connect (radio_none, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "none");
  gtk_fixed_put(GTK_FIXED(_fixed_box), radio_none, 10, 10);

  GtkWidget * radio_RGB = gtk_radio_button_new_with_label(group, "RGB");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_RGB));
    g_signal_connect (radio_RGB, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "RGB");
  gtk_fixed_put(GTK_FIXED(_fixed_box), radio_RGB, 10, 35);

  GtkWidget * radio_canny = gtk_radio_button_new_with_label(group, "Canny edge");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_canny));
    g_signal_connect (radio_canny, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "canny");
  gtk_fixed_put(GTK_FIXED(_fixed_box), radio_canny, 10, 60);

  GtkWidget * radio_HSV = gtk_radio_button_new_with_label(group, "HSV");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_HSV));
    g_signal_connect (radio_HSV, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "HSV");
  gtk_fixed_put(GTK_FIXED(_fixed_box), radio_HSV, 10, 85);

  GtkWidget * radio_of = gtk_radio_button_new_with_label(group, "optical flow");
    g_signal_connect (radio_of, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "of");
  gtk_fixed_put(GTK_FIXED(_fixed_box), radio_of, 10, 110);


}


GtkWidget * gui_control::get_main_box()
{
    return _fixed_box;
}

