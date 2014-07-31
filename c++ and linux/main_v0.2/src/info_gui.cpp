#include "info_gui.h"

info_gui::info_gui ()
{
    _execution_thread = new thread (&info_gui::build_gui, this);
}


void info_gui::build_gui ()
{
  GtkWidget * win = NULL;

  gtk_init(0, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Hello World");
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);

  gtk_widget_realize (win);

  // image widget
  //GdkPixbuf *pix = gdk_pixbuf_n   (GDK_COLORSPACE_RGB, FALSE, 8, CAPTURE_FRAME_WIDTH, CAPTURE_FRAME_HEIGHT);
  //_image = gtk_image_new_from_pixbuf (pix);
  gtk_container_add (GTK_CONTAINER (win), _image);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();

}



void info_gui::show_image(IplImage * image)
{
    GtkWidget *gtkImg;
    GdkPixbuf *pix;
    pix = gdk_pixbuf_new_from_data((guchar*)image->imageData, GDK_COLORSPACE_RGB, FALSE, image->depth,
    image->width, image->height, (image->widthStep), NULL, NULL);
    _image = gtk_image_new_from_pixbuf(pix);


   //  gdk_draw_pixbuf(widget->window,
   //   widget->style->fg_gc[GTK_WIDGET_STATE (widget)], pix, 0, 0, 0, 0,
  //    opencvImage->width, opencvImage->height, GDK_RGB_DITHER_NONE, 0, 0);

    gtk_container_add (GTK_CONTAINER (win), _image);

}

