#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <unistd.h>

#include <thread>
#include <unistd.h>
#include <atomic>

#include "loaders/modules_loader.hpp"
#include "modules/module_basic.hpp"
#include "modules/modules.hpp"

#include "events/event_handler.hpp"

//
// Поток рендера
//

int RenderThread(
      std::atomic<bool>& running
    , std::vector<std::shared_ptr<Modules::Module>>& left
    , std::vector<std::shared_ptr<Modules::Module>>& center
    , std::vector<std::shared_ptr<Modules::Module>>& right
    , std::vector<std::shared_ptr<Modules::Module>>& all
) {
    std::string separator = "|";
    
    std::stringstream leftStream;
    std::stringstream centerStream;
    std::stringstream rightStream;

    bool leftIsUpd   = false;
    bool centerIsUpd = false;
    bool rightIsUpd  = false;

    uint timer = 0;
    while (running) {
        // Обновление модулей
        for (auto module : all) {
            if (timer % module->GetInterval() == 0 || module->isNeedUpdNow == true) {
                module->Update();
                switch (module->GetPosition()) {
                    case Modules::Position::Left   :  leftIsUpd   = true;     break;
                    case Modules::Position::Center :  centerIsUpd = true;     break;
                    case Modules::Position::Right  :  rightIsUpd  = true;     break;
                    default: std::cerr << "Unknown module with id " << module->GetId() << " position" << std::endl; break;
                }
                module->isNeedUpdNow = false;
            }
        }

        // Вывод
        #define SET_MODULES_STREAM(MODULES, STREAM) { int i = 0; STREAM.str(""); STREAM.clear(); for (auto module : MODULES) { STREAM << module->GetOut(); if (i < MODULES.size() - 1) { STREAM << separator; } i++; } STREAM << std::flush; }
        if (leftIsUpd)      { SET_MODULES_STREAM(left, leftStream);     leftIsUpd = false;   }
        if (centerIsUpd)    { SET_MODULES_STREAM(center, centerStream); centerIsUpd = false; }
        if (rightIsUpd)     { SET_MODULES_STREAM(right, rightStream);   rightIsUpd = false;  }

        std::cout << "%{l}" << leftStream.str() << "%{c}" << centerStream.str() << "%{r}" << rightStream.str() << std::flush;
        usleep(1000000); // одна секудна
        timer++;
    }

    return 0;
}

//
// Поток ввода
//

int InputThread(std::atomic<bool>& running, std::vector<std::shared_ptr<Modules::Module>> &modules) {
    EventHandler::Initialize(modules);

    std::ifstream fifo("/tmp/lemonbar_fifo");
    std::string line;

    while (std::getline(fifo, line)) {
        EventHandler::Process(line);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    std::vector<std::shared_ptr<Modules::Module>> left;
    std::vector<std::shared_ptr<Modules::Module>> center;
    std::vector<std::shared_ptr<Modules::Module>> right;
    std::vector<std::shared_ptr<Modules::Module>> all;

    //
    // Загрузка, инициализация модулей
    //

    try {
        ModulesLoader::Load(
            "/home/tigran/Projects/Code/lemonbar/output/modules/"
            , left
            , center
            , right
            , all);
    }
    catch (std::runtime_error &err) {
        std::cout << "Ошибка загрузчика модулей:\n" << err.what() << std::endl;
    }
    

    for (auto module : all) {
        module->Start();
    }

    //
    // Запуск потоков
    //

    std::atomic<bool> running = true;

    // Поток рендера
    std::thread graphics(
        RenderThread
        , std::ref(running)
        , std::ref(left)
        , std::ref(center)
        , std::ref(right)
        , std::ref(all)
    );

    // Поток ввода
    InputThread(std::ref(running), all);

    running = false;
    graphics.join();

    //
    // Отчистка памяти
    //

    // Освобождение памяти от модулей
    #define FREE_MODULES(MODULE) for (std::shared_ptr<Modules::Module> item : MODULE) { item.reset(); }

    FREE_MODULES(all)
    FREE_MODULES(left)
    FREE_MODULES(center)
    FREE_MODULES(right)
}
