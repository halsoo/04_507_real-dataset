#include "Logger.hpp"

Logger::Logger() {}

void Logger::setup() {
    xPos = LoggerX;
    yPos = LoggerY;
    firstGap = 278; //original : 278
    secondGap = 162; //original : 162
    yGap = 215; //origin : 204
    
    statusGap = 30; //original: 27
    logGap = 35; //original : 40
    
    titles.reserve(7);
    generalStatus.reserve(4);
    lastStatus.reserve(4);
    logs.reserve(3);
    
    initTitles();
    initGeneralStatus();
    initLastStatus();
}

void Logger::update() {}

void Logger::draw() {
    drawTitles();
    drawStatus();
    drawLogs();
}

void Logger::drawTitles() {
    drawPlainTitles();
    drawBlueTitles();
}

void Logger::initTitles() {
    initPlainTitles();
    initBlueTitles();
}

void Logger::initPlainTitles() {
    
    for(int i = 0; i < PlainTitles.size(); i++) {
        Text newTitle;
        
        newTitle.init(TitleFont, TitleSize);
        newTitle.setText(PlainTitles[i]);
        newTitle.setColor(ofColor::white);
        newTitle.wrapTextX(209);
        titles.push_back(newTitle);
    }
}

void Logger::drawPlainTitles() {
    for(int i = 0; i < PlainTitles.size(); i++)
        titles[i].draw(xPos, (yPos * 2) + (i * statusGap), 0);
}

void Logger::initBlueTitles() {
    
    for(int i = 0; i < BlueTitles.size(); i++) {
        Text newTitle;
        
        newTitle.init(TitleFont, TitleSize);
        newTitle.setText(BlueTitles[i]);
        newTitle.setColor(LoggerBlue);
        newTitle.wrapTextX(209);
        titles.push_back(newTitle);
    }
}

void Logger::drawBlueTitles() {
    //total
    titles[4].draw(xPos + firstGap, yPos, 0);
    //current block
    titles[5].draw(xPos + firstGap + secondGap, yPos, 0);
    //view full logs
    titles[6].draw(xPos, yPos + yGap, 0);
}

void Logger::drawStatus() {
    drawGeneralStatus();
    drawLastStatus();
}

void Logger::initGeneralStatus() {
    for(int i = 0; i < generalStatus.capacity(); i++) {
        Text newStatus;
        
        newStatus.init(LogFont, StatusSize);
        newStatus.wrapTextX(160);
        generalStatus.push_back(newStatus);
    }
}
void Logger::updateGeneralSatus(int traffic, int accuTraffic,
                                int size, int fileNum, int failCnt) {
    
    std::string footPrint = std::to_string(accuTraffic * 4) + "/dg";
    generalStatus[0].setText(footPrint);
    
    float calcEf = (float(fileNum) / float(size)) * 100;
    calcEf = 100 - calcEf;
    
    std::ostringstream truncEf;
    
    truncEf << std::fixed;
    truncEf << std::setprecision(2);
    truncEf << calcEf;
    
    std::string efficiency = truncEf.str() + " %";
    generalStatus[1].setText(efficiency);
    
    std::string trafficStr = std::to_string(traffic);
    generalStatus[2].setText(trafficStr);
    
    std::string count = std::to_string(failCnt);
    generalStatus[3].setText(count);
}

void Logger::drawGeneralStatus() {
    for(int i = 0; i < generalStatus.size(); i++) {
        generalStatus[i].setColor(LoggerBlue);
        generalStatus[i].draw(xPos + firstGap, (yPos * 2) + (i * statusGap), 0);
    }
}

void Logger::initLastStatus() {
    for(int i = 0; i < lastStatus.capacity(); i++) {
        Text newStatus;

        newStatus.init(LogFont, StatusSize);
        newStatus.setColor(LoggerBlue);
        newStatus.wrapTextX(160);
        lastStatus.push_back(newStatus);
    }
}
void Logger::updateLastStatus(int traffic) {
    std::string footPrint = std::to_string(traffic * 4) + "/dg";
    lastStatus[0].setText(footPrint);
    lastStatus[0].setColor(LoggerBlue);
    
    lastStatus[1].setText("[none]");
    lastStatus[1].setColor(LoggerYellow);
    
    std::string trafficStr = std::to_string(traffic);
    lastStatus[2].setText(trafficStr);
    lastStatus[2].setColor(LoggerBlue);
    
    lastStatus[3].setText("[none]");
    lastStatus[3].setColor(LoggerYellow);
}

void Logger::drawLastStatus() {
    for(int i = 0; i < lastStatus.size(); i++) {
        lastStatus[i].draw(xPos + firstGap + secondGap,
                           (yPos * 2) + (i * statusGap), 0);
    }
}

void Logger::updateLogs(uint64_t fileName, uint64_t frames) {
    uint64_t seconds = frames/ofGetFrameRate();
    std::string nameString = std::to_string(fileName);
    
    uint64_t hour = seconds/3600;
    uint64_t minute = (seconds - hour) / 60;
    uint64_t second = seconds - (hour + minute);
    int subSecond = myUtil::randomInt(10, 999);
    
    hour += 13;
    
    if(hour > 24)
        hour = 0;
    
    std::string hourLog = std::to_string(hour);
    if(hour < 10)
        hourLog = "0" + hourLog;
    
    std::string minuteLog = std::to_string(minute);
    if(minute < 10)
        minuteLog = "0" + minuteLog;
    
    std::string secondLog = std::to_string(second);
    if(second < 10)
        secondLog = "0" + minuteLog;
    
    std::string reqSubSecondLog = std::to_string(subSecond);
    if(subSecond < 100)
        reqSubSecondLog = "0" + reqSubSecondLog;
    
//    int resSubSecond = subSecond + myUtil::randomInt(1, 30);
//    std::string resSubSecondLog = std::to_string(resSubSecond);
//    if(resSubSecond < 100)
//        resSubSecondLog = resSubSecondLog;
    
    std::string TimeStampTime
    = hourLog + ":" + minuteLog + ":" + secondLog + ".";
    
    std::string reqLog
    = TimeStampDate + " " + TimeStampTime + reqSubSecondLog + " " + "file " + nameString + " uploaded";
//    std::string resLog
//    = TimeStampDate + " " + TimeStampTime + resSubSecondLog + " " + "res file " + nameString;
    
    addLog(reqLog);
    //addLog(resLog);
}

void Logger::addLog(std::string log) {
    if(logs.size() > 5)
        logs.erase(logs.begin());
    
    Text newLog;
    newLog.init(LogFont, LogSize);
    newLog.setText(log);
    newLog.setColor(ofColor::white);
    newLog.wrapTextForceLines(1);
    logs.push_back(newLog);
}

void Logger::drawLogs() {
    for(int i = 0; i < logs.size(); i++) {
//        if(i % 2 == 0)
//            logs[i].draw(356, 265 + (i * 40), 0);
//        else
//            logs[i].draw(356 + 30, 265 + (i * 40), 0);
        logs[i].draw(xPos + firstGap, (yPos + yGap) + (i * logGap), 0);
    }
}
