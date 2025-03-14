#include "modules_loader.hpp"
#include <exception>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>

#include "../../include/json.hpp"

#include "loader.hpp"

using namespace Modules;
using json = nlohmann::json;

void ModulesLoader::Load(
    std::string modulesFolder,
    std::vector<std::shared_ptr<Module>> &left,
    std::vector<std::shared_ptr<Module>> &center,
    std::vector<std::shared_ptr<Module>> &right,
    std::vector<std::shared_ptr<Module>> &all
) {
    //
    // Загрузка модулей из папки
    //
    std::vector<std::shared_ptr<Module>> loadedModules;

    try{
        std::ifstream f(modulesFolder + "/modules.json");
        if (!f.is_open()) {
            throw std::runtime_error("Невозможно открыть файл \"" + modulesFolder + "/modules.json\"");
        }

        json data = json::parse(f);
        if (data.count("modules_list") < 1) {
            throw std::runtime_error("В \"" + modulesFolder + "/modules.json\" отсутствует поле \"modules_list\"");
        }
        
        auto FieldValidation = [](int index, const std::string& fieldName, json &module) {
            if (module.count(fieldName) < 1) {
                std::cerr << "Failed to load module with index " << index << "\n   Field \"" << fieldName << "\" not found." << std::endl;
                return false;
            }
            return true;
        };

        auto TryGet = [](auto &output, const json &module, const std::string &fieldName, std::string &errOutput) {
            try {
                output = module[fieldName].get<std::decay_t<decltype(output)>>();
            }
            catch (const std::exception &err) {
                errOutput.append(std::string(err.what()) + "\n");
                return false;
            }
            return true;
        };
        
        for (int i = 0; i < data["modules_list"].size(); i++) {
            json module = data["modules_list"][i];

            // Проверка обязательных полей
            if (!FieldValidation(i, "type",     module)) { continue; }
            if (!FieldValidation(i, "interval", module)) { continue; }
            if (!FieldValidation(i, "position", module)) { continue; }

            int interval                =  0;
            std::string positionStr     =  "";
            std::string typeStr         =  "";
            Modules::Position position  =  Modules::Position::Left;

            std::string err = "";

            // Попытка получения базовых полей
            TryGet(     typeStr,     module, "type",     err);
            TryGet(     interval,    module, "interval", err);
            if  (TryGet(positionStr, module, "position", err)) {
                position = Modules::GetPosByStr(positionStr);
                if (position == Modules::Position::Unknown) { err.append("   Unknown module position.\n"); }
            }

            if (err != "") {
                std::cerr << "Failed to load module with index " << i << std::endl << err << std::endl;
                continue;
            }

            switch(Modules::GetTypeByStr(typeStr)) {
                case Modules::Type::CPU:
                    loadedModules.push_back(loader::load<Modules::CPU>(module, interval, position));
                    break;
                case Modules::Type::RAM:
                    loadedModules.push_back(loader::load<Modules::RAM>(module, interval, position));
                    break;
                case Modules::Type::Battery:
                    loadedModules.push_back(loader::load<Modules::Battery>(module, interval, position));
                    break;
                case Modules::Type::Time:
                    loadedModules.push_back(loader::load<Modules::Time>(module, interval, position));
                    break;
                case Modules::Type::Script:
                    loadedModules.push_back(loader::load<Modules::Script>(module, interval, position));
                    break;
                case Modules::Type::Shortcuts:
                    loadedModules.push_back(loader::load<Modules::Shortcuts>(module, interval, position));
                    break;

                default: std::cerr << "Unknown module type on module with index " << i << std::endl; continue; break;
            }
        }
    }
    catch (std::exception &err) {
        std::cout << err.what();
    }

    //
    // Сортировка
    //
    {
        int iteration = 0;
        for (const auto& module : loadedModules) {
            module->id = iteration;
            switch (module->GetPosition()) {
                case Position::Left:
                    left.push_back(module);
                    break;
                case Position::Center:
                    center.push_back(module);
                    break;
                case Position::Right:
                    right.push_back(module);
                    break;
                default: break;
            }
            all.push_back(module);
            iteration++;
        }
    }
}