#ifndef KCLOUD_H
#define KCLOUD_H

#include <vector>
#include <string>

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxKinect.h"

class kinectCloud : public ofNode{

    public:

        int                     id;
        ///size
        int                     w;
        int                     h;
        int                     cloudRes;

        ofxKinect*              kinect;
        ofMesh                  mesh;
        ofxMatrix4x4            model;

        float                   xClip;
        float                   XClip;
        float                   yClip;
        float                   YClip;
        float                   zClip;
        float                   ZClip;

        /// METHODS
        kinectCloud();

        void init(int i, ofxKinect* k);

        void meshCloud();

        virtual void customDraw();

        /// EXPORT METHODS


        ///
        virtual ~kinectCloud();
};

#endif // KCLOUD_H
