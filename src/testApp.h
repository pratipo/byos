#pragma once

/*
#include <time.h>
#include <vector>
#include <string>
*/

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxPCL.h"
#include "ofxVectorMath.h"

#include "kinectCloud.h"

#define nK 3

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        //------------------------------------
        ofxPanel gui;

        ofxParameter<bool> autorotation;


        //------------------------------------
        string kserials[3];
        ofColor kColors[3];
		ofxKinect kinects[3];
		kinectCloud kinectClouds[3];

		void setUpKinects();

        void setUpKinectClouds();

        void updateKinectClouds();

        void drawKinectClouds();

		ofCamera camera;
		ofxVec3f cam_location;

        void updateCamera();

        void drawOrigin();

		ofMesh mesh;
};
