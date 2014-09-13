#include "camera.h"


camera::camera()
{
    mouse_last_x = 0;
    mouse_last_y = 0;
    mouse_on_hold = false;
    go_to_start();
}

void camera::go_to_start()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, 1.0f ,0.1f,1000.0f);
}

void camera::current()
{
    go_to_start();

    gluLookAt(mPos.x, mPos.y, mPos.z,
                mView.x, mView.y, mView.z,
                mUp.x,   mUp.y,   mUp.z);

}

void camera::Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	mPos	= tVector3(pos_x,  pos_y,  pos_z ); // set position
	mView	= tVector3(view_x, view_y, view_z); // set view
	mUp		= tVector3(up_x,   up_y,   up_z  ); // set the up vector
}

void camera::move_vertical(float height)
{
    mPos.z +=  height;
    mView.z +=  height;
}

void camera::Move_Camera(float speed)
{
	tVector3 vVector = mView - mPos;	// Get the view vector

	// forward positive cameraspeed and backward negative -cameraspeed.
	mPos.x  = mPos.x  + vVector.x * speed; mView.x = mView.x + vVector.x * speed;
	mPos.y  = mPos.y  + vVector.y * speed; mView.y = mView.y + vVector.y * speed;
}

void camera::move_x(float distance)
{
    tVector3 vVector = mView - mPos;

    vVector = vVector * distance;

    mPos.x  = mPos.x  - vVector.y ;
	mPos.y  = mPos.y  + vVector.x ;

	mView.x = mView.x - vVector.y ;
	mView.y = mView.y + vVector.x ;


}

void camera::Rotate_View(float speed )
{
	tVector3 vVector = mView - mPos;	// Get the view vector

        mView.y = (float)(mPos.y + sin(speed)*vVector.x + cos(speed)*vVector.y);
        mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.y);
}


void camera::mouse_press(int x, int y)
{
    mouse_on_hold = true;

    mouse_last_x = x;
    mouse_last_y = y;
}

void camera::mouse_release()
{


    mouse_on_hold = false;
}

void camera::mouse_drag(int x, int y)
{
    if (!mouse_on_hold) return;

    int diff_x = mouse_last_x - x;
    int diff_y = mouse_last_y - y;

    float angle_x = (float)diff_x / 300;
	float angle_z = (float)diff_y / 300;

   // mView.x += angle_x * 2;
    mView.z = mView.z + angle_z * 5 ;

	if((mView.z - mPos.z) > 8)  mView.z = mPos.z + 8;
	if((mView.z - mPos.z) <-8)  mView.z = mPos.z - 8; // difference must be small, z current z view

    Rotate_View(-angle_x);

    mouse_last_x = x;
    mouse_last_y = y;
}
