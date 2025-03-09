#pragma once

#include "../module_basic.hpp"

class loader;

namespace Modules {
    class Battery : public Module {
        friend loader;
        
        private:
        std::string iconFull        =   " ";
        std::string iconFourFifths  =   " ";
        std::string iconOneHalf     =   " ";
        std::string iconTwoFifths   =   " ";
        std::string iconOneFifths   =   " ";

        std::string iconStatusCharging      =   "";
        std::string iconStatusDischarging   =   " ";
        std::string iconStatusFull          =   " ";
        public:
        void        Start   ()  override;
        void        Update  ()  override;
        
        ~Battery();
    };
}