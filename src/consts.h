#pragma once

#include "ofMain.h"
#include "commonAliases.hpp"

namespace consts {
    const int FrameRate = 10;
    
    //for Block and Building
    const VCol ColorPalette = VCol {ofColor(0, 252, 39), ofColor(241, 0, 179), ofColor(230, 159, 0), ofColor(0, 255, 228)}; //size : 04;
    //green, magenta, yellow, light blue
    
    const int NumberOfSteps = 8;
    const int SizeOfStep = 30;
    const int DuplicateLimit = 3; // size of colorPalette
    
    const int DiffMax = 10;
    const int DiffMin = -10;
    const int TrafficMax = 70;
    
    const float BdLineWid = 3.0;
    
    const V2 FailV2 = V2(-1, -1);
    const ofColor NoCol = ofColor(255, 255, 255, 0);
    
    const std::string ErrorFont = "RobotoMono-BoldItalic.ttf";
    const string FailText = "Error : No more storage space available for files";
    const int ErrorSize = 25;
    
    const uint64_t InitFileName = 1309500;
    const uint64_t InitFileNameAfter = 1427800;
    
    
    //for Logger
    const int LoggerX = 55; // original : 78
    const int LoggerY = 52; // original : 52
    
    const int TitleSize = 15;
    const int StatusSize = 14;
    const int LogSize = 13;
    
    const ofColor LoggerBlue = ofColor(0, 255, 228);
    const ofColor LoggerYellow = ofColor(230, 159, 0);
    
    const std::string TitleFont = "RobotoMono-MediumItalic.ttf";
    const std::string LogFont = "RobotoMono-Regular.ttf";
    
    const VSt PlainTitles = { "carbon footprint/", "web efficiency/", "traffic/", "fail count/" };
    const VSt BlueTitles = { "total/", "last updated/", "view upload logs/" };
    const std::string TimeStampDate = "16-MAR-2078";
};
