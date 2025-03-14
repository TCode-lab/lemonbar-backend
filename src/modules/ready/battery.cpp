#include "battery.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>

using namespace Modules;

std::map <std::string, int> states_map;

enum States : uint8_t {
    Charging = 0,
    Discharging = 1,
    Full = 2,
    NotCharging = 3
};

void Battery::Start() {
    states_map["Charging"]      =   States::Charging;
    states_map["Discharging"]   =   States::Discharging;
    states_map["Full"]          =   States::Full;
    states_map["Not charging"]  =   States::NotCharging;
    Update();
}

void Battery::Update() {
    #define BAT_PATH std::string("/sys/class/power_supply/BAT0")

    #define ERR_NOTFOUND " "
    #define ERR_CODEDMG " "
    #define FINISH(OUT) capacityFile.close(); statusFile.close(); SetOutput(OUT); return;

    std::ifstream capacityFile  (BAT_PATH + "/capacity");
    std::ifstream statusFile    (BAT_PATH + "/status");

    int capacity;
    std::string status;
    std::string icon = iconOneFifths;

    if (!capacityFile.is_open() || !statusFile.is_open()) { FINISH(ERR_NOTFOUND); }

    std::string line;
    if (std::getline(capacityFile, line)) {
        try {
            capacity = std::stoi(line);
            if (capacity > 90) {
                icon = iconFull;
            }
            else if (capacity > 70) {
                icon = iconFourFifths;
            }
            else if (capacity > 40) {
                icon = iconOneHalf;
            }
            else if (capacity > 10) {
                icon = iconTwoFifths;
            }
            else {
                icon = iconOneFifths;
            }
        } 
        catch (...) {
            FINISH(ERR_CODEDMG);
        }
    }
    else {
        FINISH(ERR_NOTFOUND);
    }

    if (std::getline(statusFile, line)) {
        if (states_map.count(line) < 1) {
            FINISH(ERR_CODEDMG);
        }
        
        switch (states_map[line]) {
            case States::Charging:
                status = iconStatusCharging;
                break;
            case States::Discharging:
                status = iconStatusDischarging;
                break;
            case States::Full:
                status = iconStatusFull;
                break;
            case States::NotCharging:
                status = "НАПРЯЖЕНИЕ СЛАБОЕ БЛЯТЬ";
                break;
            default:
                status = "";
                break;
        }
    }
    else {
        FINISH(ERR_NOTFOUND);
    }

    FINISH(icon + std::to_string(capacity) + "%%" + status);
}

Battery::~Battery() {
    
}