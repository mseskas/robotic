#include "gui_display.h"


GtkWidget * gui_display::_darea = NULL;
GdkPixbuf * gui_display::_pixbuf = NULL;

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


void gui_display::build_gui()
{
    _fixed_box = gtk_fixed_new(); // fixed container

    _darea = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(_darea), CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);
    g_signal_connect (G_OBJECT (_darea), "expose_event",
                    G_CALLBACK (expose_event), NULL);

    gtk_signal_connect (GTK_OBJECT(_darea),"configure_event",
                      G_CALLBACK (configure_event), NULL);

    gtk_fixed_put(GTK_FIXED(_fixed_box), _darea, 0, 0);

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

