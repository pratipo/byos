#include "kinectCloud.h"

kinectCloud::kinectCloud()
{}

void kinectCloud::init(int i, ofxKinect* k)
{
    id = i;

    kinect = k;

    step = 1;
    w = 640/step;
    h = 480/step;

    /// MODEL matrix RESET ?
//    model.makeIdentityMatrix();
//    model.glScale(400.0, 400.0, 400.0);
//    model.glRotate(120.0*id, 0.0, 1.0, 0.0);
//    model.glTranslate(0.0, 0.0, -800.0/400.0);
//    model.glRotate(90.0, 0.0, 0.0, -1.0);

    xClip = -1000;
    XClip = 1000;
    yClip = -1500;
    YClip = 1500;
    zClip = 1000;
    ZClip = 2500;

    posX = 0.0;
    posY = 0.0;
    posZ = 0.0;

    rotX = 0.0;
    rotY = 0.0;
    rotZ = 0.0;

    mesh.setMode(OF_PRIMITIVE_POINTS);
}

void kinectCloud::meshCloud(){
	mesh.clear();

	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect->getDistanceAt(x, y) > 0) {
                ofVec3f p = kinect->getWorldCoordinateAt(x, y);
				if( (p.x > xClip) && (p.x < XClip) &&
                    (p.y > yClip) && (p.y < YClip) &&
                    (p.z > zClip) && (p.z < ZClip)   )
                    mesh.addVertex(p);
			}
		}
	}
}

void kinectCloud::update(){
    this->resetTransform();

    this->rotate(0.0      + rotX,  1.0,0.0,0.0); // x
    this->rotate(-90.0    + rotY,  0.0,1.0,0.0); // y
    this->rotate(120.0*id  + rotZ,  0.0,0.0,1.0); // z

    this->move ( 2100 * cos(TWO_PI*(float)id/nK), // triangle_radius
                 2100 * sin(TWO_PI*(float)id/nK), // triangle_radius
                 740                            ); // kHeights[i]

    this->truck(posX);
    this->boom(posY);
    this->dolly(posZ);
}

void kinectCloud::customDraw()
{
    ofDrawAxis(500);
    ofCircle(0,0,100);
    mesh.drawVertices();
}

//--------------------------------------------------------------
kinectCloud::~kinectCloud()
{
    // no pointers to delete
}
