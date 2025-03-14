#include "event_handler.hpp"

#include <exception>
#include <vector>
#include <iostream>

std::unordered_map<uint32_t, std::function<void()>> EventHandler::hashedEventsMap;

void EventHandler::Initialize(std::vector<std::shared_ptr<Module>> &modules) {
    for (auto module : modules) {
        if (!module->isAllowCustomInput) {
            if (module->isAllowLeftClick) { 
                hashedEventsMap[HashStr(std::to_string(module->id) + "A1")] = std::bind(&Module::OnLeftClick, module); 
            };
    
            if (module->isAllowMiddleClick) { 
                hashedEventsMap[HashStr(std::to_string(module->id) + "A2")] = std::bind(&Module::OnMiddleClick, module); 
            };
    
            if (module->isAllowRightClick) { 
                hashedEventsMap[HashStr(std::to_string(module->id) + "A3")] = std::bind(&Module::OnRightClick, module); 
            };
    
            if (module->isAllowWheelUp) { 
                hashedEventsMap[HashStr(std::to_string(module->id) + "A4")] = std::bind(&Module::OnWheelUp, module); 
            };
    
            if (module->isAllowWheelDown) { 
                hashedEventsMap[HashStr(std::to_string(module->id) + "A5")] = std::bind(&Module::OnWheelDown, module); 
            };
        }
        else {
            try {
                auto rules = module->GetCustomInputRules();
                for (auto rule : rules) {
                    hashedEventsMap[rule.first] = rule.second;
                }
            } 
            catch (std::exception &err) {
                std::cout << err.what() << std::endl;
            }
        }
    }
}

void EventHandler::Process(std::string command) {
    uint32_t commandHash = HashStr(command);
    if (hashedEventsMap.count(commandHash) > 0) {
        hashedEventsMap[commandHash]();
    }
    else {
        std::cerr << "Unknown \"" << command << "(hash: " << commandHash << ")\" command" << std::endl;
    }
}