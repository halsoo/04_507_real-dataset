#include "ofMain.h"
#include "ofApp.h"
#include "consts.h"

//========================================================================
int main(){
    ofSetupOpenGL(1920, 1080, OF_FULLSCREEN); // setup the GL context
    ofSetVerticalSync(true);
    ofSetFrameRate(FrameRate);
    ofHideCursor();
    
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
