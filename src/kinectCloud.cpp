#include "kinectCloud.h"

kinectCloud::kinectCloud()
{}

void kinectCloud::init(int i, int res=1)
{
    id = i;
    cloudRes = res;
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

void kinectCloud::customDraw()
{
    ofSphere(0,0,0,700); ofDrawAxis(500);
}

//--------------------------------------------------------------
kinectCloud::~kinectCloud()
{
    // no pointers to delete
}
