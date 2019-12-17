#include "myUtil.hpp"

void myUtil::withStrokeRect(int x, int y, int size,
                            ofColor stroke, ofColor fill) {
    ofFill();
    ofSetColor(fill); //fill color
    ofDrawRectangle(x, y, size, size);
    ofNoFill();
    ofSetColor(stroke);//stroke color
    ofDrawRectangle(x, y, size, size);
}


bool myUtil::V2Compare(V2 A, V2 B) {
    
    auto comp = glm::lessThan(A, B);
    auto equal = glm::equal(A, B);
    
    if(equal[1])
        return comp[0];
    else
        return comp[1];
}

bool myUtil::V2Same(V2 A, V2 B) {
    return glm::all(glm::equal(A, B));
}

bool myUtil::V3Same(V3 A, V3 B) {
    return glm::all(glm::equal(A, B));
}

bool myUtil::VV2Comp(const VV2 A, const VV2 B) {
    if(VV2Same(A, B)) return false;

    else if(A.size() == B.size()) {

        for(int i = 0; i < A.size(); i++) {
            if(A[i] == B[i]) continue;
            else if(V2Compare(A[i], B[i])) return true;
            else return false;
        }

    } else if(A.size() < B.size()) return true;

    else return false;
}

bool myUtil::VV2Same(const VV2 A,const VV2 B) {
    int sameCnt = 0;
    bool result = false;
    
    for(auto it : A) {
        auto isSame = VV2BinarySearch(B, it);
        if(isSame.first) sameCnt ++;
    }
    
    if(sameCnt == B.size()) result = true;
    
    return result;
}

bool myUtil::PBBAll(PBB input) {
    return input.first && input.second;
}

PBI myUtil::VV2BinarySearch(const VV2 list, const V2 key) {
    // low == list's begining, high == list's end
    int low = 0;
    int high = list.size() - 1;
    
    while (low <= high) {
        // calculate middle point
        // to prevent overflow do not use "(low+high)/2"
        int mid = low + (high - low)/2;
        
        //if key is smaller than list[mid], throw away indexes bigger than mid
        if(V2Same(key, list[mid]))
            return std::make_pair(true, mid);
        else
            if(V2Compare(key, list[mid]))
                high = mid - 1;
        //if key is bigger than list[mid], throw away indexes smaller than mid
            else if(!V2Compare(key, list[mid]))
                low = mid + 1;
    }
    
    return std::make_pair(false, -1);  // key not found
}

bool myUtil::VV3Search(VV3 list, V3 key) {
    for(auto it : list) {
        if(it == key) return true;
    }
    
    return false;
}

bool myUtil::returnPressedOnce(bool *now, bool *prev) {
    
    bool returnVal;
    
    if(ofGetKeyPressed(OF_KEY_RETURN)) *now = true;
    else *now = false;
    
    if(*now ==true && *prev == false) returnVal = true;
    else returnVal = false;
    
    *prev = *now;
    
    return returnVal;
    
}

int myUtil::randomInt(int min, int max) {
    if(min == max) return min;
    else {
        int result = std::round(ofRandom(min, max));
        return result;
    }
}

VV2 myUtil::convertGap(VV2 area, int step) {
    for(int i = 0; i < area.size(); i++) {
        area[i] = V2(area[i].x*2, area[i].y*2);
    }
    
    return area;
}

VV3 myUtil::makeOutPoints(VV2 area, int step) {
    VV3 result;
    VVV3 rows;
    const V3 noPoint(-1, -1, -1);
    VV3 noPoints = VV3 { noPoint };
    PL returnShape;
    returnShape.clear();
    
    for(int i = 0; i < step; i++) {
        VV3 tempRow;
        
        for(auto temp : area) {
            if(temp.y == i) {
                V3 convertedVec(temp.x, temp.y, 0);
                tempRow.push_back(convertedVec);
            }
        }
        
        if(!tempRow.empty()) rows.push_back(tempRow);
        else rows.push_back(noPoints);
    }

    for(int i = 0; i < step; i++) {
        if(!V3Same(rows[i][0], noPoint)) {
            int rowSize = rows[i].size();
            V3 tempVec = rows[i][rowSize-1];
            
            V3 tempVertexUpRight(tempVec.x + 1, tempVec.y, tempVec.z);
            result.push_back(tempVertexUpRight);
            
            V3 tempVertexDownRight(tempVec.x + 1, tempVec.y + 1, tempVec.z);
            result.push_back(tempVertexDownRight);
        }
    }
    
    for(int i = 0; i<step; i++) {
        int reverseIdx = step -(i+1);
        if(!V3Same(rows[reverseIdx][0], noPoint)) {
            V3 tempVec = rows[reverseIdx][0];
            
            V3 tempVertexDownLeft(tempVec.x, tempVec.y + 1, tempVec.z);
            result.push_back(tempVertexDownLeft);
            
            V3 tempVertexUpLeft(tempVec.x, tempVec.y, tempVec.z);
            result.push_back(tempVertexUpLeft);
        }
    }
    
    return result;
}

VV3 myUtil::eraseGarbage(VV3 origin) {
    VV3 tempCont;
    
    for(auto it = origin.begin(); it < origin.end(); it++) {
        if(it == origin.begin()) tempCont.push_back(*it);
        else if(*it != tempCont[tempCont.size() - 1]) tempCont.push_back(*it);
    }
    
    origin = tempCont;
    
    VV3 middles = VV3MiddlePoints(origin);
    VV3 temp;

    for(auto it : origin) {
        if(!VV3Search(middles, it))
            temp.push_back(it);
    }

    origin = temp;
    
    return origin;
}

bool myUtil::getDirection(VV3IT now, VV3IT next) {
    if(now->x == next->x && now->y != next->y) return true;
    else if(now->y == next->y && now->x != next->x) return true;
    else if(now->x != next->x && now->y != next->y) return false;
}

VV3 myUtil::VV3MiddlePoints(VV3 list) {
    
    VV3 middles;
    VV3 temp;
    temp.clear();
    auto now = list.begin();
    int mover = 1;
    
    while (now + mover < list.end()) {
        bool dir;
        bool nextDir;
        
        if((now + mover + mover) != list.end()) {
            dir = getDirection(now, now + mover);
            nextDir = getDirection(now, now + mover + mover);
            
        } else {
            dir = getDirection(now, now + mover);
            nextDir = getDirection(now, list.begin());
        }
        
        if(dir && nextDir)
            temp.push_back(*(now+mover));
        else {
            if(!temp.empty()) middles.insert(middles.end(), temp.begin(), temp.end());
            temp.clear();
        }
        
        now = now + mover;
    }
    
    return middles;
}

PL myUtil::makeShape(VV3 source) {
    PL returnShape;
    returnShape.clear();
    
    for(V3 it : source)
        returnShape.addVertex(it);
    
    returnShape.addVertex(source[0]);
    
    return returnShape;
}

PL myUtil::updateToWorld(PL source, V2 start, int size, int gap) {
    for(auto vertx = source.begin(); vertx < source.end(); vertx++) {
        vertx->x = start.x + (vertx->x * size) + (((vertx->x)-1) * gap);
        vertx->y = start.y + (vertx->y * size) + (((vertx->y)-1) * gap);
    }
    
    return source;
}

VVMSH myUtil::shapeExtrudeWireFrame(PL shape, float height,
                                    bool yIsHeight, bool drawFloor) {
    
    VVMSH finalMesh;
    
    V3 middlepoint = shape.getCentroid2D();
    
    //create top vertices
    VMSH tempTop;
    MSH topMesh;
    topMesh.clear();
    for(int i = 0; i < shape.size(); i++) {
        
        if (yIsHeight)
            topMesh.addVertex(shape[i] + V3(0, height, 0));
        else
            topMesh.addVertex(shape[i] + V3(0, 0, height));
    }
    
    tempTop.push_back(topMesh);
    
    //create side as triangles
    VMSH sideMeshes;
    for (int i = 0; i<shape.size(); i++) {
        
        MSH sideMesh;
        sideMesh.clear();
        
        if (i<shape.size() - 1) { //
            // bottom1 - bottom2 - top1
            
            if (yIsHeight) {
                sideMesh.addVertex(shape[i]);
                sideMesh.addVertex(shape[i+1]);
                sideMesh.addVertex(shape[i+1] + V3(0, height, 0));
                sideMesh.addVertex(shape[i] + V3(0, height, 0));
                
            } else {
                sideMesh.addVertex(shape[i]);
                sideMesh.addVertex(shape[i] + V3(0, 0, height));
                sideMesh.addVertex(shape[i+1] + V3(0, 0, height));
                sideMesh.addVertex(shape[i+1]);
                sideMesh.addVertex(shape[i]);
            }
            
        } else {
            if (yIsHeight) {
                sideMesh.addVertex(shape[i]);
                sideMesh.addVertex(shape[0]);
                sideMesh.addVertex(shape[0] + V3(0, height, 0));
                sideMesh.addVertex(shape[i] + V3(0, height, 0));
            } else {
                sideMesh.addVertex(shape[i]);
                sideMesh.addVertex(shape[i] + V3(0, 0, height));
                sideMesh.addVertex(shape[0] + V3(0, 0, height));
                sideMesh.addVertex(shape[0]);
                sideMesh.addVertex(shape[i]);
            }
        }
        
        sideMeshes.push_back(sideMesh);
    }
    
    //create floor vertices
    if(drawFloor) {
        VMSH tempFloor;
        MSH floorMesh;
        floorMesh.clear();
        
        for(int i = 0; i < shape.size(); i++)
            floorMesh.addVertex(shape[i]);
        
        tempFloor.push_back(floorMesh);
        finalMesh.push_back(tempFloor);
    }
    
    finalMesh.push_back(sideMeshes);
    finalMesh.push_back(tempTop);
    
    return finalMesh;
}

MSH myUtil::shapeExtrudeFace(PL shape, float height, bool yIsHeight) {
    
    ofMesh mesh;
    mesh.clear();
    
    V3 middlepoint = shape.getCentroid2D();
    
    //create floor vertices
    for(int i = 0; i < shape.size(); i++) {
        V3 diffb = shape[i] - middlepoint;
        mesh.addVertex(shape[i]);
        mesh.addNormal(glm::normalize(diffb));
    }
    
    //create top vertices
    for(int i = 0; i < shape.size(); i++) {
        
        V3 diffb;
        if (yIsHeight) {
            diffb = (shape[i] - middlepoint) + V3(0, height, 0);
            mesh.addVertex(shape[i] + V3(0, height, 0));
        }
        else {
            diffb = (shape[i] - middlepoint) + V3(0, 0, height);
            mesh.addVertex(shape[i] + V3(0, 0, height));
        }
        
        //add normal vector [vertex<-center] to vertex
        mesh.addNormal(glm::normalize(diffb));
    }
    
    //create side as triangles
    for (int i = 0; i<shape.size(); i++) {
        if (i<shape.size() - 1) { //
            // bottom1 - bottom2 - top1
            mesh.addTriangle(i, i + 1, shape.size() + i);
            //bottom2 - top2 - top1
            mesh.addTriangle(i + 1, shape.size() + i + 1, shape.size() + i);
        }
        else {
            //bottomLast - top1 - topLast
            mesh.addTriangle(i, i + 1, shape.size() + i);
            //bottomLast - bottom1 - top1
            mesh.addTriangle(i, 0, i + 1);
        }
    }
    
    //create top
    ofMesh topmesh;
    ofTessellator tess;
    tess.tessellateToMesh(shape, OF_POLY_WINDING_NONZERO, topmesh);
    int offsettwo = mesh.getNumVertices();
    for (auto &it : topmesh.getVertices()) {

        if (yIsHeight) {
            mesh.addVertex(it + V3(0, height,0 ));
            mesh.addNormal(V3(0, 1, 0));
        }
        else {
            mesh.addVertex(it + V3(0, 0, height));
            mesh.addNormal(V3(0, 0, 1));
        }
    }

    for (auto &it : topmesh.getIndices())
        mesh.addIndex(it+offsettwo);
    
    return mesh;
}

PL myUtil::makeCircle(V3 center, float radius) {
    PL circle;
    for(float deg = 0; deg <= 360; deg += 1) {
        float tempX = center.x + (glm::cos(glm::radians(deg)) * radius);
        float tempY = center.y + (glm::sin(glm::radians(deg)) * radius);
        V3 tempVec(tempX, tempY, center.z);
        
        circle.addVertex(tempVec);
    }
    
    return circle;
}

VMSH myUtil::circleExtrudeWireFrame(PL shape, float height,
                                    bool yIsHeight, bool drawFloor) {
    VMSH result;
    
    MSH floorMesh;
    floorMesh.clear();
    for(int i = 0; i < shape.size(); i++) {
            floorMesh.addVertex(shape[i]);
    }
    
    //create top vertices
    MSH topMesh;
    topMesh.clear();
    for(int i = 0; i < shape.size(); i++) {
        
        if (yIsHeight)
            topMesh.addVertex(shape[i] + V3(0, height, 0));
        else
            topMesh.addVertex(shape[i] + V3(0, 0, height));
    }
    
    result.push_back(floorMesh);
    result.push_back(topMesh);
    
    return result;
}

MSH myUtil::circleExtrudeFace(PL shape, float height, bool yIsHeight) {
    
    ofMesh mesh;
    mesh.clear();
    
    V3 middlepoint = shape.getCentroid2D();
    
    //create floor vertices
    for(int i = 0; i < shape.size(); i++) {
        V3 diffb = shape[i] - middlepoint;
        mesh.addVertex(shape[i]);
        
        //add normal vector [vertex<-center] to vertex
        mesh.addNormal(glm::normalize(diffb));
    }
    
    //create top vertices
    for(int i = 0; i < shape.size(); i++) {
        
        V3 diffb;
        if (yIsHeight) {
            diffb = (shape[i] - middlepoint) + V3(0, height, 0);
            mesh.addVertex(shape[i] + V3(0, height, 0));
        }
        else {
            diffb = (shape[i] - middlepoint) + V3(0, 0, height);
            mesh.addVertex(shape[i] + V3(0, 0, height));
        }
        
        //add normal vector [vertex<-center] to vertex
        mesh.addNormal(glm::normalize(diffb));
    }
    
    //create side as triangles
    for (int i = 0; i<shape.size(); i++) {
        if (i<shape.size() - 1) { //
            // bottom1 - bottom2 - top1
            mesh.addTriangle(i, i + 1, shape.size() + i);
            //bottom2 - top2 - top1
            mesh.addTriangle(i + 1, shape.size() + i + 1, shape.size() + i);
        }
        else {
            //bottomLast - top1 - topLast
            mesh.addTriangle(i, i + 1, shape.size() + i);
            //bottomLast - bottom1 - top1
            mesh.addTriangle(i, 0, i + 1);
        }
    }
    
    //create top
    ofMesh topmesh;
    ofTessellator tess;
    tess.tessellateToMesh(shape, OF_POLY_WINDING_NONZERO, topmesh);
    int offsettwo = mesh.getNumVertices();
    for (auto &it : topmesh.getVertices()) {
        
        if (yIsHeight) {
            mesh.addVertex(it + V3(0, height,0 ));
            mesh.addNormal(V3(0, 1, 0));
        }
        else {
            mesh.addVertex(it + V3(0, 0, height));
            mesh.addNormal(V3(0, 0, 1));
        }
    }
    
    for (auto &it : topmesh.getIndices())
        mesh.addIndex(it+offsettwo);
    
    return mesh;
}
