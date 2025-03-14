#include "../module_basic.hpp"
#include <vector>

class loader;

class Shortcut {
    public:
    std::string label = "";
    std::string command = "";
};

namespace Modules {
    class Shortcuts : public Module {
        friend loader;

        private:
        std::vector<Shortcut*> shortcuts;

        std::string separator = " ";
        
        public:
        void        Start   ()  override;
        void        Update  ()  override;

        std::unordered_map<uint32_t, std::function<void()>> GetCustomInputRules () override;

         Shortcuts();
        ~Shortcuts();
    };
}

