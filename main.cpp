#include <cmath>
#include <iostream>

/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
#include <glm/glm.hpp>


using namespace std;

int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;
int screen_width = 0, screen_height = 0;

bool rotate = false;

float angle = 0;
glm::vec3 axis_in_camera_coord;

float rot = 0;

double xRot = 0;
double yRot = 0;
double zRot = 0;


void drawRect(double x, double y, double z) {
    x /= 2;
    y /= 2;
    z /= 2;

    // Yellow side - FRONT
    glBegin(GL_POLYGON);

    glColor3f(1.0, 1.0, 0.0);

    glVertex3f(  x, -y, -z );
    glVertex3f(  x,  y, -z );
    glVertex3f( -x,  y, -z );
    glVertex3f( -x, -y, -z );
    glEnd();

    // White side - BACK
    glBegin(GL_POLYGON);

    glColor3f(1.0, 1.0, 1.0);

    glVertex3f(  x, -y, z );
    glVertex3f(  x,  y, z );
    glVertex3f( -x,  y, z );
    glVertex3f( -x, -y, z );
    glEnd();
    
    // Red side - RIGHT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);

    glVertex3f( x, -y, -z );
    glVertex3f( x,  y, -z );
    glVertex3f( x,  y,  z );
    glVertex3f( x, -y,  z );
    glEnd();
    
    // Orange side - LEFT
    glBegin(GL_POLYGON);

    glColor3f(1.0, 0.5, 0.0);

    glVertex3f( -x, -y,  z );
    glVertex3f( -x,  y,  z );
    glVertex3f( -x,  y, -z );
    glVertex3f( -x, -y, -z );
    glEnd();
    
    // Green side - TOP
    glBegin(GL_POLYGON);

    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(  x,  y,  z );
    glVertex3f(  x,  y, -z );
    glVertex3f( -x,  y, -z );
    glVertex3f( -x,  y,  z );
    glEnd();
    
    // Blue side - BOTTOM
    glBegin(GL_POLYGON);

    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(  x, -y, -z );
    glVertex3f(  x, -y,  z );
    glVertex3f( -x, -y,  z );
    glVertex3f( -x, -y, -z );
    glEnd();
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //glScalef(0.5, 0.5, 0.5);

    // glPushMatrix();
    // glTranslatef(-0.75, 0, 0);
    // glRotatef(rot, 1, 1, 0);
    // drawRect(1, 1, 1);
    // glPopMatrix();
    
    glPushMatrix();
    //glTranslatef(0.75, 0, 0);

    // if (rotate == true)
    //     glRotatef(angle * 180 / 3.14, axis_in_camera_coord[0], axis_in_camera_coord[1], axis_in_camera_coord[2]);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glRotatef(zRot, 0, 0, 1);
    cout << xRot << " " << yRot << " " << zRot << endl;


    drawRect(1, 1, 1);
    glPopMatrix();

    glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        arcball_on = true;
        last_mx = cur_mx = x;
        last_my = cur_my = y;
    } else {
        arcball_on = false;
    }
}

void myMotion(int x, int y) {
    if (arcball_on) {  // if left button is pressed
        cur_mx = x;
        cur_my = y;
    }
}

glm::vec3 get_arcball_vector(int x, int y) {
    glm::vec3 P = glm::vec3(1.0*x/screen_width*2 - 1.0,
                1.0*y/screen_height*2 - 1.0,
                0);
    P.y = -P.y;

    float OP_squared = P.x * P.x + P.y * P.y;

    if (OP_squared <= 1*1)
        P.z = sqrt(1*1 - OP_squared);  // Pythagoras
    else
        P = glm::normalize(P);  // nearest point
    
    //cout << P[0] << " " << P[1] << " " << P[2] << endl;
    return P;
}

void myReshape(int width, int height) {
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);    
}

void myIdle() {
    rot += 0.01;
    rotate = false;
    if (cur_mx != last_mx || cur_my != last_my) {
        glm::vec3 va = get_arcball_vector(last_mx, last_my);
        glm::vec3 vb = get_arcball_vector(cur_mx,  cur_my);

        // cout << "VA:" << va[0] << " " << va[1] << " " << va[2] << endl;
        // cout << "VB:" << vb[0] << " " << vb[1] << " " << vb[2] << endl;
        // cout << endl;


        angle = acos(min(1.0f, glm::dot(va, vb)));
        axis_in_camera_coord = glm::cross(va, vb);

        xRot += angle * axis_in_camera_coord[0] * 18000 / 3.14;
        yRot += angle * axis_in_camera_coord[1] * 18000 / 3.14;
        zRot += angle * axis_in_camera_coord[2] * 18000 / 3.14;
        //glRotatef(angle, axis_in_camera_coord[0], axis_in_camera_coord[1], axis_in_camera_coord[2]);
        //glm::mat3 camera2object = glm::inverse(glm::mat3(transforms[MODE_CAMERA]) * glm::mat3(mesh.object2world));
        //glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        //mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);

        last_mx = cur_mx;
        last_my = cur_my;

        rotate = true;
    } 


    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Arcball!");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutReshapeWindow(1000,600);

    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutReshapeFunc(myReshape);
    glutMotionFunc(myMotion);
    glutIdleFunc(myIdle);

    glutMainLoop();
    return 0;
}