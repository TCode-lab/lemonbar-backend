#include "ram.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace Modules;

int getAvailable() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    unsigned long totalMem = 0;
    unsigned long availableMem = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal") == 0) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> totalMem;
        } else if (line.find("MemAvailable") == 0) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> availableMem;
        }
    }

    if (totalMem == 0) {
        return -1;
    }

    return static_cast<double>(totalMem - availableMem) / totalMem * 100.0;
}

void RAM::Start() {
    Update();
}

void RAM::Update() {
    #define TEMP_ERROR_ICON " "
    int percents = getAvailable();

    if (percents > 0) {
        SetOutput(icon + std::to_string(percents) + "%%");
    }
    else {
        SetOutput(TEMP_ERROR_ICON);
    }
}

RAM::~RAM() {

}