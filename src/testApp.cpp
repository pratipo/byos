#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetVerticalSync(true);

    kinect1.init(false, false);
	kinect1.open("A00364911617035A"); // open by serial number

    ofBackground(0);
    ofEnableAlphaBlending();
    mesh.setMode(OF_PRIMITIVE_POINTS);
}

void testApp::meshCloud(){
	mesh.clear();
	int w = 640;
	int h = 480;

	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect1.getDistanceAt(x, y) > 0) {
				mesh.addVertex(kinect1.getWorldCoordinateAt(x, y));
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
    kinect1.update();

    if(kinect1.isFrameNew()){
		meshCloud();
	}

    camera.resetTransform();
    camera.truck(mouseX-ofGetWidth()*.5);
    camera.boom(mouseY-ofGetHeight()*.5);

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),2,10);

	kinect1.drawDepth(2,20,640/4,480/4);

    glEnable(GL_DEPTH_TEST);
	camera.begin();
        ofScale(1,-1,-1);
        ofSetColor(255,255,255,200);

        glEnable(GL_DEPTH_TEST);
        mesh.drawVertices();
        glDisable(GL_DEPTH_TEST);

        camera.end();
	glDisable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
