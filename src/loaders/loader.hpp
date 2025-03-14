#include <memory>

#include "../../include/json.hpp"
#include "../modules/modules.hpp"

using namespace Modules;
using json = nlohmann::json;

class loader {
public:
    template <typename T>
    static std::shared_ptr<T> load(json js, int interval, Modules::Position pos) {
        return std::shared_ptr<T>();
    };
};

template <>
std::shared_ptr<Modules::CPU> loader::load<Modules::CPU>(json js, int interval, Modules::Position pos);

template <>
std::shared_ptr<Modules::RAM> loader::load<Modules::RAM>(json js, int interval, Modules::Position pos);

template <>
std::shared_ptr<Modules::Battery> loader::load<Modules::Battery>(json js, int interval, Modules::Position pos);

template <>
std::shared_ptr<Modules::Time> loader::load<Modules::Time>(json js, int interval, Modules::Position pos);

template <>
std::shared_ptr<Modules::Script> loader::load<Modules::Script>(json js, int interval, Modules::Position pos);

template <>
std::shared_ptr<Modules::Shortcuts> loader::load<Modules::Shortcuts>(json js, int interval, Modules::Position pos);