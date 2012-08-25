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
        void updateCamera();

        void drawOrigin();
		void meshCloud();

		ofCamera camera;
		ofxVec3f cam_location;

		ofxKinect kinect1, kinect2, kienct3;
		ofMesh mesh;


};
