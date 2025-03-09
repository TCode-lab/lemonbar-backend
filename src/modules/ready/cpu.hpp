#pragma once

#include "../module_basic.hpp"

class loader;

namespace Modules {
    class CPU : public Module {
        friend loader;

        private:
        std::string iconCPU = " ";

        bool        showTemp        =   true    ;
        std::string iconTempCold    =   " "    ;
        std::string iconTempCool    =   " "    ;
        std::string iconTempMedium  =   " "    ;
        std::string iconTempWarm    =   " "    ;
        std::string iconTempHot     =   " "  ;
        public:
        void        Start   ()  override;
        void        Update  ()  override;
        
        ~CPU();
    };
}