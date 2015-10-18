/*
 * depthImage.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: francisco
 */

#include "depthImage.h"

DepthImage::DepthImage() {
	level=1;
	//Intrinsic data for Stum dataset
	fx = 525.0/level;  // focal length x
	fy = 525.0/level;  // focal length y
	cx = 319.5/level;  // optical center x
	cy = 239.5/level;  // optical center y
	factor = 5000.0; // for the 16-bit PNG files
}

DepthImage::~DepthImage() {
	// TODO Auto-generated destructor stub
}

Point3f DepthImage::getPoint3D(int u,int v){
	float x=u;
	float y=v;
	float deep=dImg.at<float>(v,u);
	float Z=deep/factor;
	float X = (x - cx) * Z / fx;
	float Y = (y - cy) * Z / fy;
	return Point3f(X,Y,Z);
}
vector<Point3f> DepthImage::getPoints3D(){
	vector<Point3f> vp;
	for (int v=0;v<dImg.rows;v++){
		for (int u=0;u<dImg.cols;u++){
			if (isGoodDepthPixel(u,v)){
				Point3f p=getPoint3D(u,v);
				vp.push_back(p);
			}
		}
	}
	return vp;
}
void DepthImage::setDepth(const Mat& img) {
	dImg = img;
	vector<Point3f> pts;
	Point3f pt;
	for(Point3f p:pts){
		pt+=p;
	}
	centroid=pt;
	centroid.x/=size();
	centroid.y/=size();
	centroid.z/=size();
}
void DepthImage::glRender(){
	glPointSize(1.0);
	glBegin(GL_POINTS);
	for (int v=0;v<dImg.rows;v++)
	{
		for (int u=0;u<dImg.cols;u++)
		{
			if(isGoodDepthPixel(u,v)){
			    Vec3b col=getColor(u,v);
				float b=col.val[0]/255.0;
				float g=col.val[1]/255.0;
				float r=col.val[2]/255.0;
				glColor3f(r,g,b);
				Point3f p=getPoint3D(u,v);
				glVertex3f(p.x,-p.y,-p.z);
			}
		}
	}
	glEnd();
}
