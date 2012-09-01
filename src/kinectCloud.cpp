#include "kinectCloud.h"

kinectCloud::kinectCloud(){ }

void kinectCloud::init(int i, ofxKinect* k, int dcenter, int dfloor)
{
    id = i;
    kinect = k; // pointer to assigned kinect device

    step = 1;
    w = 640/step;
    h = 480/step;

    dist_to_center = dcenter;
    dist_to_floor = dfloor;
    useclips = true;

    scaleXYZ = 1.0;
    posX = posY = posZ = 0.0;
    rotX  = rotY = rotZ = 0.0;

    mesh.setMode(OF_PRIMITIVE_POINTS);
}

void kinectCloud::resetClips()
{
    xClip = 500;
    XClip = 2000;
    yClip = -1000;
    YClip = 1000;
    zClip = -1000;
    ZClip = 1000;
}

void kinectCloud::resetTransf()
{
    posX = posY = posZ = 0.0;
    rotX  = rotY = rotZ = 0.0;
}

void kinectCloud::meshCloud()
{
	mesh.clear();

	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect->getDistanceAt(x, y) > 0) {
                ofVec3f o = kinect->getWorldCoordinateAt(x, y);
                ofVec3f p = ofVec3f(o.z,-o.y,o.x);
				if(useclips){
                    if( (p.x > xClip) && (p.x < XClip) &&
                        (p.y > yClip) && (p.y < YClip) &&
                        (p.z > zClip) && (p.z < ZClip)   )
                        mesh.addVertex(p);
                }
                else{
                        mesh.addVertex(p);
                }
			}
		}
	}
}

void kinectCloud::update()
{
    this->resetTransform();

    this->setScale(scaleXYZ);

    this->rotate(0.0                + rotX,  1.0,0.0,0.0); // x
    this->rotate(0.0                + rotY,  0.0,1.0,0.0); // y
    this->rotate(180.0 + 120.0*id   + rotZ,  0.0,0.0,1.0); // z

    this->move (dist_to_center * cos(TWO_PI*(float)id/nK),
                 dist_to_center * sin(TWO_PI*(float)id/nK),
                 dist_to_floor                           ); // kHeights[i]

    this->dolly(posZ);
    this->truck(posX);
    this->boom(posY);
}

void kinectCloud::drawClips()
{
    glLineWidth(1.0);
    if (selected)
        glLineWidth(3.0);
    glBegin(GL_LINE_LOOP); //
        glVertex3f(xClip, yClip, zClip);
        glVertex3f(xClip, YClip, zClip);
        glVertex3f(xClip, YClip, ZClip);
        glVertex3f(xClip, yClip, ZClip);
    glEnd();
    glBegin(GL_LINE_LOOP); //
        glVertex3f(XClip, yClip, zClip);
        glVertex3f(XClip, YClip, zClip);
        glVertex3f(XClip, YClip, ZClip);
        glVertex3f(XClip, yClip, ZClip);
    glEnd();

//    glLineWidth(6.0);
//    std::cout << " cloud " << id << std::endl;
//    std::cout << "\t"<< this->getXAxis() << " " << this->getYAxis() << " "<< this->getZAxis() << " " << std::endl;
//    ofLine(ofVec3f(0,0,0),(ofVec3f)this->getYAxis());
}

void kinectCloud::customDraw()
{
    ofDrawAxis(200);
    ofCircle(0,0,50);

    if (useclips)
        drawClips();

    if (selected)
        glPointSize(2.0);
    mesh.drawVertices();
    glPointSize(1.0);
}

void kinectCloud::ascVertices(ofstream& f)
{
    ofxMatrix4x4 m;
    m = this->getLocalTransformMatrix();
    m = ofxMatrix4x4::getTransposedOf(m);

    int nv = mesh.getNumVertices();
    for(int i=0; i<nv; i++){
        ofVec3f p = m*(ofVec3f(mesh.getVertex(i)));  // POST MULTIPLY POINTS TO ->SCALE >ROTATE(z->up)
       //ofVec3f p = ofVec3f(mesh.getVertex(i));
        f << p.x << ", " << p.y << ", " << p.z << endl;
    }
}

//--------------------------------------------------------------
kinectCloud::~kinectCloud()
{
    // no pointers to delete
}
