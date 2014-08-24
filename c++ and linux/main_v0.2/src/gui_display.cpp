#include "gui_display.h"


GtkWidget * gui_display::_darea = NULL;
GdkPixbuf * gui_display::_pixbuf = NULL;
GtkWidget * gui_display::_front_dist;
GtkWidget * gui_display::_rear_dist;

gui_display::gui_display()
{
    gui_display::build_gui();
}

void gui_display::show_image(IplImage * image)
{
    if (image == NULL)
        return;

    int channels = image->nChannels;

    IplImage * copy_img = cvCreateImage(cvGetSize(image ),IPL_DEPTH_8U, channels);

    if (channels == 3)
    {
        cvtColor( (Mat)image, (Mat)copy_img, CV_BGR2RGB);
        gdk_draw_rgb_image(GDK_DRAWABLE(_darea->window),
                      _darea->style->fg_gc[GTK_WIDGET_STATE (_darea)],
                      0,0,
                      CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT,
                      GDK_RGB_DITHER_NORMAL,
                      (guint8*)copy_img->imageData,
                      CAPTURE_FRAME_WIDTH*3);
    }
    else if (channels == 1)
    {
        copy_img = cvCloneImage(image);
        gdk_draw_gray_image(_darea->window,
                        _darea->style->fg_gc[GTK_WIDGET_STATE (_darea)],
                        0, 0, CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT,
                        GDK_RGB_DITHER_NORMAL, (guchar*)copy_img->imageData,
                        CAPTURE_FRAME_WIDTH);
    }

}

void gui_display::set_distance(int data, int index)
{
    char buf[20];
    sprintf(buf, "%d", data);

    switch(index)
    {
        case 1 :
            gtk_label_set_text(GTK_LABEL(gui_display::_front_dist), buf);
        break;
        case 2 :
            gtk_label_set_text(GTK_LABEL(gui_display::_rear_dist), buf);
        break;
        default:
            printf("undefined sonar sensor\n");
        break;
    }

}

void gui_display::build_gui()
{

    gdk_threads_enter();
    XInitThreads();
    gtk_init( 0, NULL );

    _fixed_box = gtk_fixed_new(); // fixed container

    // draw area
    _darea = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(_darea), CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);
    g_signal_connect (G_OBJECT (_darea), "expose_event",
                    G_CALLBACK (expose_event), NULL);

    gtk_signal_connect (GTK_OBJECT(_darea),"configure_event",
                      G_CALLBACK (configure_event), NULL);

    gtk_fixed_put(GTK_FIXED(_fixed_box), _darea, 0, 0);

    // labels
    _front_dist = gtk_label_new("front");
        gtk_fixed_put(GTK_FIXED(_fixed_box), _front_dist, 50, CAPTURE_FRAME_HEIGHT+10);

    _rear_dist = gtk_label_new("rear");
    gtk_fixed_put(GTK_FIXED(_fixed_box), _rear_dist, 50, CAPTURE_FRAME_HEIGHT+245);


    // robot image
    GdkPixbuf * img = gdk_pixbuf_new_from_file("robot.png", NULL);
    img = gdk_pixbuf_scale_simple(img, 100, 200, GDK_INTERP_BILINEAR);
    GtkWidget * image = gtk_image_new_from_pixbuf(img);

    gtk_fixed_put(GTK_FIXED(_fixed_box), image, 20, CAPTURE_FRAME_HEIGHT+40);

}

GtkWidget *  gui_display::get_main_box()
{
    return _fixed_box;
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

