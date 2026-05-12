#pragma once

#include <istream> // std::istream
#include <memory>  // std::unique_ptr
#include <string>  // std::string

#include "DialogWindow.h"
#include "MenuItem.h"
#include "MenuResource.h"

// Класс MenuParser читает внешний файл ресурсов .gleb.
class MenuParser {
public:
    // Читает файл и возвращает главное меню вместе с диалогами.
    MenuResource parse(const std::string& filename);

private:
    // Чтение блоков меню.
    std::unique_ptr<MenuItem> readMenuAfterKeyword(std::istream& input);
    std::unique_ptr<MenuItem> readSubmenuAfterKeyword(std::istream& input);
    std::unique_ptr<MenuItem> readItemAfterKeyword(std::istream& input);

    // Чтение блоков диалоговых окон.
    std::unique_ptr<DialogWindow> readDialogAfterKeyword(std::istream& input);
    DialogButton readDialogButtonAfterKeyword(std::istream& input);

    // Служебные функции парсера.
    std::string readToken(std::istream& input);
    void expectToken(std::istream& input, const std::string& expectedToken);
    void skipSpacesAndComments(std::istream& input);
};
