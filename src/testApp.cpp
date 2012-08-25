#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetVerticalSync(true);

    autoRotate = false;

    gui.setup();
	gui.add(autorotation.set("auto rotation",false));


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

    kinect1.update();

    if(kinect1.isFrameNew()){
		meshCloud();
	}s
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

	kinect1.drawDepth(2,40,640/4,480/4);


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

    if(key=='s'){
    	mesh.save(ofGetTimestampString()+".ply");
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

