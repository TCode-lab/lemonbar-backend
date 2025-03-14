#include "loader.hpp"
#include <exception>
#include <iostream>

constexpr auto TryFillField = [](auto &field, json &jsModule, const std::string &jsFieldName, std::string &errStr) {
    if (jsModule.count(jsFieldName) < 1) { 
        errStr.append("   Field \"" + jsFieldName + "\" not found.\n");
        return false;
    }

    try { field = jsModule[jsFieldName].get<std::decay_t<decltype(field)>>(); return true; }
    catch (std::exception &err) { errStr.append("   Field \"" + jsFieldName + "\" incorrect: \"" + std::string(err.what()) + "\"\n"); }
    return false;
};

void Log(const std::string &msg) {
    std::cerr << msg << std::endl;
}

template <>
std::shared_ptr<Modules::CPU> loader::load<Modules::CPU>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::CPU> out = std::make_shared<Modules::CPU>();
    out->Init(interval, pos);

    std::string err = "";

    TryFillField(out->iconCPU,        js, "icon_cpu", err        );
    TryFillField(out->iconTempCold,   js, "icon_temp_cold", err  );
    TryFillField(out->iconTempCool,   js, "icon_temp_cool", err  );
    TryFillField(out->iconTempMedium, js, "icon_temp_medium", err);
    TryFillField(out->iconTempWarm,   js, "icon_temp_warm", err  );
    TryFillField(out->iconTempHot,    js, "icon_temp_hot", err   );
    TryFillField(out->showTemp,       js, "show_temp", err       );

    if (err != "") { std::cerr << err << std::endl; }
    return out;
}

template <>
std::shared_ptr<Modules::RAM> loader::load<Modules::RAM>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::RAM> out = std::make_shared<Modules::RAM>();
    out->Init(interval, pos);
    
    std::string err = "";

    TryFillField(out->icon, js, "icon", err);

    if (err != "") { std::cerr << err << std::endl; }
    return out;
}

template <>
std::shared_ptr<Modules::Battery> loader::load<Modules::Battery>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::Battery> out = std::make_shared<Modules::Battery>();
    out->Init(interval, pos);
    
    std::string err = "";

    TryFillField(out->iconFull, js, "icon_full", err);
    TryFillField(out->iconFourFifths, js, "icon_four_fifths", err);
    TryFillField(out->iconOneHalf, js, "icon_one_half", err);
    TryFillField(out->iconTwoFifths, js, "icon_two_fifths", err);
    TryFillField(out->iconOneFifths, js, "icon_one_fifths", err);

    TryFillField(out->iconStatusCharging, js, "icon_status_charging", err);
    TryFillField(out->iconStatusDischarging, js, "icon_status_discharging", err);
    TryFillField(out->iconStatusFull, js, "icon_status_full", err);

    if (err != "") { std::cerr << err << std::endl; }
    return out;
}

template <>
std::shared_ptr<Modules::Time> loader::load<Modules::Time>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::Time> out = std::make_shared<Modules::Time>();
    out->Init(interval, pos);
    
    std::string err = "";
    
    TryFillField(out->shortOut, js, "short_out", err);

    if (err != "") { std::cerr << err << std::endl; }
    return out;
}

template <>
std::shared_ptr<Modules::Script> loader::load<Modules::Script>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::Script> out = std::make_shared<Modules::Script>();
    out->Init(interval, pos);
    
    std::string err = "";

    TryFillField(out->command, js, "command", err);

    TryFillField(out->onStartPath, js, "on_start", err);
    TryFillField(out->onUpdatePath, js, "on_update", err);
    
    if (TryFillField(out->onClickLeftPath, js, "on_click_left", err))       { out->isAllowLeftClick = true;     }
    if (TryFillField(out->onClickMiddlePath, js, "on_click_middle", err))   { out->isAllowMiddleClick = true;   }
    if (TryFillField(out->onClickRightPath, js, "on_click_right", err))     { out->isAllowRightClick = true;    }
    if (TryFillField(out->onWheelUpPath, js, "on_wheel_up", err))           { out->isAllowWheelUp = true;       }
    if (TryFillField(out->onWheelDownPath, js, "on_wheel_down", err))       { out->isAllowWheelDown = true;     }

    if (err != "") { std::cerr << err << std::endl; }
    return out;
}

bool ValidataJSShortcut(json js) {
    if ( js.count("label") < 1   )  { return false; }
    if ( js.count("command") < 1 )  { return false; }
    return true;
}

template <>
std::shared_ptr<Modules::Shortcuts> loader::load<Modules::Shortcuts>(json js, int interval, Modules::Position pos) {
    std::shared_ptr<Modules::Shortcuts> out = std::make_shared<Modules::Shortcuts>();
    out->Init(interval, pos);

    std::string err = "";

    TryFillField(out->separator, js, "separator", err);

    if(js.count("shortcuts") > 0) {
        auto shortcutsArray = js["shortcuts"];
        if (shortcutsArray.size() > 0) {
            Shortcut* shortcut;
            for (auto jsShortcut : shortcutsArray) {
                if (ValidataJSShortcut(jsShortcut)) {
                    shortcut = new Shortcut();
                    bool result = true;
                    result = TryFillField(shortcut->label, jsShortcut, "label", err);
                    result = TryFillField(shortcut->command, jsShortcut, "command", err);
                    if (!result) { continue; }
                    out->shortcuts.push_back(shortcut);
                }
            }
        }
    }

    if (!out->shortcuts.empty()) {
        out->isAllowCustomInput = true;
    }
    
    if (err != "") { std::cerr << err << std::endl; }
    return out;
}