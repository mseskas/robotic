#include <iostream>
#include <string>


#include "activities/activities.h"

#include <stdlib.h>
#include <gtk/gtk.h>


using namespace std;

static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void drive_forward (GtkWidget *wid, gpointer user_data)
{
    activities * main_act = user_data;

    main_act->drive(FORWARD, 1);
}

int gui (activities * main_act)
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
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  /* Create a vertical box with buttons */
  vbox = gtk_vbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  button = gtk_button_new_from_stock (GTK_STOCK_DIALOG_INFO);
  int n =10;
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK(drive_forward), (gpointer)main_act);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();


  return 0;
}

#include <thread>

int main()
{
    thread  * _execution_thread;

    activities main_act;

    _execution_thread = new thread (gui, &main_act);


    //gui(main_act);


   // time_t t = time(0);   // get time now
   // struct tm * now = localtime( & t );

   // string datetime = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-"+ to_string(now->tm_mday)
   //             + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min)  + ":" + to_string(now->tm_sec);





    int cmd = 10000;

    while(true)
    {

        main_act.print_activities();
        cout << "insert new activity - ";
        cin >> cmd;
        cout << endl;

        if (0 == cmd)  // closes program at 0 or any key than is not a number
            break;

        main_act.act(cmd);
    }

    //main_act.~activities();   - called in every return statement
    return 0;
}
