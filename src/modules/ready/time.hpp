#pragma once

#include "../module_basic.hpp"

class loader;

namespace Modules {
    class Time : public Module {
        friend loader;

        private:
        int         basicInterval   ;
        bool        shortOut = false;

        public:
        void        Start   ()  override;
        void        Update  ()  override;
        void        OnLeftClick ()  override;
        
        Time();
        ~Time();
    };
}