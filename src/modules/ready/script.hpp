#pragma once

#include "../module_basic.hpp"

class loader;

namespace Modules {
    class Script : public Module {
        friend loader;

        private:
        std::string command = "bash";
        std::string onStartPath = "";
        std::string onUpdatePath = "";
        std::string onClickLeftPath = "";
        std::string onClickMiddlePath = "";
        std::string onClickRightPath = "";
        std::string onWheelUpPath = "";
        std::string onWheelDownPath = "";

        public:
        void        Start       ()  override;
        void        Update      ()  override;

        void        OnLeftClick     ()   override;
        void        OnMiddleClick   ()   override;
        void        OnRightClick    ()   override;
        void        OnWheelUp       ()   override;
        void        OnWheelDown     ()   override;
        
        ~Script();
    };
}