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
        else if ( str.compare("floor") == 0)
        {
            _main_act->act(6);
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


  // add frame with fixed radio widget
  GtkWidget * radio_box = gtk_fixed_new();
  GtkWidget * frame = gtk_frame_new("Camera");

  gtk_container_set_border_width(GTK_CONTAINER (radio_box), 10);
  gtk_container_add(GTK_CONTAINER (frame), radio_box);

  //redio buttons
  GtkWidget * radio_none = gtk_radio_button_new_with_label(NULL, "none");
  GSList * group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_none));
  g_signal_connect (radio_none, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "none");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_none, 0, 0);

  GtkWidget * radio_RGB = gtk_radio_button_new_with_label(group, "RGB");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_RGB));
    g_signal_connect (radio_RGB, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "RGB");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_RGB, 0, 25);

  GtkWidget * radio_canny = gtk_radio_button_new_with_label(group, "Canny edge");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_canny));
    g_signal_connect (radio_canny, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "canny");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_canny, 0, 50);

  GtkWidget * radio_HSV = gtk_radio_button_new_with_label(group, "HSV");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_HSV));
    g_signal_connect (radio_HSV, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "HSV");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_HSV, 0, 75);

  GtkWidget * radio_floor = gtk_radio_button_new_with_label(group, "floor");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio_floor));
    g_signal_connect (radio_floor, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "floor");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_floor, 0, 100);

  GtkWidget * radio_of = gtk_radio_button_new_with_label(group, "optical flow");
    g_signal_connect (radio_of, "toggled", G_CALLBACK (gui_control::toggle_button_callback), "of");
  gtk_fixed_put(GTK_FIXED(radio_box), radio_of, 0, 125);

  gtk_fixed_put(GTK_FIXED (_fixed_box), frame, 0, 0);

  // scales
  int _speed_box_x = 140;
  int _speed_box_y = 125;

  drive_scale = gtk_hscale_new_with_range(0, 1, 0.05);
  gtk_widget_set_usize(drive_scale, 150, 45);
  gtk_fixed_put(GTK_FIXED (_fixed_box), drive_scale, _speed_box_x+45, _speed_box_y);

  turn_scale = gtk_hscale_new_with_range(0, 1, 0.05);
  gtk_widget_set_usize(turn_scale, 150, 45);
  gtk_fixed_put(GTK_FIXED (_fixed_box), turn_scale, _speed_box_x+45, _speed_box_y+45);

  GtkWidget * _set_drive_btn = gtk_button_new_with_label("Set");
  g_signal_connect (_set_drive_btn, "clicked", G_CALLBACK (gui_control::drive_backward), NULL);
  gtk_widget_set_usize(_set_drive_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED (_fixed_box), _set_drive_btn, _speed_box_x+200, _speed_box_y+10);

  GtkWidget * _set_turn_btn = gtk_button_new_with_label("Set");
    g_signal_connect (_set_turn_btn, "clicked", G_CALLBACK (gui_control::drive_backward), NULL);
  gtk_widget_set_usize(_set_turn_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED (_fixed_box), _set_turn_btn, _speed_box_x+200, _speed_box_y+55);

  GtkWidget * label = gtk_label_new("drive");
  gtk_fixed_put(GTK_FIXED (_fixed_box), label, _speed_box_x, _speed_box_y+20);

  label = gtk_label_new("turn");
  gtk_fixed_put(GTK_FIXED (_fixed_box), label, _speed_box_x, _speed_box_y+65);
}


GtkWidget * gui_control::get_main_box()
{
    return _fixed_box;
}

