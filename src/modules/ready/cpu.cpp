#include "cpu.hpp"

#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <filesystem>

#include "../../loaders/loader.hpp"

namespace fs = std::filesystem;

using namespace Modules;

void CPU::Start() {
    Update();
}

struct CpuStats {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
    unsigned long guest;
    unsigned long guest_nice;
};

std::vector<CpuStats> getCpuStats() {
    std::ifstream statFile("/proc/stat");
    std::vector<CpuStats> stats;
    std::string line;

    while (std::getline(statFile, line)) {
        if (line.compare(0, 3, "cpu") == 0 && line.size() > 3) {
            std::istringstream iss(line);
            std::string cpuLabel;
            iss >> cpuLabel;

            CpuStats coreStats{};
            iss >> coreStats.user >> coreStats.nice >> coreStats.system >> coreStats.idle >>
                   coreStats.iowait >> coreStats.irq >> coreStats.softirq >> coreStats.steal >>
                   coreStats.guest >> coreStats.guest_nice;

            stats.push_back(coreStats);
        }
    }

    return stats;
}

double calculateCpuUsage(const CpuStats& prev, const CpuStats& curr) {
    unsigned long prevIdle = prev.idle + prev.iowait;
    unsigned long currIdle = curr.idle + curr.iowait;

    unsigned long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    unsigned long currNonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;

    unsigned long prevTotal = prevIdle + prevNonIdle;
    unsigned long currTotal = currIdle + currNonIdle;

    unsigned long totalDiff = currTotal - prevTotal;
    unsigned long idleDiff = currIdle - prevIdle;

    if (totalDiff == 0) return 0.0;
    return static_cast<double>(totalDiff - idleDiff) / totalDiff * 100.0;
}

int getAverageCpuUsage() {
    auto prevStats = getCpuStats();
    std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Ждем 300 мс

    auto currStats = getCpuStats();
    double totalUsage = 0.0;
    size_t numCores = currStats.size();

    for (size_t i = 0; i < numCores; ++i) {
        totalUsage += calculateCpuUsage(prevStats[i], currStats[i]);
    }

    double averageUsage = totalUsage / numCores;
    return static_cast<int>(std::round(averageUsage));
}

int getTemp() {
    std::vector<int> temperatures;
    std::string thermalPath = "/sys/class/thermal";

    for (const auto& entry : fs::directory_iterator(thermalPath)) {
        if (entry.path().string().find("thermal_zone") != std::string::npos) {
            std::ifstream tempFile(entry.path().string() + "/temp");
            std::ifstream tempType(entry.path().string() + "/type");

            if (tempType.is_open()) { 
                std::string type;
                tempType >> type;
                if (type == "acpitz") { continue; } //Нахуй внешнюю температуру проца
            }

            if (tempFile.is_open()) {
                int temp;
                tempFile >> temp;
                temp /= 1000;
                temperatures.push_back(temp);
            }
        }
    }

    if (temperatures.empty()) {
        return -1;
    }

    int sum = 0;
    for (int temp : temperatures) {
        sum += temp;
    }

    return static_cast<int>(std::round(static_cast<double>(sum) / temperatures.size()));
}

void CPU::Update() {
    const std::string errorTempIcon = " ";

    int cpuUsage = getAverageCpuUsage();
    std::string out = iconCPU + std::to_string(cpuUsage) + "%%";
    
    if (showTemp) {
        int cpuTemp = getTemp();

        std::string tempIcon = iconTempCold;
        if      (cpuTemp > 80) { tempIcon = iconTempHot;    }
        else if (cpuTemp > 60) { tempIcon = iconTempWarm;   }
        else if (cpuTemp > 45) { tempIcon = iconTempMedium; }
        else if (cpuTemp > 20) { tempIcon = iconTempCool;   }
        else                   { tempIcon = iconTempCold;   }

        if (cpuTemp > 0) {
            out.append(" " + tempIcon + std::to_string(cpuTemp) + "°C");
        } else {
            out.append(errorTempIcon);
        }
    }

    SetOutput(out);
}

CPU::~CPU() {

}