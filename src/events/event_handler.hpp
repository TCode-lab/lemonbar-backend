#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "../modules/modules.hpp"

using namespace Modules;

class EventHandler {
private:
    static std::unordered_map<uint32_t, std::function<void()>> hashedEventsMap;

public:
    static void Initialize(std::vector<std::shared_ptr<Module>> &modules);
    static void Process(std::string command);
};