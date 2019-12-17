#pragma once

#include "ofMain.h"
#include "Block.hpp"
#include "Logger.hpp"
#include "myUtil.hpp"
#include "commonAliases.hpp"
#include "consts.h"

using namespace consts;

class ofApp : public ofBaseApp{

	public:
    void setup();
    void resetBeforeCam();
    void resetAfterCam();
    
    void update();
    
    void draw();
    
    void basicSettings();
    
    ofCamera beforeCam;
    Block beforeBlock;
    ofFbo beforeVisual;
    ofFbo beforeTerminal;
    Logger beforeLogger;
    bool isBeforeUpdated;
    Bd beforeLastUpdate;
    
    ofCamera afterCam;
    Block afterBlock;
    ofFbo afterVisual;
    ofFbo afterTerminal;
    Logger afterLogger;
    bool isAfterUpdated;
    Bd afterLastUpdate;
    
    void beforeVisualUpdate();
    void afterVisualUpdate();
    
    void beforeVisualDraw();
    void afterVisualDraw();
    
    void beforeTerminalDraw();
    void afterTerminalDraw();
    
    void beforeTerminalUpdate();
    void afterTerminalUpdate();
};
