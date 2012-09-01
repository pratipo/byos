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

        int                     dist_to_center;
        int                     dist_to_floor;

        bool                    useclips;
        float                   xClip;
        float                   XClip;
        float                   yClip;
        float                   YClip;
        float                   zClip;
        float                   ZClip;

        float                   scaleXYZ;
        float                   posX;
        float                   posY;
        float                   posZ;

        float                   rotX;
        float                   rotY;
        float                   rotZ;

        /// METHODS
        kinectCloud();

        void init(int i, ofxKinect* k, int dcenter, int dfloor);

        void resetTransf();
        void resetClips();

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
