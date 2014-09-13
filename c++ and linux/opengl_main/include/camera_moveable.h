#ifndef CAMERA_MOVEABLE_H
#define CAMERA_MOVEABLE_H

#include "movable.h"

#include <GL/glut.h>

#include <math.h>

class camera_moveable : public movable
{
    public:
        camera_moveable();

        void print();

        void position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z);
        void go_to_start();

        void mouse_press(int x, int y);
        void mouse_release();
        void mouse_drag(int x, int y);

    private:
        float up_x, up_y, up_z;

        int mouse_last_x, mouse_last_y;
        bool mouse_on_hold;

};

#endif // CAMERA_MOVEABLE_H
