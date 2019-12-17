#pragma once

#include "ofMain.h"
#include "commonAliases.hpp"
#include "consts.h"

using namespace consts;

namespace myUtil {
    void withStrokeRect(int x, int y, int size,
                        ofColor stroke, ofColor fill);
    
    
    bool V2Compare(V2 A, V2 B);
    bool V3Compare(V3 A, V3 B);
    
    bool V2Same(V2 A, V2 B);
    bool V3Same(V3 A, V3 B);
    
    bool VV2Comp(const VV2 A, const VV2 B);
    bool VV2Same(const VV2 A, const VV2 B);
    
    bool PBBAll(PBB input);
    
    PBI VV2BinarySearch(VV2 list, const V2 key);
    bool VV3Search(VV3 list, V3 key);
    
    int VV3ElemCnt(VV3 list, const V3 key);
    
    bool returnPressedOnce(bool *now, bool *prev);
    
    int randomInt(int min, int max);
    
    //functions for pass searchSpace()'s result to individual building
    VV2 convertGap(VV2 area, int step);
    
    VV3 makeOutPoints(VV2 area, int step);
    
    VV3 eraseGarbage(VV3 origin);
    bool getDirection(VV3IT now, VV3IT next);
    VV3 VV3MiddlePoints(VV3 list);
    
    PL makeShape(VV3 source);
    PL updateToWorld(PL source, V2 start, int size, int gap);
    
    VVMSH shapeExtrudeWireFrame(PL shape, float height,
                                bool yIsHeight = false,
                                bool drawFloor = false);
    
    MSH shapeExtrudeFace(PL shape, float height, bool yIsHeight = false);
    
    PL makeCircle(V3 center, float radius);
    
    VMSH circleExtrudeWireFrame(PL shape, float height,
                                bool yIsHeight = false,
                                bool drawFloor = false);
    
    MSH circleExtrudeFace(PL shape, float height, bool yIsHeight = false);
};
