#pragma once

#include <memory>
#include <vector>
#include "../modules/modules.hpp"

class ModulesLoader {
    public:
    static void Load(
        std::string modulesFolder
      , std::vector<std::shared_ptr<Modules::Module>> &left
      , std::vector<std::shared_ptr<Modules::Module>> &center
      , std::vector<std::shared_ptr<Modules::Module>> &right
      , std::vector<std::shared_ptr<Modules::Module>> &all
      );
};