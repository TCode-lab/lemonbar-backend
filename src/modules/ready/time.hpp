#pragma once

#include "../module_basic.hpp"


namespace Modules {
    class Time : public Module {
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