#include <iostream>

#include <GL/glut.h>

#include "camera.h"
#include "obj_loader.h"

#include "camera_moveable.h"

using namespace std;

camera_moveable *cam = NULL;


const int window_x = 700;
const int window_y = 600;


#include "camera_moveable.h"


obj_loader loader("3d_obj/jerrypi.obj");

void reshape (int width, int height)
{
    GLfloat w, h;
    glViewport(0, 0, width, height);

    glutPostRedisplay();

}


void Draw_Grid()
{

  float  off[] = { 0.0, 0.0, 0.0, 0.0 };
    float white[] = { 1.0, 1.0, 1.0, 1.0 };
    float red[] = { 1.0, 0.0, 0.0, 1.0 };
    float deep_blue[] = { 0.1, 0.5, 0.8, 1.0 };
    float shiny[] = { 50.0 };
    float dull[] = { 0.0 };


    glColor3f(1.0, 1.0, 1.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, shiny);
   // glMaterialfv(GL_FRONT, GL_SHININESS, dull);

    float i;
	for( i = -500; i <= 500; i += 5)
	{
		glBegin(GL_LINES);
			glColor3ub(150, 190, 150);

			glVertex3f(-500, i, 0);
			glVertex3f(500, i, 0);
			glVertex3f(i, -500,0);
			glVertex3f(i, 500, 0);
		glEnd();
	}
}

void draw_coordinates()
{
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.8,0.1,0);
        glVertex3f(1,0,0);
        glVertex3f(0.8,-0.1,0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.1, 0.8, 0);
        glVertex3f(0,     1, 0);
        glVertex3f(-0.1, 0.8, 0);


        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-0.1, -0.1, 0.8);
        glVertex3f(0,0,1);
        glVertex3f(0.1, 0.1, 0.8);
    glEnd();

    glBegin(GL_LINES);

            glLineWidth(10.0);

            glColor3f(1.0, 0.0, 0.0);   //red
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
//
            glColor3f(0.0, 1.0, 0.0);  // gree
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);

//
            glColor3f(0.0, 0.0, 1.0);  // blue
            glVertex3f(0,0,0);
            glVertex3f(0,0,1);

        glEnd();

}

void draw_shaded()
{
    float  off[] = { 0.0, 0.0, 0.0, 0.0 };
    float white[] = { 1.0, 1.0, 1.0, 1.0 };
    float red[] = { 1.0, 0.0, 0.0, 1.0 };
    float deep_blue[] = { 0.1, 0.5, 0.8, 1.0 };
    float shiny[] = { 50.0 };
    float dull[] = { 0.0 };

  /* Draw a large, red cube made of non-reflective material */
glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
glMaterialfv(GL_FRONT, GL_SPECULAR, shiny);




}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    { // projection view
        cam->print();
//    glTranslatef(cx, cy,cz);
//    glRotatef(x_angle, 1.0, 0.0, 0.0);
//    glRotatef(z_angle, 0.0, 0.0, 1.0);

    }

    {  // model view
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);

        Draw_Grid();


        loader.set_scale(0.1, 0.1, 0.1);

     //   draw_shaded();

       // draw_coordinates();

        //glutWireCube(3.0);
       // draw_jerry();


        loader.print();

        //glEnable(GL_NORMALIZE);
//        glBegin(GL_TRIANGLES);
//            glNormal3f(0, 0, 1); /* Normal for triangle 012.*/
//            glVertex3f(0, 0, 0);
//            glVertex3f(1,0,0);
//            glVertex3f(1,1,0);
//            glNormal3f(-0.5,0.5,0.5); /* Normal for triangle 132. */
//            glVertex3f(0,0,0);
//            glVertex3f(1,1,0);
//            glNormal3f(-0.5,0.5,0);
//            glVertex3f(0,1,-1);
//        glEnd();
        draw_shaded();

        glBegin(GL_QUADS);
            glVertex3f(-10, 0, 0);
            glVertex3f(-10, 0, 10);
            glVertex3f(-10, 80, 10);
            glVertex3f(-10, 80, 0);

            glVertex3f(10, 0, 0);
            glVertex3f(10, 80, 0);
            glVertex3f(10, 80, 10);
            glVertex3f(10, 0, 10);

        glEnd();

      // glutWireCube(1);

   //    glutWireCone(10.5, 50.0, 15, 15) ;

    }

    glutSwapBuffers();
}

void init (void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    cam->go_to_start();


//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0f, 1.0f ,0.1f,100.0f);
//
//    gluLookAt(cx, cy, cz,	wx, wy, wz, 0, 1, 0); // default look position
}

void drag (int x, int y)
{
    cam->mouse_drag(x, y);

    glutPostRedisplay();
}

void mouse_button (int button, int state, int x, int y)
{
     // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if (button == 3) // It's a wheel event
    {
         cam->translate(0, 0, -1);
         glutPostRedisplay();
    }
    if (button == 4)  // It's a wheel event
    {
         cam->translate(0, 0, 1);
         glutPostRedisplay();
    }

    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            cam->mouse_press(x, y);
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {

        }
        else if (button == GLUT_RIGHT_BUTTON)
        {

        }
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            cam->mouse_release();
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {

        }
        else if (button == GLUT_RIGHT_BUTTON)
        {

        }
    }

}


void keyboard (unsigned char key, int x, int y)
{

//    if (key=='1')
//    {
//        cycle++;
//    }
//    if (key=='2')
//    {
//        cycle--;
//    }
//    if (cycle < 1) cycle = 1;


    if (key=='w')
    {
        cam->move_forward( 0.2);
    }
    if (key=='s')
    {
        cam->move_forward( -0.2);
    }
    if (key=='d')
    {
        cam->translate(0.2, 0, 0);
    }
    if (key=='a')
    {
        cam->translate(-0.2, 0, 0);
    }
    if (key=='+')
    {
        cam->translate(0, 0, 1);
    }
    if (key=='-')
    {
        cam->translate(0, 0, -1);
    }
    if (key=='q')
    {
        cam->rotate_z(0.15);
    }
    if (key=='e')
    {
        cam->rotate_z(-0.15);
    }

    /*
     *  object control keys
     */
    if ((key >= '0') && (key <= '9'))
    {


        if (key=='2')
        {
            loader.move_forward(-0.5);
        }
        else if (key=='4')
        {
            loader.translate(-1, 0, 0);
        }
        else if (key=='6')
        {
            loader.translate(1, 0, 0);
        }
        else if (key=='8')
        {
            loader.move_forward(0.5);
        }
        else if (key=='7')
        {
            loader.rotate_z(0.2);
        }
        else if (key=='9')
        {
            loader.rotate_z(-0.2);
        }

        else if (key=='0')
        {

        }
        else if (key=='1')
        {

        }


    }




    if (key==27)
    {
    exit(0);
    }

    glutPostRedisplay();
}

int main (int argc, char *argv[])
{

    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(window_x, window_y);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Rotating a wire cube");

    cam = new camera_moveable();
    cam->position_Camera(0, 0, 10,	0, 3, 0,   0, 0, 1);

    //loader.init(0,0,0, 0,1,0);


    init();
    /* Callback registration here */
    glutDisplayFunc(display);
    glutIdleFunc(display);


    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_button);
    glutMotionFunc(drag);

    glutReshapeFunc(reshape);
    glEnable(GL_DEPTH_TEST);


    glutMainLoop();
    return 0;
}

