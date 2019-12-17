#pragma once

#include "ofMain.h"
#include "myUtil.hpp"
#include "commonAliases.hpp"
#include "consts.h"

using namespace consts;

class Building {
    
public:
    Building();
    
    void setup(VV2 _shape, int initHeight,
               V2 pos, int step, int size, uint64_t name, bool center = false);
    void update(int height);
    void draw();
    
    bool isCenter;
    
    VV2 area;
    PL originalShape;
    
    V2 blockPos;
    int height;
    int stepNum;
    int stepSize;
    
    ofColor lineCol;
    ofColor col;
    
    uint64_t fileName;
    
    PL worldShape;
    VVMSH shapesWireFrame;
    MSH shapeColor;
    
    V3 shapeCenter2D;
    PL circle;
    VMSH cylinderWireFrame;
    MSH cylinder;
    
    
private:
    void shapesSetup();
    
    VVMSH makeWireFrame(PL shape, float height);
    MSH makeFaces(PL shape, float height);
    VMSH circleWireFrame(PL shape, float height);
    MSH circleFaces(PL shape, float height);
};
