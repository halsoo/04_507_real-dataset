#pragma once

#include "ofMain.h"
#include "myUtil.hpp"
#include "ofxTextSuite.h"
#include "commonAliases.hpp"
#include "consts.h"

using namespace consts;

class Logger {
    
public:
    Logger();
    
    void setup();
    void update();
    void updateGeneralSatus(int traffic, int accuTraffic,
                            int size, int fileNum, int failCnt);
    void updateLastStatus(int traffic);
    void updateLogs(uint64_t fileName, uint64_t frames);
    void draw();
    
private:
    void drawTitles();
    
    void initTitles();
    void initPlainTitles();
    void drawPlainTitles();
    void initBlueTitles();
    void drawBlueTitles();
    
    void drawStatus();
    
    void initGeneralStatus();
    void drawGeneralStatus();
    
    void initLastStatus();
    void drawLastStatus();
    
    void addLog(std::string log);
    void drawLogs();
    
    int xPos;
    int yPos;
    int firstGap;
    int secondGap;
    int yGap;
    
    int statusGap;
    int logGap;
    
    VTx titles;
    VTx generalStatus;
    VTx lastStatus;
    VTx logs;
};
