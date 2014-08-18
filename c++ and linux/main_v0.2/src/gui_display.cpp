#include "gui_display.h"


GtkWidget * gui_display::_darea = NULL;
GdkPixbuf * gui_display::_pixbuf = NULL;

gui_display::gui_display()
{
    _execution_thread = new std::thread (&gui_display::build_gui, this);
}

void gui_display::show_image(IplImage * image)
{
    IplImage * rgb_img = cvCreateImage(cvGetSize(image ),IPL_DEPTH_8U, 3);
    cvtColor( (Mat)image, (Mat)rgb_img, CV_BGR2RGB);


    gdk_draw_rgb_image(GDK_DRAWABLE(_darea->window),
                      _darea->style->fg_gc[GTK_WIDGET_STATE (_darea)],
                      0,0,
                      CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT,
                      GDK_RGB_DITHER_NORMAL,
                      (guint8*)rgb_img->imageData,
                      CAPTURE_FRAME_WIDTH*3);

}


void gui_display::build_gui()
{
    GtkWidget *win = NULL;


    gdk_threads_enter();
    XInitThreads();
    gtk_init( 0, NULL );

    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "Hello World");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);


    /* Create a vertical box with buttons */

    //  _vbox = gtk_hbox_new(FALSE, 6);
    _vbox = gtk_fixed_new(); // fixed container
    gtk_container_add (GTK_CONTAINER (win), _vbox);


    _darea = gtk_drawing_area_new(); //gtk_image_new_from_file("/home/minde/Desktop/1062teeth_LRG.jpg");
  //  gtk_widget_set_usize(_image, CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);
    gtk_drawing_area_size(GTK_DRAWING_AREA(_darea), CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);


  g_signal_connect (G_OBJECT (_darea), "expose_event",
                    G_CALLBACK (expose_event), NULL);

  gtk_signal_connect (GTK_OBJECT(_darea),"configure_event",
                      G_CALLBACK (configure_event), NULL);

    gtk_fixed_put(GTK_FIXED(_vbox), _darea, 0, 0);

    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();

    /* Release gtk's global lock */
    //gdk_threads_leave();
}

static gint gui_display::configure_event (GtkWidget *widget, GdkEventConfigure *event)
{

   _pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, false, 8, CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);


  return TRUE;
}

static gint gui_display::expose_event (GtkWidget *widget, GdkEventExpose *event)
{

  gdk_draw_pixbuf(widget->window,
                  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                  gui_display::_pixbuf,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height,NULL, 0, 0);

  return FALSE;
}

