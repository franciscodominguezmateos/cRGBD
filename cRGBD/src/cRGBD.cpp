//============================================================================
// Name        : cRGBD.cpp
// Author      : Francico Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <sophus/so3.hpp>
#include <GL/glut.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "depthImage.h"

using namespace cv;
using namespace std;
using Eigen::MatrixXd;

float newAngle=0;
Mat image,depth;
DepthImage di;

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //gluLookAt (0.0, 0.0, 2.5, di.getCentroid().x, -di.getCentroid().y, -di.getCentroid().z, 0.0, 1.0, 0.0);
    gluLookAt (0.0, 0.0, 3.0, 0, 0, 0, 0.0, 1.0, 0.0);
    //glTranslatef(0.0f, 0.0f, -2.0f);
    glRotatef(newAngle, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
      glTranslatef(-di.getCentroid().x, di.getCentroid().y, di.getCentroid().z);
      di.glRender();
    glPopMatrix();
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.1, 0.0, 0.0);
        glVertex3f(0.1, 0.0, 0.1);
        glVertex3f(0.0, 0.0, 0.1);
    glEnd();
    glColor3f(0.5,1.0,0.5);
    glBegin(GL_POLYGON);
        glVertex3f( 0.0,0.0, 0.0);
        glVertex3f( 0.0,0.1, 0.0);
        glVertex3f( 0.0,0.1, 0.1);
        glVertex3f( 0.0,0.0, 0.1);
    glEnd();
    //glFlush();
    glutSwapBuffers();
}
void managerIdle(void)
{
    //usleep(1000);
    glutPostRedisplay();
}
void managerKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
    case 'Q':{
    	newAngle++;
        break;

    }
    case 'a':
    case 'A':{
    	newAngle--;
        break;

    }
        case 27:
        {
            exit(0);
        }
    }
    (void)(x);
    (void)(y);
}
void managerMouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(managerIdle);
        break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(NULL);
        break;
        default:
        break;
    }
    (void)(x);
    (void)(y);
}
void managerResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(18.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void glInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("Hello world :D GIT!!!");
    glutDisplayFunc(displayMe);
    glutReshapeFunc(managerResize);
    glutMouseFunc(managerMouse);
    glutKeyboardFunc(managerKeyboard);
    glutIdleFunc(managerIdle);
}

void show(Mat i,Mat d){
    imshow("Display Image", i);
    imshow("Display Depth", d);
}

vector<string> getLinesFromFile(string fileName){
	vector<string> lines;
	ifstream myfile(fileName.c_str());
	string str;
	while (getline(myfile,str))
		lines.push_back(str);
	return lines;
}
vector<string> split(string line){
	string str;
	istringstream ss(line);
	vector<string> words;
	while(ss>> str)
		words.push_back(str);
	return words;
}
int main(int argc, char** argv ) {
	string basepath,assopath,imagepath,depthpath,str;
	if ( argc != 2 )
		{
	        printf("usage: cRGB pathToRGBDdataset \n");
	        return -1;
	}
	basepath=argv[1];
	DepthImage dImg(basepath,10);
    di=dImg.sparse();
    cout << di.getCentroid()<< " centroid"<<endl;
    cout << di.getPoints3D().size()/1000 << "mil filtered points" <<endl;
	    //namedWindow("Display Image", WINDOW_AUTOSIZE );
	    //show(image,depth);
	    //waitKey(0);
	glInit(argc,argv);
	glutMainLoop();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
