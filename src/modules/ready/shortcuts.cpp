#include "shortcuts.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace Modules;

bool ValidateShortcut(Shortcut& shortcut) {
    if ( shortcut.label == ""   )   { return false; }
    if ( shortcut.command == "" )   { return false; }

    return true;
}

void SimpleExec(std::string command) {
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Ошибка выполнения команды \"" << command << "\"\n"; 
    }
}

std::string GetStrFuncID(int modID, int shortcutID) {
    return std::to_string(modID) + "A1" + std::to_string(shortcutID);
}

std::unordered_map<uint32_t, std::function<void()>> Shortcuts::GetCustomInputRules() {
    std::unordered_map<uint32_t, std::function<void()>> output;

    int iterator = 0;
    for (auto shortcut : shortcuts) {
        if (ValidateShortcut(*shortcut)) {
            output[HashStr(GetStrFuncID(GetId(), iterator))] = std::bind( &SimpleExec, shortcut->command );
        }
        iterator++;
    }

    return output;
}

void Shortcuts::Start() {
    Update();
}

void Shortcuts::Update() {
    std::string output = "";

    for(int i = 0; i < shortcuts.size(); i++) {
        std::string shortcutLabel = "%{A1:" + GetStrFuncID(GetId(), i) + ":}" + shortcuts[i]->label + "%{A1}";
        output.append(shortcutLabel);
        if (i < shortcuts.size() - 1 ) { output.append(separator); }
    }

    SetOutput(output);
}

Shortcuts::Shortcuts() {  }

Shortcuts::~Shortcuts() {
    for (auto item : shortcuts) {
        delete item;
    }
}