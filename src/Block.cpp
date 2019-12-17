#include "Block.hpp"

Block::Block() {}

void Block::setup(int startX, int startY, int _stepNum, int _stepSize, bool after){
    stepNum = _stepNum;
    stepSize = _stepSize;
    
    blockWidth = (stepNum * stepSize);
    blockCapacity = stepNum * stepNum;
    
    x = startX - blockWidth/2;
    y = startY - blockWidth/2;
    pos = V2(x, y);
    center = V2(x + blockWidth/2, y + blockWidth/2);
    
    isAfter = after;
    isOver = false;
    
    minimumSize = 1;
    maximalSize = stepNum;
    
    startFrame = ofGetFrameNum();
    
    duplimit = DuplicateLimit;
    
    trafficCnt = 0;
    accuTrafficCnt = 0;
    failCnt = 0;
    
    isVacant.assign(stepNum*stepNum, true);
    
    if(isAfter) {
        isAfterSetup();
        BdNameInit = InitFileNameAfter;
    } else {
        BdNameInit = InitFileName;
    }
    
    fileCnt = 0;
    
    initOverSign();
}

void Block::restart() {
    isOver = false;
    
    startFrame = ofGetFrameNum();
    
    trafficCnt = 0;
    accuTrafficCnt = 0;
    failCnt++;
    
    Bds.clear();
    isVacant.assign(stepNum*stepNum, true);
    
    if(isAfter)
        isAfterSetup();
}

bool Block::update(uint64_t frame) {
    bool isMaked = false;
    
    if(!isOver) {
        uint64_t diff = frame - startFrame;
        
        if(diff % (FrameRate/2)) {
            updateHeights();
            updateTrafficCnt();
            if(!Bds.empty())
                lastUpdated = Bds.back();
        }
        
        if(diff % (FrameRate * 1) == 0) {
            if(isAfter)
                eraseDuplicates();
        }
        
        if(diff % myUtil::randomInt(FrameRate/2, FrameRate * 3) == 0) {
            makeBuildings();
            isMaked = true;
        }
    }
    
    return isMaked;
}

void Block::draw() {
    if(!isOver) {
        //drawGrid(x, y, stepNum, stepSize);
        drawBound(x, y, stepNum, stepSize);
        for(Bd now : Bds)
            now.draw();
        
        if(isAfter)
            centerBd.draw();
    }
}

void Block::printOutOverSign(int xPos, int yPos, uint64_t nowFrame) {
    
    if(isOver && (nowFrame - isOverStart) < (FrameRate * 10) ) {
        float drawX = xPos - OverSign.getWidth()/2;
        float drawY = yPos + OverSign.getHeight()/4;
        ofSetColor(ofColor::white);
        OverSign.draw(drawX, drawY, 0);
        
    } else if (isOver && (nowFrame - isOverStart) >= (ofGetFrameRate() * 10)) {
        restart();
    }
}

Bd Block::getLastUpdate() { return lastUpdated; }
int Block::getTrafficCnt() { return trafficCnt; }
int Block::getAccuTrafficCnt() { return accuTrafficCnt; }
int Block::getCapacity() { return blockCapacity; }

int Block::getBdsSize() {
    int result = 0;
    
    for(auto it : Bds) {
        result = result + it.area.size();
    }
    
    return result;
}

int Block::getFailCnt() { return failCnt; }

void Block::initOverSign() {
    OverSign.init(ErrorFont, ErrorSize);
    OverSign.setText(FailText);
    
    OverSign.wrapTextX(ofGetWidth() / 3);
}

void Block::isAfterSetup() {
    Bd Center;
    int offset = (stepNum-2)/2;
    
    VV2 tempV2 { V2(offset, offset), V2(offset + 1, offset),
        V2(offset, offset + 1), V2(offset + 1, offset + 1) };
    
    isVacantUpdate(tempV2, stepNum);
    
    Center.setup(tempV2, 30, V2(x, y), stepNum, stepSize, 0, true);
    centerBd = Center;
}

void Block::drawGrid(int xPos, int yPos, int step, int size) {
    ofSetLineWidth(1);
    
    for(int x = 0; x < step; x++)
        for(int y = 0; y<step; y++){
            myUtil::withStrokeRect(xPos + (x * size), yPos + (y * size),
                                   size, ofColor::red, NoCol);
        }
}

void Block::drawBound(int xPos, int yPos, int step, int size) {
    ofSetLineWidth(1);
    myUtil::withStrokeRect(xPos, yPos, step * size, ofColor::gray, NoCol);
}

void Block::makeBuildings() {
    VVV2 tempVacant;
    VV2 tempArea;
    Bd newBd;
    
    tempVacant = spacesCheck(stepNum, isVacant);
    tempArea = searchSpace(randomSize(), stepNum, tempVacant, isVacant);
    
    if(failDetector(tempArea)) {
        isOver = true;
        isOverStart = ofGetFrameNum();
    
    } else {
        isVacantUpdate(tempArea, stepNum);
        fileCnt++;
        newBd.setup(tempArea, 3, pos, stepNum, stepSize,
                    BdNameInit + fileCnt, false);
        lastUpdated = newBd;
        Bds.push_back(newBd);
    }
}

void Block::updateHeights() {
    for(auto it = Bds.begin(); it < Bds.end(); it++) {
        int traffic = diffTraffic();
        //std::cout << traffic << std::endl;
        it->update(traffic);
    }
}

void Block::eraseDuplicates() {
    VBd subs;
    
    for(auto col : ColorPalette) {
        VBd dupl = countBdDuplicates(Bds, col);
        int cnt = dupl.size();
        
        if(cnt > duplimit) {
            PBdI temp = eraseBuilding(dupl);
            centerBd.update(temp.second);
            subs.push_back(temp.first);
        } else {
            subs.insert(subs.end(), dupl.begin(), dupl.end());
        }
    }
    
    Bds = subs;
    updateTrafficCnt();
}

void Block::updateTrafficCnt() {
    int temp = 0;
    for(auto Bd : Bds)
        temp += Bd.height;
    accuTrafficCnt += trafficCnt;
    trafficCnt = temp;
}

PBdI Block::eraseBuilding(VBd origin) {
    std::pair<Bd, int> result;
    
    VBdIT max = std::max_element(origin.begin(), origin.end(),
                                  [](Bd A, Bd B) -> bool {
                                      return A.area.size() < B.area.size();
                                  });
    
    VBdIT min = std::min_element(origin.begin(), origin.end(),
                                 [](Bd A, Bd B) -> bool {
                                     return A.area.size() < B.area.size();
                                 });
    
    result = std::make_pair(*min, (*max).area.size());
    origin.erase(min);
    
    for(auto target : origin)
        isVacantRemove(target.area, stepNum);
    
    return result;
}

VBd Block::countBdDuplicates(VBd origin, ofColor key) {
    VBd result;
    
    for(Bd it : origin)
        if(it.lineCol == key) result.push_back(it);
    
    return result;
}

void Block::isVacantUpdate(VV2 area, int step) {
    for(auto tempVec : area)
        isVacant[(tempVec.y*step)+tempVec.x] = false;
}

void Block::isVacantRemove(VV2 area, int step) {
    for(auto tempVec : area)
        isVacant[(tempVec.y*step)+tempVec.x] = true;
}

VV2 Block::searchSpace(int areaSize, int step, VVV2 spaces, VB isVacant) {
    
    VVV2 tempArea;
    
    for(int i = 0; i< spaces.size(); i++) {
        if(spaces[i].size() == areaSize) return spaces[i];
        else if(spaces[i].size() > areaSize) tempArea.push_back(spaces[i]);
    }
    
    if(tempArea.empty())
        return VV2 { FailV2 };
    
    int tempIndex = myUtil::randomInt(0, tempArea.size()-1);
    
    VVV2 continuityCheckedArea;
    continuityCheckedArea = continuityCheck(tempArea[tempIndex], areaSize, isVacant);
    
    if(continuityCheckedArea.empty())
        return VV2 { FailV2 };
    
    int tempJndex = myUtil::randomInt(0, continuityCheckedArea.size()-1);
    
    return continuityCheckedArea[tempJndex];
}

VVV2 Block::spacesCheck(int step, VB vacant) {
    
    VVV2 vacantSpaces;
    VB isChecked;
    isChecked.assign(step*step, false);
    
    for(int x = 0; x< step; x++)
        for(int y = 0; y<step; y++)
            if(vacant[y*step + x]) {
                VV2 tempSpace;
                
                floodCheck(x, y, step, &tempSpace, &isChecked, vacant);
                
                if(!tempSpace.empty()) {
                    std::sort(tempSpace.begin(), tempSpace.end(), myUtil::V2Compare);
                    vacantSpaces.push_back(tempSpace);
                }
            }
    
    return vacantSpaces;
}

void Block::floodCheck(int x,int y, int step, VV2 *temp, VB *isChecked, VB vacant) {
    
    bool curr = vacant[(y*step)+x];
    bool checked = (*isChecked)[(y*step)+x];
    
    if(curr && !checked) {
        
        V2 tempVec(x, y);
        temp->push_back(tempVec);
        
        (*isChecked)[(y*step)+x] = true;
        
        if(y>0) floodCheck(x,y-1, step, temp, isChecked, vacant);
        if(y<step-1) floodCheck(x,y+1, step, temp, isChecked, vacant);
        if(x>0) floodCheck(x-1,y, step, temp, isChecked, vacant);
        if(x<step-1) floodCheck(x+1,y, step, temp, isChecked, vacant);
    }
}

VVV2 Block::continuityCheck(VV2 area, int areaSize, VB isVacnat) {
    
    VVV2 resultArea;
    
    for(int i = 0; i< area.size(); i++) {
        V2 nowVec = area[i];
        VV2 route;
        route.push_back(nowVec);
        
        floodArea(nowVec, area, route, &resultArea, areaSize, isVacant);
    }
    
    VVV2 sortedResult;
    
    for(VV2 it : resultArea) {
        std::sort(it.begin(), it.end(), myUtil::V2Compare);
        sortedResult.push_back(it);
    }
    
    return sortedResult;
}

void Block::floodArea(V2 now, const VV2 area, VV2 route,
                      VVV2 *result, int areaSize, VB isVacant) {
    
    if(route.size() < areaSize) {
        NearPoint nearPoints = findNear(now, area, isVacant);
        if(!nearPoints.noPoints() &&
            nearPoints.AvailPoints() >= areaSize - route.size()) {
            
            VV2 temp = randomPick(areaSize-route.size(),
                                  nearPoints.pointsAsVV2(area));
            route.insert(route.end(), temp.begin(), temp.end());
            result->push_back(route);
        } else if (!nearPoints.noPoints()) {
            
            for(PBI it : nearPoints) {
                if(it.first) {
                    VV2 nextRoute = route;
                    V2 nextVec = area[it.second];
                    nextRoute.push_back(nextVec);
                    floodArea(nextVec, area, nextRoute, result, areaSize, isVacant);
                }
            }
        }
    } else if(route.size() == areaSize)
        result->push_back(route);
}

bool Block::failDetector(VV2 sample) {
    if(sample.size() == 1 && sample[0] == FailV2)
        return true;
    else
        return false;
}

int Block::randomSize() {
    return myUtil::randomInt(minimumSize, maximalSize);
}

VV2 Block::randomPick(int num, VV2 vector) {
    VV2 result;
    std::vector<int> randomIndex;
    
    for(int i = 0; i < num; i++) {
        int randomIndex = myUtil::randomInt(0, vector.size()-1);
        result.push_back(vector[randomIndex]);
        vector.erase(vector.begin() + randomIndex);
    }
    
    return result;
}

int Block::diffTraffic() {
    return myUtil::randomInt(DiffMin, DiffMax);
}

int Block::randomTraffic() {
    return myUtil::randomInt(0, TrafficMax);
}

Block::NearPoint Block::findNear(const V2 pos, const VV2 area, VB isVacant) {
    NearPoint result;
    
    int step = std::round(std::sqrt(isVacant.size()));
    
    //UP
    V2 UP(pos.x, pos.y-1);
    if(pos.y>0 && isVacant[(UP.y*step) + UP.x])
        result.push_back(myUtil::VV2BinarySearch(area, UP));
    else
        result.push_back(std::make_pair(false, -1));
    
    //DOWN
    V2 DOWN(pos.x, pos.y+1);
    if(pos.y<step-1 && isVacant[(DOWN.y*step) + DOWN.x])
        result.push_back(myUtil::VV2BinarySearch(area, DOWN));
    else
        result.push_back(std::make_pair(false, -1));
    
    //LEFT
    V2 LEFT(pos.x-1, pos.y);
    if(pos.x>0 && isVacant[(LEFT.y*step) + LEFT.x])
        result.push_back(myUtil::VV2BinarySearch(area, LEFT));
    else
        result.push_back(std::make_pair(false, -1));
    
    //RIGHT
    V2 RIGHT(pos.x+1, pos.y);
    if(pos.x<step-1 && isVacant[(RIGHT.y*step) + RIGHT.x])
        result.push_back(myUtil::VV2BinarySearch(area, RIGHT));
    else
        result.push_back(std::make_pair(false, -1));
    
    return result;
}

int Block::NearPoint::AvailPoints() {
    if(!points.empty()) {
        int cnt = 0;
        
        for(auto it : points)
            if(it.first) cnt ++;
        
        return cnt;
    } else
        return 0;
}

//true if there is no points
bool Block::NearPoint::noPoints() {
    if(!points.empty())
        return (points.size() - AvailPoints()) == 0;
    else
        return true;
}

void Block::NearPoint::push_back(PBI input) {
    points.push_back(input);
}

VPBI::iterator Block::NearPoint::begin() {
    return points.begin();
}

VPBI::iterator Block::NearPoint::end() {
    return points.end();
}

VV2 Block::NearPoint::pointsAsVV2(const VV2 area) {
    VV2 temp;
    temp.reserve(1);
    
    if(!points.empty()) {
        temp.reserve(4);
        
        for(auto it : points)
            if(it.first)
                temp.push_back(area[it.second]);
        
    } else {
        temp.push_back(V2(-1, -1));
    }
    
    return temp;
        
}

