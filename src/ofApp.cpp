#include "ofApp.h"

void ofApp::setup(){
    beforeVisual.allocate(ofGetWidth()/2, ofGetHeight()/2, GL_RGBA, 8);
    beforeTerminal.allocate(ofGetWidth()/2, ofGetHeight()/2, GL_RGBA, 8);
    afterVisual.allocate(ofGetWidth()/2, ofGetHeight()/2, GL_RGBA, 8);
    afterTerminal.allocate(ofGetWidth()/2, ofGetHeight()/2, GL_RGBA, 8);
    
    beforeVisual.begin();
    beforeBlock.setup(0, 0, NumberOfSteps, SizeOfStep);
    resetBeforeCam();
    beforeVisual.end();
    
    beforeTerminal.begin();
    beforeLogger.setup();
    beforeTerminal.end();
    
    afterVisual.begin();
    afterBlock.setup(0, 0, NumberOfSteps, SizeOfStep, true);
    resetAfterCam();
    afterVisual.end();
    
    afterTerminal.begin();
    afterLogger.setup();
    afterTerminal.end();
}

void ofApp::update(){
    beforeVisualUpdate();
    afterVisualUpdate();
    
    beforeTerminalUpdate();
    afterTerminalUpdate();
}

void ofApp::draw(){
    //ofBackground(ofColor::black);
    
    beforeVisualDraw();
    beforeTerminalDraw();
    afterVisualDraw();
    afterTerminalDraw();
    
    beforeVisual.draw(0, 0);
    beforeTerminal.draw(0, ofGetHeight()/2);
    afterVisual.draw(ofGetWidth()/2, 0);
    afterTerminal.draw(ofGetWidth()/2, ofGetHeight()/2);
    
    ofSetLineWidth(3);
    ofNoFill();
    ofSetColor(255, 255, 255);
    //ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
}

//====================================================================================

void ofApp::resetBeforeCam() {
    beforeCam.resetTransform();
    beforeCam.setFov(50);
    
    beforeCam.setPosition(beforeBlock.center.x + beforeBlock.blockWidth,
                          beforeBlock.center.y - beforeBlock.blockWidth,
                          300);
    V3 dir(-beforeBlock.blockWidth/2, -beforeBlock.blockWidth/2, 0);
    
    beforeCam.lookAt(dir);
    beforeCam.rollDeg(60);
    beforeCam.panDeg(-20);
}

void ofApp::resetAfterCam() {
    afterCam.resetTransform();
    afterCam.setFov(50);
    
    afterCam.setPosition(afterBlock.center.x + afterBlock.blockWidth,
                         afterBlock.center.y - afterBlock.blockWidth,
                         300);
    V3 dir(-afterBlock.blockWidth/2, -afterBlock.blockWidth/2, 0);
    
    afterCam.lookAt(dir);
    afterCam.rollDeg(60);
    afterCam.panDeg(-20);
}

void ofApp::beforeVisualUpdate() {
    beforeVisual.begin();
    isBeforeUpdated = beforeBlock.update(ofGetFrameNum());
    beforeVisual.end();
}

void ofApp::afterVisualUpdate() {
    afterVisual.begin();
    isAfterUpdated = afterBlock.update(ofGetFrameNum());
    afterVisual.end();
}

void ofApp::beforeVisualDraw() {
    beforeVisual.begin();
    ofEnableAlphaBlending();
    ofBackground(ofColor::black);
    
    beforeCam.begin();
    ofEnableDepthTest();
    //ofDrawAxis(100);
    beforeBlock.draw();
    
    ofDisableDepthTest();
    beforeCam.end();
    ofDisableAlphaBlending();
    
    beforeBlock.printOutOverSign(beforeVisual.getWidth()/2,
                                 beforeVisual.getHeight()/2,
                                 ofGetFrameNum());
    beforeVisual.end();
}

void ofApp::afterVisualDraw() {
    afterVisual.begin();
    ofEnableAlphaBlending();
    ofBackground(ofColor::black);
    
    afterCam.begin();
    ofEnableDepthTest();
    //ofDrawAxis(100);
    afterBlock.draw();
    
    ofDisableDepthTest();
    afterCam.end();
    ofDisableAlphaBlending();
    
    afterBlock.printOutOverSign(afterVisual.getWidth()/2,
                                afterVisual.getHeight()/2,
                                ofGetFrameNum());
    afterVisual.end();
}

void ofApp::beforeTerminalUpdate() {
    beforeTerminal.begin();
    
    if(isBeforeUpdated) {
        beforeLastUpdate = beforeBlock.getLastUpdate();
        beforeLogger.updateLogs(beforeLastUpdate.fileName, ofGetFrameNum());
    }
    
    beforeLogger.updateGeneralSatus(beforeBlock.getTrafficCnt(),
                                    beforeBlock.getAccuTrafficCnt(),
                                    beforeBlock.getCapacity(),
                                    beforeBlock.getBdsSize(),
                                    beforeBlock.getFailCnt());
    beforeLogger.updateLastStatus(beforeBlock.getLastUpdate().height);
    
    beforeTerminal.end();
}

void ofApp::afterTerminalUpdate() {
    afterTerminal.begin();
    
    if(isAfterUpdated) {
        afterLastUpdate = afterBlock.getLastUpdate();
        afterLogger.updateLogs(afterLastUpdate.fileName, ofGetFrameNum());
    }
    
    afterLogger.updateGeneralSatus(afterBlock.getTrafficCnt(),
                                   afterBlock.getAccuTrafficCnt(),
                                   afterBlock.getCapacity(),
                                   afterBlock.getBdsSize(),
                                   afterBlock.getFailCnt());
    
    afterLogger.updateLastStatus(afterBlock.getLastUpdate().height);
    
    afterTerminal.end();
}

void ofApp::beforeTerminalDraw() {
    beforeTerminal.begin();
    ofBackground(ofColor::black);
    beforeLogger.draw();
    beforeTerminal.end();
}

void ofApp::afterTerminalDraw() {
    afterTerminal.begin();
    ofBackground(ofColor::black);
    afterLogger.draw();
    afterTerminal.end();
}

