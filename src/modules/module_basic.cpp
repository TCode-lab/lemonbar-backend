#include "module_basic.hpp"
#include <string>

using namespace Modules;

void Module::Init(
    unsigned int  _updInterval
  , Position      _position
  , std::string   _bgColor
  , std::string   _fontColor
) {
    updInterval = _updInterval;
    position = _position;
    bgColor = _bgColor;
    fontColor = _fontColor;
}

unsigned int Module::GetInterval() {
    return updInterval;
}

Position Module::GetPosition() {
    return position;
}

constexpr auto SetInputFormat = [](std::string &formatStart, std::string &formatEnd, const std::string &inputCode, uint32_t id) {
    formatStart.append  ("%{" + inputCode + ":" + std::to_string(id) + inputCode + ":}");
    formatEnd.append    ("%{" + inputCode + "}");
};

void Module::SetOutput(std::string _output) {
    std::string formatStart = "";
    std::string formatEnd = "";
    if ( bgColor != "" ) {
        formatStart.append  ("%{B" + bgColor + "}");
        formatEnd.append    ("%{B-}");
    }

    if (fontColor != "") {
        formatStart.append  ("%{F" + fontColor + "}");
        formatEnd.append    ("%{F-}");
    }

    if (isAllowLeftClick)   { SetInputFormat(formatStart, formatEnd, "A1", this->id); }
    if (isAllowMiddleClick) { SetInputFormat(formatStart, formatEnd, "A2", this->id); }
    if (isAllowRightClick)  { SetInputFormat(formatStart, formatEnd, "A3", this->id); }
    if (isAllowWheelUp)     { SetInputFormat(formatStart, formatEnd, "A4", this->id); }
    if (isAllowWheelDown)   { SetInputFormat(formatStart, formatEnd, "A5", this->id); }

    output = formatStart + " " + _output + " " + formatEnd;
}

std::string Module::GetOut() {
    return output;
}

uint32_t Module::GetId() {
    return id;
}