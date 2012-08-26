#ifndef KCLOUD_H
#define KCLOUD_H

#include <vector>
#include <string>

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxKinect.h"

#define nK 3

class kinectCloud : public ofNode{

    public:

        int                     id;
        int                     selected;
        ///size
        int                     w;
        int                     h;
        int                     step; // brute force downsampling factor

        ofxKinect*              kinect;
        ofMesh                  mesh;
        ofxMatrix4x4            model;

        bool                    clips;
        float                   xClip;
        float                   XClip;
        float                   yClip;
        float                   YClip;
        float                   zClip;
        float                   ZClip;

        float                   posX;
        float                   posY;
        float                   posZ;

        float                   rotX;
        float                   rotY;
        float                   rotZ;

        /// METHODS
        kinectCloud();

        void init(int i, ofxKinect* k);

        void meshCloud();
        void update();

        void drawClips();
        virtual void customDraw();

        /// EXPORT METHODS
        void ascVertices(ofstream& f);

        ///
        virtual ~kinectCloud();
};

#endif // KCLOUD_H
