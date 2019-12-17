#pragma once

#include "ofMain.h"
#include "ofxTextSuite.h"

using VI = std::vector<int>;

using V2 = glm::vec2;
using VV2 = std::vector<glm::vec2>;
using VVV2 = std::vector<std::vector<glm::vec2>>;

using V3 = glm::vec3;
using VV3 = std::vector<glm::vec3>;
using VVV3 = std::vector<std::vector<glm::vec3>>;
using VV3IT = std::vector<glm::vec3>::iterator;
using VV3VIT = std::vector<std::vector<glm::vec3>::iterator>;
using PVV3IT = std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator>;

using VSt = std::vector<std::string>;
using VVSt = std::vector<std::vector<std::string>>;

using VB = std::vector<bool>;
using PBI = std::pair<bool, int>;
using PBB = std::pair<bool, bool>;
using VPBI = std::vector<std::pair<bool, int>>;

using PL = ofPolyline;
using VPL = std::vector<ofPolyline>;

using MSH = ofMesh;
using VMSH = std::vector<ofMesh>;
using VVMSH = std::vector<std::vector<ofMesh>>;

using VCol = std::vector<ofColor>;

using Text = ofxTextBlock;
using TextAlign = TextBlockAlignment;
using VTx = std::vector<ofxTextBlock>;
