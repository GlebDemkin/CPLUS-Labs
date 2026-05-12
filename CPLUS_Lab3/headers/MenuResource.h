#pragma once

#include <map>    // std::map
#include <memory> // std::unique_ptr
#include <string> // std::string

#include "DialogWindow.h"
#include "MenuItem.h"

// Структура MenuResource хранит результат чтения .gleb-файла.
// Через неё MenuParser передаёт главное меню и диалоговые окна в Application.
struct MenuResource {
    std::unique_ptr<MenuItem> rootMenu; // корневое меню

    // Диалоговые окна хранятся по строковому id.
    // Например: "about_dialog" -> объект DialogWindow.
    std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;
};
