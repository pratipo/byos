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

class testApp : public ofBaseApp{

	public:
        ofxXmlSettings          XML;
        void loadXML(string file);
        void updateXML();
        //------------------------------------
        ofxPanel gui;

        ofxParameter<bool> autorotation;
        ofxParameter<bool> clips;

        void clipsChanged(bool & clips);
        //------------------------------------
        int curK;

        string kserials[3];
        ofColor kColors[3];
        int kHeights[3];
		ofxKinect kinects[3];
		kinectCloud kinectClouds[3];

		int triangle_radius;

		void setUpKinects();

        void setUpKinectClouds();

        void setup();
        //------------------------------------
        void updateKinects();
        void updateKinectClouds();

        void drawKinectClouds();

		ofCamera camera;
		ofVec3f cam_location;
		int cam_distance;

        void updateCamera();

        void update();
        //------------------------------------
        void drawOrigin();

        void draw();
        //------------------------------------

        void saveClouds();

        void exit();

        //------------------------------------
        void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
