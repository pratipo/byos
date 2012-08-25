#include "testApp.h"

void testApp::setUpKinects(){

    int i = 0;
    kinects[i].init(false, false);
	kinects[i].open(kserials[i]); // open by serial number
}

void testApp::setUpKinectClouds(){

    int i = 0;
    kinectClouds[i].init(0, 1);

}

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetVerticalSync(true);

    gui.setup();
	gui.add(autorotation.set("auto rotation",false));

    kserials[0] = "A00364911617035A"; //, "x", "y"};
    setUpKinects();
    setUpKinectClouds();

    ofBackground(0);
    ofEnableAlphaBlending();
    mesh.setMode(OF_PRIMITIVE_POINTS);
}

// MyCustom3DNode*node=newMyCustom3DNode(); node->setPosition( cos((float)i/10*TWO_PI)*r ,sin((float)i/10*TWO_PI)*r ,-5 );

void testApp::meshCloud(){
	mesh.clear();
	int w = 640;
	int h = 480;

	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinects[0].getDistanceAt(x, y) > 0) {
				mesh.addVertex(kinects[0].getWorldCoordinateAt(x, y));
			}
		}
	}
}

void testApp::updateCamera(){

    float distance = 3000;
    float speed = 20.0;

    if (autorotation){
        cam_location = ofxVec3f(    sin(2.0*PI*ofGetElapsedTimef()/speed) * distance,
                                    cos(2.0*PI*ofGetElapsedTimef()/speed) * distance,
                                    1000); // z elevation
    }
    else{
        cam_location = ofxVec3f(    sin(4.0*PI*mouseX/ofGetWidth())*cos(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * distance,
                                    cos(4.0*PI*mouseX/ofGetWidth())*cos(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * distance,
                                    sin(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * distance); // z elevation
    }

    camera.resetTransform();
    camera.setPosition(cam_location);
    camera.lookAt(ofVec3f(0,0,1000), ofVec3f(0,0,1));
}

//--------------------------------------------------------------
void testApp::update(){

    updateCamera();

    kinects[0].update();

    if(kinects[0].isFrameNew()){
		meshCloud();
	}
}

void testApp::drawOrigin(){

    ofPushStyle();

    ofNoFill();

    ofDrawAxis(200);

    ofSetColor(ofColor(255, 255, 255));
    ofSetCircleResolution(6);
    ofCircle(0,0,250);

    /// TODO draw vertices according to corrected kinect position
    ofSetCircleResolution(3);
    ofCircle(0,0,2100);

    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);

    gui.draw();

    ofDrawBitmapString(ofToString(ofGetFrameRate()),2,10);
    ofDrawBitmapString(ofToString(ofGetElapsedTimef()),2,20);

	kinects[0].drawDepth(2,40,640/4,480/4);


    glEnable(GL_DEPTH_TEST);
	camera.begin();

        drawOrigin();

        ofPushMatrix();

            ofScale(1,1,1);
            // ofRotate()
            ofSetColor(255,255,255,200);

            glEnable(GL_DEPTH_TEST);
            mesh.drawVertices();
            glDisable(GL_DEPTH_TEST);

        ofPopMatrix();
    camera.end();
	glDisable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key){
        case 's':
            mesh.save(ofGetTimestampString()+".ply");
            break;
        case 'r':
            autorotation = !autorotation;
            break;
    }

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

//--------------------------------------------------------------

