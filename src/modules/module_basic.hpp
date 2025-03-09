#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class ModulesLoader;
class EventHandler;

namespace Modules {
    enum class Position : uint8_t {
        Unknown,
        Left,
        Center,
        Right
    };

    enum class Type : uint8_t {
        Unknown,
        CPU,
        Battery,
        RAM,
        Time,
        Script
    };

    class Module {
        friend ModulesLoader;
        friend EventHandler;

    private:
        uint32_t id;
        std::string output;
        
    protected:
        unsigned int updInterval;
        Position position;
        std::string bgColor, fontColor;

        bool isAllowLeftClick   = false;
        bool isAllowMiddleClick = false;
        bool isAllowRightClick  = false;
        bool isAllowWheelUp     = false;
        bool isAllowWheelDown   = false;

        bool IsAllowClick() { return isAllowLeftClick || isAllowRightClick || isAllowMiddleClick || isAllowWheelUp || isAllowWheelDown; }
        void SetOutput (std::string _output);

        virtual void    OnLeftClick     () {};
        virtual void    OnMiddleClick   () {};
        virtual void    OnRightClick    () {};
        virtual void    OnWheelUp       () {};
        virtual void    OnWheelDown     () {};
    public:
        bool isNeedUpdNow = false;

        void Init(
              unsigned int  _updInterval    = 5
            , Position      _position       = Position::Left
            , std::string   _bgColor        = ""
            , std::string   _fontColor      = ""
        );
    
        virtual void    Start       ()  {};
        virtual void    Update      ()  {};
        
        uint32_t        GetId           ();
        std::string     GetOut          ();
        unsigned int    GetInterval     ();
        Position        GetPosition     ();

        virtual ~Module() {};
    };

    constexpr uint32_t HashStr(const std::string_view str) {
        uint32_t hash = 5381;
        for (char c : str) {
            hash = (hash << 5) + hash + c;
        }
        return hash;
    }

    inline Type GetTypeByStr(const std::string &type) {
        constexpr uint32_t cpuHash      =   HashStr("CPU");
        constexpr uint32_t ramHash      =   HashStr("RAM");
        constexpr uint32_t batteryHash  =   HashStr("Battery");
        constexpr uint32_t timeHash     =   HashStr("Time");
        constexpr uint32_t scriptHash   =   HashStr("Script");

        switch (HashStr(type)) {
            case cpuHash:       return Type::CPU;
            case ramHash:       return Type::RAM;
            case batteryHash:   return Type::Battery;
            case timeHash:      return Type::Time;
            case scriptHash:    return Type::Script;
            default:            return Type::Unknown;
        }
    }

    inline Position GetPosByStr(const std::string &position) {
        constexpr uint32_t leftHash     =   HashStr("Left");
        constexpr uint32_t centerHash   =   HashStr("Center");
        constexpr uint32_t rightHash    =   HashStr("Right");

        switch (HashStr(position)) {
            case leftHash:      return Position::Left;
            case centerHash:    return Position::Center;
            case rightHash:     return Position::Right;
            default:            return Position::Unknown;
        }
    }
}