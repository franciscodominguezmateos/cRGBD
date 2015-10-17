/*
 * depthImage.h
 *
 *  Created on: Oct 17, 2015
 *      Author: Francisco Dominguez
 */

#ifndef DEPTHIMAGE_H_
#define DEPTHIMAGE_H_
#include <vector>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class DepthImage {
	Mat dImg;
	Mat cImg;
	float fx,fy;
	float cx,cy;
	float level; //pyramid level
	float factor;//Sturm data set is 5000
	Point3f centroid;
public:
	DepthImage();
	virtual ~DepthImage();
	Point3f getPoint3D(int u,int v);
	vector<Point3f> getPoints3D();
	inline const Mat& getImg() const {	return cImg;}
	inline void setImg(const Mat& img) {cImg = img;}
	inline float getCx() const {return cx;	}
	inline void setCx(float cx) {this->cx = cx;}
	inline float getCy() const {return cy;	}
	inline void setCy(float cy) {this->cy = cy;}
	inline const Mat& getDepth() const {return dImg;	}
	inline void setDepth(const Mat& img) {
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
	inline float getFactor() const {return factor;	}
	inline void setFactor(float factor) {this->factor = factor;}
	inline float getFx() const {return fx;	}
	inline void setFx(float fx) {this->fx = fx;}
	inline float getFy() const {return fy;	}
	inline void setFy(float fy) {this->fy = fy;}
	inline float getLevel() const {return level;}
	inline void setLevel(float level) {this->level = level;}
	inline int cols(){return cImg.cols;}
	inline int rows(){return cImg.rows;}
	inline int size(){return cImg.cols*cImg.rows;}
	inline Point3f getCentroid(){return centroid;}
};

#endif /* DEPTHIMAGE_H_ */
