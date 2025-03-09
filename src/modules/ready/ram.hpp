#pragma once

#include "../module_basic.hpp"

class loader;

namespace Modules {
    class RAM : public Module {
        friend loader;

        private:
        std::string icon = " ";

        public:
        void        Start   ()  override;
        void        Update  ()  override;
        
        ~RAM();
    };
}