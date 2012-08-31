#include "testApp.h"

void testApp::setUpKinects(){
    for(int i = 0; i<nK; i++){
        kinects[i].init(false, false);
        kinects[i].open(kserials[i]); // open by serial number
    }
}

void testApp::setUpKinectClouds(){
    for(int i = 0; i<nK; i++){
        kinectClouds[i].init(i, &kinects[i], triangle_radius);
    }
}

void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofBackground(0);

    midiIn.listPorts(); // via instance

	// open port by number (you may need to change this)
	midiIn.openPort(1);
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this); // add testApp as a listener
	midiIn.setVerbose(false);

	//-------------------------------------------------
    kserials[0] = "A00364911617035A"; //, "x", "y"};
    kserials[1] = "A00365805405044A";
    kserials[2] = "A00362803303039A";

    kColors[0] = ofColor(255,0,0); kColors[1] = ofColor(0,200,55); kColors[2] = ofColor(255,255,0);

    kHeights[0] = kHeights[1] = kHeights[2] = 740;
    triangle_radius = 750; // RECOMMENDED 2100

    cam_distance = 3000;
    camera.setFarClip(10000);

    loadXML("settings.xml");
    setUpKinects();
    setUpKinectClouds();
    curK = 0;
    //--------------------------------------------------
    gui.setup();

    gui.add(drawdepths.set("draw kinects depth",true));
	gui.add(autorotation.set("auto rotation",false));
	gui.add(clips.set("clips",true));
	gui.add(resetTransformation.set("reset transf current cloud",false));
	gui.add(resetClip.set("reset clipping box current cloud",false));

    clips.addListener(this,&testApp::clipsChanged);

    resetTransformation.addListener(this,&testApp::resetTransf);
    resetClip.addListener(this,&testApp::resetClips);

    exportAsc = false;
}

//--------------------------------------------------------------

void testApp::updateCamera(){
    float speed = 20.0;
    if (autorotation){
        cam_location = ofxVec3f(    sin(TWO_PI*ofGetElapsedTimef()/speed) * cam_distance,
                                    cos(TWO_PI*ofGetElapsedTimef()/speed) * cam_distance,
                                    1000); // z elevation
    }
    else{
        cam_location = ofxVec3f(    sin(4.0*PI*mouseX/ofGetWidth())*cos(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * cam_distance,
                                    cos(4.0*PI*mouseX/ofGetWidth())*cos(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * cam_distance,
                                    sin(1.0*PI*mouseY/ofGetHeight() - PI/2.0) * cam_distance); // z elevation
    }
    camera.resetTransform();
    camera.setPosition(cam_location);
    camera.lookAt(ofVec3f(0,0,1000), ofVec3f(0,0,1));
}

void testApp::resetTransf(bool& resetTransformation){
    kinectClouds[curK].resetTransf();
    resetTransformation = false;
}

void testApp::resetClips(bool& resetClip){
    kinectClouds[curK].resetClips();
    resetClip = false;
}

void testApp::updateKinects(){
    for (int i=0; i<nK; i++){
        kinects[i].update();

        if(kinects[i].isFrameNew()){
            kinectClouds[i].meshCloud();
        }
    }
}

void testApp::updateKinectClouds(){
    for(int i = 0; i<nK; i++){
        kinectClouds[i].update();
    }
}

void testApp::update(){
    updateCamera();
    updateKinects();
	updateKinectClouds();

	if (exportAsc){
        exportToAsc();
        exportAsc = false;
	}
}

//--------------------------------------------------------------
void testApp::drawKinectClouds(){
    for(int i = 0; i<nK; i++){
        ofSetColor(kColors[i]);
        kinectClouds[i].draw();
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
        ofCircle(0,0,triangle_radius);
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    if(drawdepths){
        ofPushMatrix();
            ofTranslate(0,640/4+20);
            ofRotateZ(-90);
            kinects[0].drawDepth(2,40,640/4,480/4);
            kinects[1].drawDepth(2,170,640/4,480/4);
            kinects[2].drawDepth(2,300,640/4,480/4);
        ofPopMatrix();
    }
    glEnable(GL_DEPTH_TEST);
	camera.begin();
        drawOrigin();
        drawKinectClouds();
    camera.end();
	glDisable(GL_DEPTH_TEST);

    gui.draw();
    ofDrawBitmapString(ofToString(ofGetFrameRate()),2,10);
}

//--------------------------------------------------------------
void testApp::saveClouds(){
    for(int i = 0; i<nK; i++){
        kinectClouds[i].mesh.save(ofGetTimestampString()+"i"+".ply");
    }
}

void testApp::exportToAsc(){
    ostringstream filename;
    ofstream export_file;

    string timestamp = ofGetTimestampString();

    cout << "enter name : " << endl;
    string nom;
    cin >> nom;

    std::ostringstream pathname;
    pathname << "/home/dbsus4/Desktop/scans/" << timestamp << "_" << nom << "/";
    mkdir(pathname.str().c_str(), 0777);

    filename << pathname.str() << timestamp << "_" << nom << ".asc" ;

    /// TODO: VIBRATORS
//    serial.writeByte('H');
//    ofSleepMillis(500);

    cout << "saving asc model to... "+filename.str() << endl;
    export_file.open(filename.str().c_str());

    for (int k=0; k<nK; k++)
        kinectClouds[k].ascVertices(export_file);

    export_file.close();
    cout << "finnished exporting to "+filename.str() << endl;
}


void testApp::loadXML(string file){
    if( XML.loadFile(file) )
		cout << "settings.xml loaded!" << endl;
    else
        cout << "settings.xml NOT loaded!" << endl;

    cam_distance = XML.getValue("DISTANCE", 3000);
    for (int k=0; k<nK; k++){
        std::ostringstream s;

        s << "MODEL" << k << ":XCLIP";
        kinectClouds[k].xClip = XML.getValue(s.str(), 0.0);
        s.str("");
        s << "MODEL" << k << ":XXCLIP";
        kinectClouds[k].XClip = XML.getValue(s.str(), 0.0);
        s.str("");
        s << "MODEL" << k << ":YCLIP";
        kinectClouds[k].yClip = XML.getValue(s.str(), 0.0);
        s.str("");
        s << "MODEL" << k << ":YYCLIP";
        kinectClouds[k].YClip = XML.getValue(s.str(), 0.0);
        s.str("");
        s << "MODEL" << k << ":ZCLIP";
        kinectClouds[k].zClip = XML.getValue(s.str(), 0.0);
        s.str("");
        s << "MODEL" << k << ":ZZCLIP";
        kinectClouds[k].ZClip = XML.getValue(s.str(), 0.0);
        s.str("");
    }
}

void testApp::updateXML(){
    XML.setValue("DISTANCE", cam_distance);

    for (int k=0; k<nK; k++){
        std::ostringstream s;
        s << "MODEL" << k << ":XCLIP";
        XML.setValue( s.str(), kinectClouds[k].xClip );
        s.str("");
        s << "MODEL" << k << ":XXCLIP";
        XML.setValue( s.str(), kinectClouds[k].XClip );
        s.str("");
        s << "MODEL" << k << ":YCLIP";
        XML.setValue( s.str(), kinectClouds[k].yClip );
        s.str("");
        s << "MODEL" << k << ":YYCLIP";
        XML.setValue( s.str(), kinectClouds[k].YClip );
        s.str("");
        s << "MODEL" << k << ":ZCLIP";
        XML.setValue( s.str(), kinectClouds[k].zClip );
        s.str("");
        s << "MODEL" << k << ":ZZCLIP";
        XML.setValue( s.str(), kinectClouds[k].ZClip );
        s.str("");
    }
    XML.saveFile("settings.xml");
    cout << "settings.xml saved !" << endl;
}

//--------------------------------------------------------------

void testApp::newMidiMessage(ofxMidiMessage& msg) {
    //cout   << msg.toString() << endl;
    cout << " midi >> control:" << msg. control << " value:" << msg.value << " delta:" << msg.deltatime << endl;
    int value = (msg.value);
    int change = 0;

    switch (msg.control){
        case 49:
            if (value) autorotation = !autorotation;
            break;
        case 44:
            if(value) exportAsc = true;
            break;

        // translation
        case 2:
            kinectClouds[curK].posX -= value;
            break;
        case 3:
            kinectClouds[curK].posY -= 10;
            break;
        case 4:
            kinectClouds[curK].posZ += 10;
            break;

        // rotation

        // scale?

        // camera distance to origin

        // FOV

    }

/*
    switch (msg.byteOne) {

	    //AUTOROTATE
	    case 49:
            if(args.byteTwo==0) autoRotate = !autoRotate;
            break;
        //CAPTURE
        case 44:
            if(args.byteTwo==0) doExport = true;
            break;
        //LAYER 1 KNOB 1
        case 14:
            increment =l1k1-p;
            if(abs(increment)>2) {
                factor = 4;
            }else{
                factor = 0.1;
            }
            if (increment>0) factor*=-1;

                    //TEST

//                    clouds[kindex]->model.glRotate(-90.0, 0.0, 0.0, 1.0);
//                    clouds[kindex]->model.glTranslate(0.0, 0.0, 2); //* -800/400
//                    clouds[kindex]->model.glRotate(-120.0*kindex, 0.0, 1.0, 0.0);


                        clouds[kindex]->model.rotate(factor, 1.0, 0.0, 0.0);

//                    clouds[kindex]->model.glRotate(120.0*kindex, 0.0, 1.0, 0.0);
//                    clouds[kindex]->model.glTranslate(0.0, 0.0, -2); //* -800/400
//                    clouds[kindex]->model.glRotate(90.0, 0.0, 0.0, 1.0);

            l1k1=p;
            break;
        //LAYER 1 KNOB 2
        case 15:
            increment =l1k2-p;
            if(abs(increment)>2) {
                factor = 4;
            }else{
                factor = 0.1;
            }
            if (increment>0) factor*=-1;
            clouds[kindex]->model.rotate(factor, 0.0, 1.0, 0.0);
            l1k2=p;
            break;
        //LAYER 1 KNOB 3
        case 16:
            increment =l1k3-p;
            if(abs(increment)>2) {
                factor = 4;
            }else{
                factor = 0.1;
            }
            if (increment>0) factor*=-1;
            clouds[kindex]->model.rotate(factor, 0.0, 0.0, 1.0);
            l1k3=p;
            break;

        //LAYER 1 SLIDER 1  X TRANSLATION
        case 2:
            increment =l1s1-p;
            if(abs(increment)>2) {
                factor = 2;
            }else{
                factor = .2;
            }
            if (increment>0) factor*=-1;
            clouds[kindex]->model.translate(factor, 0.0, 0.0);
            l1s1=p;
            break;

        //LAYER 1 SLIDER 2 Y TRANSLATION
        case 3:
            increment =l1s2-p;
            if(abs(increment)>2) {
                factor = 2;
            }else{
                factor = .2;
            }
            if (increment>0) factor*=-1;
            clouds[kindex]->model.translate(0.0, factor, 0.0);
            l1s2=p;
            break;
        //LAYER 1 SLIDER 3 Z TRANSLATION
        case 4:
            increment =l1s3-p;
            if(abs(increment)>2) {
                factor = 2;
            }else{
                factor = .2;
            }
            if (increment>0) factor*=-1;
            clouds[kindex]->model.translate(0.0, 0.0, factor);
            l1s3=p;
            break;

        ///-----------------------------------------------------
        //LAYER 1 SLIDER 4 SCALE
//        case 5:
//            fincrement =l1s4-p;
//            if(p > 127/2+127/6 || p < 127/2-127/6){
//                clouds[kindex]->model.scale(1.02, 1.02, factor);
//
//            break;
        ///-----------------------------------------------------

        //LAYER 1 SLIDER 8 DISTANCE
        case 12:
            increment =l1s8-p;
            if(abs(increment)>4) {
                factor = 50;
            }else{
                factor = 5;
            }
            if (increment>0) factor*=-1;
            distance += factor;
            l1s8=p;
            break;

        //LAYER 1 SLIDER 9 FOV
        case 13:
            fov = (float)args.byteTwo*0.5+30.0 ;
            needsUpdateProjection=true;
            break;
	}
	*/
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

    kinectClouds[0].selected = kinectClouds[1].selected = kinectClouds[2].selected = false;

    switch (key){
        case 'z':
            exportAsc = true;
            break;
        case 'x':
            autorotation = !autorotation;
            break;
        case 'c':
            clips = !clips;
            break;

        case '+':
            cam_distance -= 100;
            break;
        case '-':
            cam_distance += 100;
            break;

        case '1':
            curK = 0;
            break;
        case '2':
            curK = 1;
            break;
        case '3':
            curK = 2;
            break;

        case 's':
            kinectClouds[curK].posX -= 10;
            break;
        case 'w':
            kinectClouds[curK].posX += 10;
            break;
        case 'd':
            kinectClouds[curK].posY -= 10;
            break;
        case 'a':
            kinectClouds[curK].posY += 10;
            break;
        case 'q':
            kinectClouds[curK].posZ -= 10;
            break;
        case 'e':
            kinectClouds[curK].posZ += 10;
            break;



        case 'D':
            kinectClouds[curK].rotX -= 1;
            break;
        case 'A':
            kinectClouds[curK].rotX += 1;
            break;
        case 'S':
            kinectClouds[curK].rotY -= 1;
            break;
        case 'W':
            kinectClouds[curK].rotY += 1;
            break;
        case 'E':
            kinectClouds[curK].rotZ -= 1;
            break;
        case 'Q':
            kinectClouds[curK].rotZ += 1;
            break;


        case 'g':
            kinectClouds[curK].xClip -= 10;
            break;
        case 't':
            kinectClouds[curK].XClip -= 10;
            break;
        case 'f':
            kinectClouds[curK].yClip -= 10;
            break;
        case 'h':
            kinectClouds[curK].YClip -= 10;
            break;
        case 'r':
            kinectClouds[curK].zClip -= 10;
            break;
        case 'y':
            kinectClouds[curK].ZClip -= 10;
            break;

        case 'G':
            kinectClouds[curK].xClip += 10;
            break;
        case 'T':
            kinectClouds[curK].XClip += 10;
            break;
        case 'F':
            kinectClouds[curK].yClip += 10;
            break;
        case 'H':
            kinectClouds[curK].YClip += 10;
            break;
        case 'R':
            kinectClouds[curK].zClip += 10;
            break;
        case 'Y':
            kinectClouds[curK].ZClip += 10;
            break;

    }

    kinectClouds[curK].selected = true;

}

void testApp::exit(){

    updateXML();
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

void testApp::clipsChanged(bool & clips)
{
    for(int i = 0; i<nK; i++){
        kinectClouds[i].clips = clips;
    }
}
