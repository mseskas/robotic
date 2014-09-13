#ifndef MOVABLE_H
#define MOVABLE_H

#include <iostream>

#include <vector>
#include <GL/glut.h>
#include <math.h>

#include "glm/glm.hpp"

using namespace glm;

class movable
{
    public:
        movable();

        void print();

        void draw_view_vector();

        void move_forward(float distance);

        void rotate_z(float angle);
        void rotate_x(float angle);

        void translate(float x, float y, float z);

        void init(float pos_x, float pos_y,float pos_z,
             float view_x, float view_y, float view_z);
        void set_view(float x, float y, float z);


    protected:

        void count_angles();

        float sc_x, sc_y, sc_z;

        float pos_x, pos_y, pos_z;
        float view_x, view_y, view_z;

        float angle_x, angle_y, angle_z;

};

#endif // MOVABLE_H
