#include "kinectCloud.h"

kinectCloud::kinectCloud()
{}

void kinectCloud::init(int i, ofxKinect* k)
{
    id = i;

    kinect = k;

    cloudRes = 1;
    w = 640/cloudRes;
    h = 480/cloudRes;

    /// MODEL matrix RESET ?
//    model.makeIdentityMatrix();
//    model.glScale(400.0, 400.0, 400.0);
//    model.glRotate(120.0*id, 0.0, 1.0, 0.0);
//    model.glTranslate(0.0, 0.0, -800.0/400.0);
//    model.glRotate(90.0, 0.0, 0.0, -1.0);

    xClip = -0.5;
    XClip = 0.5;
    yClip = -0.5;
    YClip = 0.5;
    zClip = 1.5;
    ZClip = 2.5;
}

void kinectCloud::meshCloud(){
	mesh.clear();

	int w = 640;
	int h = 480;

	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect->getDistanceAt(x, y) > 0) {
				mesh.addVertex(kinect->getWorldCoordinateAt(x, y));
			}
		}
	}
}

void kinectCloud::customDraw()
{
    ofDrawAxis(500);
    mesh.drawVertices();
}

//--------------------------------------------------------------
kinectCloud::~kinectCloud()
{
    // no pointers to delete
}
