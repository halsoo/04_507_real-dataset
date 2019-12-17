#pragma once

#include "ofMain.h"
#include "Building.hpp"
#include "myUtil.hpp"
#include "ofxTextSuite.h"
#include "commonAliases.hpp"
#include "consts.h"

using namespace consts;

using Bd = Building;
using VBd = std::vector<Building>;
using VBdIT = std::vector<Building>::iterator;
using VVBdIT = std::vector<std::vector<Building>::iterator>;
using PBdI = std::pair<Building, int>;

//no Gap version

class Block {
    
public:
    Block();
    
    void setup(int startX, int startY,
               int _stepNum, int _stepSize,
               bool after = false);
    void restart();
    bool update(uint64_t time);
    void draw();
    void printOutOverSign(int xPos, int yPos, uint64_t nowFrame);
    
    Bd getLastUpdate();
    int getTrafficCnt();
    int getAccuTrafficCnt();
    int getCapacity();
    int getBdsSize();
    int getFailCnt();
    
    bool isAfter;
    bool isOver;
    
    int x;
    int y;
    V2 pos;
    V2 center;
    int stepNum;
    int stepSize;
    int blockWidth;
    int blockCapacity;
    
    uint64_t startFrame;
    uint64_t isOverStart;
    
    int duplimit;
    
    VB isVacant;
    
    VVV2 spaces;
    
    VBd Bds;
    Bd centerBd;
    VPL shapes;
    
    uint64_t BdNameInit;
    int fileCnt;
    
    int trafficCnt;
    int accuTrafficCnt;
    Bd lastUpdated;
    int failCnt;

private:
    void initOverSign();
    void isAfterSetup();
    
    void drawGrid(int xPos, int yPos, int step, int size);
    void drawBound(int xPos, int yPos, int step, int size);
    
    void updateProcedure();
    void makeBuildings(); //update building height, duplLimit, etc
    void updateHeights();
    void eraseDuplicates();
    void updateTrafficCnt();
    
    PBdI eraseBuilding(VBd origin);
    VBd countBdDuplicates(VBd origin, ofColor key);
    
    void isVacantUpdate(VV2 area, int step);
    void isVacantRemove(VV2 area, int step);
    
    //function for find exact size shape
    //called when return key pressed
    VV2 searchSpace(int areaSize, int step, VVV2 spaces, VB isVacant);
    
    //function for find vacant area
    //called when return key pressed
    VVV2 spacesCheck(int step, VB vacant);
    void floodCheck(int x, int y, int step, VV2 *temp, VB *checked, VB vacant);
    
    //function to assist searchSpace()
    VVV2 continuityCheck(VV2 area, int areaSize, VB isVacnat);
    void floodArea(V2 now, const VV2 area, VV2 route,
                   VVV2 *result, int areaSize, VB isVacant);
    
    bool failDetector(VV2 sample);
    int randomSize();
    VV2 randomPick(int num, const VV2 vector);
    int diffTraffic();
    int randomTraffic();
    
    Text OverSign;
    TextAlign alignment;
    int minimumSize, maximalSize;
    
    typedef struct NearPoint {
        VPBI points;
        
        int AvailPoints();
        
        //true if there is no points
        bool noPoints();
        
        void push_back(PBI input);
        
        VPBI::iterator begin();
        VPBI::iterator end();
        
        VV2 pointsAsVV2(const VV2 area);
        
    } NearPoint;
    
    NearPoint findNear(const V2 pos, const VV2 area, VB isVacant);
};
