#pragma once

#include <istream>
#include <memory>
#include <string>

#include "menu/DialogWindow.h"
#include "menu/MenuItem.h"
#include "menu/MenuResource.h"

// MenuParser читает внешний файл ресурсов меню формата .gleb.
class MenuParser {
public:
    // Загружает корневое меню и диалоги из файла.
    MenuResource parse(const std::string& filename);

private:
    // Читает блок MENU после уже прочитанного слова MENU.
    std::unique_ptr<MenuItem> readMenuAfterKeyword(std::istream& input);

    // Читает блок SUBMENU после уже прочитанного слова SUBMENU.
    std::unique_ptr<MenuItem> readSubmenuAfterKeyword(std::istream& input);

    // Читает блок ITEM после уже прочитанного слова ITEM.
    std::unique_ptr<MenuItem> readItemAfterKeyword(std::istream& input);

    // Читает блок DIALOG после уже прочитанного слова DIALOG.
    std::unique_ptr<DialogWindow> readDialogAfterKeyword(std::istream& input);

    // Читает кнопку внутри диалогового окна.
    DialogButton readDialogButtonAfterKeyword(std::istream& input);

    // Читает один токен с учётом кавычек, скобок и комментариев.
    std::string readToken(std::istream& input);

    // Проверяет, что следующий токен совпадает с ожидаемым.
    void expectToken(std::istream& input, const std::string& expectedToken);

    // Пропускает пробелы и комментарии перед чтением токена.
    void skipSpacesAndComments(std::istream& input);
};
