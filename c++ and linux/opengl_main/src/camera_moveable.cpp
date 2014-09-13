#include "camera_moveable.h"

camera_moveable::camera_moveable() : movable()
{
    mouse_last_x = 0;
    mouse_last_y = 0;
    mouse_on_hold = false;

    up_x = 0;
    up_y = 0;
    up_z = 0;

    go_to_start();
}

void camera_moveable::position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
    init(pos_x, pos_y, pos_z, view_x, view_y, view_z);
    this->up_x = up_x; this->up_y = up_y; this->up_z = up_z;
}


void camera_moveable::print()
{
    go_to_start();

    gluLookAt(pos_x, pos_y, pos_z,
                view_x, view_y, view_z,
                up_x, up_y, up_z);
}


void camera_moveable::go_to_start()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, 1.0f ,0.1f,1000.0f);
}


void camera_moveable::mouse_press(int x, int y)
{
    mouse_on_hold = true;

    mouse_last_x = x;
    mouse_last_y = y;
}

void camera_moveable::mouse_release()
{
    mouse_on_hold = false;
}

void camera_moveable::mouse_drag(int x, int y)
{
    if (!mouse_on_hold) return;

    int diff_x = mouse_last_x - x;
    int diff_y = mouse_last_y - y;

    float angle_x = (float)diff_x / 300;
	float angle_z = (float)diff_y / 300;

   // mView.x += angle_x * 2;
    view_z = view_z + angle_z * 5 ;

	if((view_z - pos_z) > 8)  view_z = pos_z + 8;
	if((view_z - pos_z) <-8)  view_z = pos_z - 8; // difference must be small, z current z view


    rotate_z(-angle_x);

    mouse_last_x = x;
    mouse_last_y = y;
}
