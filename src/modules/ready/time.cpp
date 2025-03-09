#include "time.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <ostream>

using namespace Modules;

std::stringstream ss;

void Time::Start() {
    basicInterval = updInterval;
    Update(); 
}

void Time::Update() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time = *std::localtime(&now_time);

    std::string format = "%H:%M";
    if (!shortOut) {
        format = "%Y.%m.%d %H:%M:%S";
        updInterval = 1;
    }
    else {
        updInterval = basicInterval;
    }

    ss.str(""); ss.clear();
    ss << std::put_time(&local_time, format.c_str()) << std::flush;
    SetOutput(ss.str());
}

void Time::OnLeftClick() {
    if (shortOut == true) {
        shortOut = false;
        updInterval = basicInterval;
    }
    else {
        shortOut = true;
        updInterval = 1;
    }
    isNeedUpdNow = true;
}

Time::Time() { isAllowLeftClick = true; }
Time::~Time() {}

