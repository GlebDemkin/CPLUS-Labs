#pragma once

#include <map>
#include <memory>
#include <string>

#include "menu/DialogWindow.h"
#include "menu/MenuItem.h"

// Результат чтения одного .gleb-файла меню.
// rootMenu хранит главное дерево меню, dialogs хранит диалоговые окна по id.
struct MenuResource {
    std::unique_ptr<MenuItem> rootMenu;
    std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;
};
