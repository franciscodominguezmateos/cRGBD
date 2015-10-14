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

using namespace cv;
using namespace std;
using Eigen::MatrixXd;

float newAngle=0;

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(newAngle += 1, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
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
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void glInit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D GIT!!!");
    glutDisplayFunc(displayMe);
    glutReshapeFunc(managerResize);
    glutMouseFunc(managerMouse);
    glutKeyboardFunc(managerKeyboard);
    glutIdleFunc(managerIdle);
}
void renderDepth(Mat img,Mat d){
	glPointSize(1.0);
	glBegin(GL_POINTS);
	for (int v=0;v<d.rows;v++)
	{
		for (int u=0;u<d.cols;u++)
		{
			float r=img.at<uchar>(v,u,0)/255.0;
			float g=img.at<uchar>(v,u,1)/255.0;
			float b=img.at<uchar>(v,u,2)/255.0;
			glColor3f(r,g,b);
			float deep=d.at<float>(v,u);
			float x=u;
			float y=v;
			float level=1;
			//Intrinsic data for Stum dataset
			float fx = 525.0/level;  // focal length x
			float fy = 525.0/level;  // focal length y
			float cx = 319.5/level;  // optical center x
			float cy = 239.5/level;  // optical center y
			float factor = 5000.0; // for the 16-bit PNG files
			float Z=deep/factor;
			float X = (x - cx) * Z / fx;
			float Y = (y - cy) * Z / fy;
			glVertex3f(X,Y,Z);
		}
	}
	glEnd();
}
void show(Mat i,Mat d){
    imshow("Display Image", i);
    imshow("Display Depth", d);
}

int main(int argc, char** argv ) {
	glInit(argc,argv);
	  MatrixXd m(2,2);
	  string basepath,assopath,imagepath,depthpath;
	  m(0,0) = 3;
	  m(1,0) = 2.5;
	  m(0,1) = -1;
	  m(1,1) = m(1,0) + m(0,1);
	  cout << m << endl;
	  cout << argv[1] << endl;
	    if ( argc != 2 )
	    {
	        printf("usage: DisplayImage.out <Image_Path> <Depth_Path>\n");
	        return -1;
	    }
	    vector<string> lines;
	    basepath=argv[1];
	    assopath=basepath+"/association.txt";
	    ifstream myfile(assopath.c_str());
	    string str;
	    while (getline(myfile,str)) {
	    	lines.push_back(str);
   	    }
	    istringstream ss(lines[50]);
	    vector<string> words;
	    while(ss>> str){
	    	words.push_back(str);
	    }
	    cout << words[1] << " " << words[3] << endl;
	    imagepath=basepath +"/"+words[1];
	    depthpath=basepath +"/"+words[3];

	    Mat image,depth;
	    image = imread(imagepath, IMREAD_COLOR );
	    depth = imread(depthpath, IMREAD_ANYDEPTH );

	    if ( !image.data )
	    {
	        printf("No image data \n");
	        return -1;
	    }
	    if ( !depth.data )
	    {
	        printf("No depth data \n");
	        return -1;
	    }
	    namedWindow("Display Image", WINDOW_AUTOSIZE );
	    //show(image,depth);
	    //waitKey(0);
	    glutMainLoop();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
