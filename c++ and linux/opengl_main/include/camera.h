#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

#include <math.h>

typedef struct tVector3					// expanded 3D vector struct
{
	tVector3() {}	// constructor
	tVector3 (float new_x, float new_y, float new_z) // initialize constructor
	{x = new_x; y = new_y; z = new_z;}
	// overload + operator so that we easier can add vectors
	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	// overload - operator that we easier can subtract vectors
	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	// overload * operator that we easier can multiply by scalars
	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
	// overload / operator that we easier can divide by a scalar
	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}

	float x, y, z;						// 3D vector coordinates
}tVector3;

typedef struct Point
{
    float x, y;

    Point() {x = 0; y = 0;}
};

class camera
{
    public:
        camera();
        void go_to_start();
        void current();

        void move_vertical(float height);
        void Move_Camera(float speed);
		void Rotate_View(float speed);

        void move_x(float);

		void Position_Camera(float pos_x, float pos_y,float pos_z,
			 				 float view_x, float view_y, float view_z,
							 float up_x,   float up_y,   float up_z);

        void mouse_press(int x, int y);
        void mouse_release();

        void mouse_drag(int x, int y);

    private:
		tVector3 mPos;
		tVector3 mView;
		tVector3 mUp;

		int mouse_last_x, mouse_last_y;
        bool mouse_on_hold;

};

#endif // CAMERA_H
