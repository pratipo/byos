#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include "ofxKinect.h"
#include "ofxPCL.h"
#include "ofxVectorMath.h"

#include "kinectCloud.h"

class testApp : public ofBaseApp , public ofxMidiListener {

	public:
        ofxXmlSettings          XML;
        void loadXML(string file);
        void updateXML();
        //------------------------------------
        bool showgui;
        ofxPanel gui;

        ofxParameter<bool> drawdepths;
        ofxParameter<bool> autorotation;
        ofxParameter<bool> clips;

        ofxParameter<bool> resetTransformation;
        ofxParameter<bool> resetClip;

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
        ofCamera camera;
		ofVec3f cam_location;
		int cam_distance;

        void updateCamera();

        void resetTransf(bool& resetTransformation);
        void resetClips(bool& resetClip);

        void updateKinects();
        void updateKinectClouds();

        void update();
        //------------------------------------
        void drawOrigin();
        void drawKinectClouds();

        void draw();
        //------------------------------------
        bool exportAsc;
        void saveClouds();
        void exportToAsc();

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

		//------------------------------------
        void newMidiMessage(ofxMidiMessage& eventArgs);

        ofxMidiIn midiIn;
        ofxMidiMessage midiMessage;

        bool bypassmidi;

        int slider1,slider2,slider3;
};
