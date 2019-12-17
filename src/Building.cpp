#include "Building.hpp"

Building::Building() {}

void Building::setup(VV2 _shape, int initHeight,
                     V2 pos, int step, int size, uint64_t name, bool center) {
    isCenter = center;
    
    col = ofColor::black;
    
    if(isCenter)
        lineCol = ofColor::white;
    else
        lineCol
        = ColorPalette[myUtil::randomInt(0, ColorPalette.size() - 1)];
    
    blockPos = pos;
    height = initHeight;
    stepNum = step;
    stepSize = size;
    
    area = _shape;
    
    fileName = name;
    
    shapesSetup();
}

void Building::update(int _height) {
    if(isCenter) {
        if((height + _height) < 100) height += _height;
        
    } else {
        if((height + _height) < 10) height = 10;
        else if((height + _height) > 100) height = 100;
        else height += _height;
    }
    
    shapesWireFrame = makeWireFrame(worldShape, height);
    shapeColor = makeFaces(worldShape, height);
    
    if(isCenter) {
        V3 shapeCenter3D = shapeCenter2D + V3(0, 0, height);
        circle = myUtil::makeCircle(shapeCenter3D, stepSize * 0.75);
        cylinder = circleFaces(circle, 40);
        cylinderWireFrame = circleWireFrame(circle, 40);
    }
}

void Building::draw() {
    ofSetColor(col);
    shapeColor.draw();
    
    ofSetColor(lineCol);
    ofSetLineWidth(BdLineWid);
    for(auto shape : shapesWireFrame)
        for(auto it : shape) {
            it.setMode(OF_PRIMITIVE_LINE_LOOP);
            it.draw();
        }
    
    if(isCenter) {
        ofSetColor(col);
        cylinder.draw();
        
        ofSetColor(lineCol);
        ofSetLineWidth(BdLineWid);
        for(auto shape : cylinderWireFrame) {
            shape.setMode(OF_PRIMITIVE_LINE_LOOP);
            shape.draw();
        }
    }
}

void Building::shapesSetup() {
    
    VV3 outPoints = myUtil::makeOutPoints(area, stepNum);
    outPoints = myUtil::eraseGarbage(outPoints);
    originalShape = myUtil::makeShape(outPoints);
    worldShape = myUtil::updateToWorld(originalShape, blockPos, stepSize, 0);
    
    shapesWireFrame = makeWireFrame(worldShape, height);
    shapeColor = makeFaces(worldShape, height);
    
    if(isCenter) {
        shapeCenter2D
        = V3(worldShape.getCentroid2D().x, worldShape.getCentroid2D().y, 0);
        V3 shapeCenter3D = shapeCenter2D + V3(0, 0, height);
        circle = myUtil::makeCircle(shapeCenter3D, stepSize * 0.75);
        cylinder = circleFaces(circle, 40);
        cylinderWireFrame = circleWireFrame(circle, 40);
    }
}

VVMSH Building::makeWireFrame(PL shape, float height) {
    return myUtil::shapeExtrudeWireFrame(shape, height, false, false);
}

MSH Building::makeFaces(PL shape, float height) {
    return myUtil::shapeExtrudeFace(shape, height, false);
}

VMSH Building::circleWireFrame(PL shape, float height) {
    return myUtil::circleExtrudeWireFrame(shape, height, false, false);
}

MSH Building::circleFaces(PL shape, float height) {
    return myUtil::circleExtrudeFace(shape, height, false);
}
