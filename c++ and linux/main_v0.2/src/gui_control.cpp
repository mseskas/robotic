#include "gui_control.h"

/*
    Static fields must be declared in header file
    and initialized in source file
*/
char gui_control::_key_on_hold = 0;
GtkWidget * gui_control::_checkbox_stop = NULL;
activities * gui_control::_main_act = NULL;
drivetrain * gui_control::_drivetrain = NULL;
GtkWidget * gui_control::drive_scale = NULL;
GtkWidget * gui_control::turn_scale = NULL;


gui_control::gui_control(activities * main_act)
{
    _main_act = main_act;
    build_gui(main_act);
}

void gui_control::set_drivetrain(drivetrain * drv)
{
    _drivetrain = drv;
}

static void gui_control::drive_forward (GtkWidget *wid, gpointer user_data)
{
    cout << " drive forward " << endl;
    if (_drivetrain == NULL)
    {
        cout << "gui drivetrain == null" << endl;
        return;
    }
    _drivetrain->a_drive(FORWARD, 10);
}

static void gui_control::drive_backward (GtkWidget *wid, gpointer user_data)
{
    cout << " drive backward " << endl;
    if (_drivetrain == NULL)
    {
        cout << "gui drivetrain == null" << endl;
        return;
    }
    _drivetrain->a_drive(BACKWARD, 10);
}

static void gui_control::turn_left (GtkWidget *wid, gpointer user_data)
{
    cout << " turn left " << endl;
    if (_drivetrain == NULL)
    {
        cout << "gui drivetrain == null" << endl;
        return;
    }
    _drivetrain->a_turn(TURN_LEFT, 10);
}

static void gui_control::turn_right (GtkWidget *wid, gpointer user_data)
{
    cout << " turn right " << endl;
    if (_drivetrain == NULL)
    {
        cout << "gui drivetrain == null" << endl;
        return;
    }
    _drivetrain->a_turn(TURN_RIGHT, 10);
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
        case 65450 :  // '*'
        {
            cout << "higher turn speed" << endl;
            double curr_speed = 0.0;
            if (_drivetrain)
            {
                float curr_speed = _drivetrain->get_turn_speed();
                curr_speed += 0.025;
                _drivetrain->set_turn_speed(curr_speed);
                gtk_range_set_value(GTK_RANGE(turn_scale), (double)curr_speed);
            }
        }
        break;
        case 65455 :  // '/'
        {
            cout << "lower turn speed" << endl;
            double curr_speed = 0.0;
            if (_drivetrain)
            {
                float curr_speed = _drivetrain->get_turn_speed();
                curr_speed += 0.025;
                _drivetrain->set_turn_speed(curr_speed);
                gtk_range_set_value(GTK_RANGE(turn_scale), (double)curr_speed);
            }
        }
        break;
        case 65451 :  // '+'
        {
            cout << "higher drive speed" << endl;
            double curr_speed = 0.0;
            if (_drivetrain)
            {
                float curr_speed = _drivetrain->get_drive_speed();
                curr_speed += 0.025;
                _drivetrain->set_drive_speed(curr_speed);
                gtk_range_set_value(GTK_RANGE(drive_scale), (double)curr_speed);
            }
        }
        break;
        case 65453 :  // '-'
        {
            cout << "lower drive speed" << endl;
            double curr_speed = 0.0;
            if (_drivetrain)
            {
                float curr_speed = _drivetrain->get_drive_speed();
                curr_speed -= 0.025;
                _drivetrain->set_drive_speed(curr_speed);
                gtk_range_set_value(GTK_RANGE(drive_scale), (double)curr_speed);
            }
        }
        break;
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
            if (_drivetrain != NULL)
            {
                cout << "  stop!" << endl;
                _drivetrain->force_stop();
            }
            else
                cout << "gui drivetrain == null" << endl;
            _key_on_hold = 0;
        break;
        case ' ' :  // space pressed
            if (_drivetrain != NULL)
            {
                cout << "  stop!" << endl;
                _drivetrain->force_stop();
            }
            else
                cout << "gui drivetrain == null" << endl;
            _key_on_hold = 0;
        break;
    }
}

static void gui_control::on_key_release(GtkWidget *wid, GdkEventKey *event, gpointer user_data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON( _checkbox_stop)) == TRUE )  // release ignored, waiting for space
        return;

    if ((event->keyval >= 65450) && (event->keyval <= 65455))
        return;

    if (user_data == NULL)
    {
        cout << "user_data == null" << endl;
        return;
    }
    if (_drivetrain != NULL)
    {
        cout << "  stop!" << endl;
        _drivetrain->force_stop();
    }
    else
        cout << "gui drivetrain == null" << endl;

    _key_on_hold = 0;
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

static void gui_control::set_speed(GtkWidget *wid, gpointer data)
{
    if (data == NULL)
            return;

    if (_drivetrain == NULL)
    {
        cout << "gui drivetrain == null" << endl;
        return;
    }

    char * mode = data;
    string str(mode);
    //drive_scale  turn_scale
    if ( str.compare("drive") == 0)
    {
        gdouble current_value = gtk_range_get_value(GTK_RANGE(drive_scale));
        _drivetrain->set_drive_speed((float)current_value);
    }
    else if ( str.compare("turn") == 0)
    {
        gdouble current_value = gtk_range_get_value(GTK_RANGE(turn_scale));
        _drivetrain->set_turn_speed((float)current_value);
    }
}

void gui_control::build_gui (activities * main_act)
{
  _fixed_box = gtk_fixed_new();
  g_signal_connect (_fixed_box, "key_press_event", G_CALLBACK (gui_control::on_key_press), (gpointer)main_act);
  g_signal_connect (_fixed_box, "key_release_event", G_CALLBACK (gui_control::on_key_release), (gpointer)main_act);

  // MOVE frame
  GtkWidget * move_box = gtk_fixed_new();
  gtk_widget_set_usize(move_box, 280, -1);  // set only width
  GtkWidget * frame = gtk_frame_new("Drive");

  gtk_container_set_border_width(GTK_CONTAINER (move_box), 5);
  gtk_container_add(GTK_CONTAINER (frame), move_box);

  int move_x = 125, move_y = 0; // WSAD position
  int wid = 80, hei = 30; // WSAD buttons size
  int shift_h = 80, shift_v = 30; // WSAD spaces

  // forward button
  GtkWidget * button = gtk_button_new_with_label ("W/up");
  g_signal_connect (button, "clicked", G_CALLBACK(gui_control::drive_forward), NULL);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(move_box), button, 0+shift_h, +30);

  // backward button
  button = gtk_button_new_with_label ("S/down");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::drive_backward), NULL);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(move_box), button, 0+shift_h, shift_v+30);

  // turn left button
  button = gtk_button_new_with_label ("A/left");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::turn_left), NULL);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(move_box), button, 0, shift_v+30);

  // turn right button
  button = gtk_button_new_with_label ("D/right");
  g_signal_connect (button, "clicked", G_CALLBACK (gui_control::turn_right), NULL);
  gtk_widget_set_usize(button, wid, hei);
  gtk_fixed_put(GTK_FIXED(move_box), button, 2*shift_h, shift_v+30);

  // checkbox
  _checkbox_stop = gtk_check_button_new_with_label("stop on '0' or SPACE");
  gtk_fixed_put(GTK_FIXED(move_box), _checkbox_stop, 0, 0);

  gtk_fixed_put(GTK_FIXED (_fixed_box), frame, move_x, move_y);
  // end of MOVE

  // add frame with fixed radio widget
  GtkWidget * radio_box = gtk_fixed_new();
  frame = gtk_frame_new("Camera");

  gtk_container_set_border_width(GTK_CONTAINER (radio_box), 5);
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
  GtkWidget * speed_box = gtk_fixed_new();
  GtkWidget * temp_frame = gtk_frame_new("Speed");

  gtk_container_set_border_width(GTK_CONTAINER (speed_box), 5);
  gtk_container_add(GTK_CONTAINER (temp_frame), speed_box);

  int _speed_box_x_ext = 125;
  int _speed_box_y_ext = 125;

  drive_scale = gtk_hscale_new_with_range(0, 1, 0.05);
  gtk_widget_set_usize(drive_scale, 150, 45);
  gtk_fixed_put(GTK_FIXED (speed_box), drive_scale, 45, 0);

  turn_scale = gtk_hscale_new_with_range(0, 1, 0.05);
  gtk_widget_set_usize(turn_scale, 150, 45);
  gtk_fixed_put(GTK_FIXED (speed_box), turn_scale, 45, 45);

  GtkWidget * _set_drive_btn = gtk_button_new_with_label("Set");
  g_signal_connect (_set_drive_btn, "clicked", G_CALLBACK (gui_control::set_speed), "drive");
  gtk_widget_set_usize(_set_drive_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED (speed_box), _set_drive_btn, 200, 10);

  GtkWidget * _set_turn_btn = gtk_button_new_with_label("Set");
    g_signal_connect (_set_turn_btn, "clicked", G_CALLBACK (gui_control::set_speed), "turn");
  gtk_widget_set_usize(_set_turn_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED (speed_box), _set_turn_btn, 200, 55);

  GtkWidget * label = gtk_label_new("drive");
  gtk_fixed_put(GTK_FIXED (speed_box), label, 0, 20);

  label = gtk_label_new("turn");
  gtk_fixed_put(GTK_FIXED (speed_box), label, 0, 65);

  gtk_fixed_put(GTK_FIXED (_fixed_box), temp_frame, _speed_box_x_ext, _speed_box_y_ext);
}


GtkWidget * gui_control::get_main_box()
{
    return _fixed_box;
}

