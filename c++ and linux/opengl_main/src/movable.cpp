#include "movable.h"

movable::movable()
{
    sc_x = 1; sc_y = 1; sc_z =1;

    pos_x = 0;
    pos_y = 0;
    pos_z = 0;

    set_view(0, 1, 0);

}

void movable::translate(float x, float y, float z)
{
    pos_x += x;
    pos_y += y;
    pos_z += z;

    view_x += x;
    view_y += y;
    view_z += z;
}

void movable::init(float pos_x, float pos_y,float pos_z,
     float view_x, float view_y, float view_z)
{
   this->pos_x = pos_x; this->pos_y = pos_y; this->pos_z = pos_z;
   set_view(view_x, view_y, view_z);
}

void movable::set_view(float x, float y, float z)
{
    this->view_x = x; this->view_y = y; this->view_z = z;
    count_angles();
}

void movable::count_angles()
{

    float x = view_x - pos_x;
    float y = view_y - pos_y;
    float z = view_z - pos_z;

    angle_z = -atan2 (x,y) * 180 / M_PI;

//    angle_x = -atan2 (y,z) * 180 / M_PI;
}

void movable::move_forward(float distance)
{
    float temp_x = view_x - pos_x;
    float temp_y = view_y - pos_y;
    float temp_z = view_z - pos_z;

    pos_x +=  temp_x * distance;
    pos_y +=  temp_y * distance;
    view_x += temp_x * distance;
    view_y += temp_y * distance;
}


void movable::rotate_z(float angle)
{
    float temp_x = view_x - pos_x;
    float temp_y = view_y - pos_y;

    float s = sin(angle);
    float c = cos(angle);


    view_x = pos_x + c*temp_x - s*temp_y;
    view_y = pos_y + s*temp_x + c*temp_y;

    count_angles();
}

void movable::rotate_x(float angle)
{

    float temp_y = view_y - pos_y;
    float temp_z = view_z - pos_z;

    float s = sin(angle);
    float c = cos(angle);

    view_y = pos_y + c*temp_y - s*temp_z;
    view_z = pos_z + s*temp_y + c*temp_z;

    count_angles();
}

void movable::draw_view_vector()
{
     glBegin(GL_LINES);
        glVertex3f(pos_x,pos_y,pos_z);
        glVertex3f(view_x, view_y, view_z);
     glEnd();
}

void movable::print()
{
    draw_view_vector();
}


