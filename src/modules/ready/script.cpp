#include "script.hpp"

#include <cstdio>
#include <exception>
#include <memory>
#include <array>
#include <stdexcept>
#include <algorithm>

using namespace Modules;

void removeNewlineСhars(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {
        return c == '\n' || c == '\r';
    }), str.end());
}

std::string exec(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    try {
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("Ошибка при вызове popen()");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    catch (std::exception &err) {
        result = "";
    }
    removeNewlineСhars(result);
    return result;
}

void Script::Start() {
    if (onStartPath != "") {
        SetOutput(exec(command + " " + onStartPath));
    } else {
        Update();
    }
}

void Script::Update() {
    SetOutput(exec(command + " " + onUpdatePath));
}

void Script::OnLeftClick() {
    SetOutput(exec(command + " " + onClickLeftPath));
}

void Script::OnMiddleClick() {
    SetOutput(exec(command + " " + onClickMiddlePath));
}

void Script::OnRightClick() {
    SetOutput(exec(command + " " + onClickRightPath));
}

void Script::OnWheelUp() {
    SetOutput(exec(command + " " + onWheelUpPath));
}

void Script::OnWheelDown() {
    SetOutput(exec(command + " " + onWheelDownPath));
}

Script::~Script() {
    
}