#include "menu/MenuParser.h"

#include <cctype>    // std::isspace
#include <fstream>   // std::ifstream
#include <limits>    // std::numeric_limits
#include <stdexcept> // std::runtime_error
#include <utility>   // std::move

MenuResource MenuParser::parse(const std::string& filename) {
    // Открываем .gleb-файл как поток ввода.
    std::ifstream input(filename);

    if (!input.is_open()) {
        throw std::runtime_error("Failed to open menu resource file: " + filename);
    }

    MenuResource resource;

    // На верхнем уровне разрешены блоки MENU и DIALOG.
    while (true) {
        std::string token = readToken(input);

        if (token.empty()) {
            break;
        }

        if (token == "MENU") {
            resource.rootMenu = readMenuAfterKeyword(input);
        }
        else if (token == "DIALOG") {
            std::unique_ptr<DialogWindow> dialog = readDialogAfterKeyword(input);

            // Сохраняем диалог по его id.
            resource.dialogs.emplace(dialog->getId(), std::move(dialog));
        }
        else {
            throw std::runtime_error("Unexpected token in resource file: " + token);
        }
    }

    if (!resource.rootMenu) {
        throw std::runtime_error("Resource file does not contain MENU block");
    }

    return resource;
}

std::unique_ptr<MenuItem> MenuParser::readMenuAfterKeyword(std::istream& input) {
    std::unique_ptr<MenuItem> menu = std::make_unique<MenuItem>();

    // После MENU читаем id и заголовок меню.
    menu->setId(readToken(input));
    menu->setText(readToken(input));

    expectToken(input, "{");

    // Внутри MENU могут быть ITEM и SUBMENU.
    while (true) {
        std::string token = readToken(input);

        if (token == "}") {
            break;
        }

        if (token == "ITEM") {
            menu->addChild(readItemAfterKeyword(input));
        }
        else if (token == "SUBMENU") {
            menu->addChild(readSubmenuAfterKeyword(input));
        }
        else {
            throw std::runtime_error("Unexpected token inside MENU: " + token);
        }
    }

    return menu;
}

std::unique_ptr<MenuItem> MenuParser::readSubmenuAfterKeyword(std::istream& input) {
    std::unique_ptr<MenuItem> submenu = std::make_unique<MenuItem>();

    // После SUBMENU читаем id и текст подменю.
    submenu->setId(readToken(input));
    submenu->setText(readToken(input));

    expectToken(input, "{");

    // SUBMENU может содержать пункты, вложенные подменю, размер, цвета и действие.
    while (true) {
        std::string token = readToken(input);

        if (token == "}") {
            break;
        }

        if (token == "ITEM") {
            submenu->addChild(readItemAfterKeyword(input));
        }
        else if (token == "SUBMENU") {
            submenu->addChild(readSubmenuAfterKeyword(input));
        }
        else if (token == "SIZE") {
            float width = 0.0f;
            float height = 0.0f;

            input >> width >> height;
            submenu->setSize(width, height);
        }
        else if (token == "ACTION") {
            submenu->setActionName(readToken(input));
        }
        else if (token == "COLOR") {
            Color color;
            input >> color;
            submenu->setNormalColor(color);
        }
        else if (token == "SELECTED_COLOR") {
            Color color;
            input >> color;
            submenu->setSelectedColor(color);
        }
        else if (token == "TEXT_COLOR") {
            Color color;
            input >> color;
            submenu->setTextColor(color);
        }
        else {
            throw std::runtime_error("Unexpected token inside SUBMENU: " + token);
        }
    }

    return submenu;
}

std::unique_ptr<MenuItem> MenuParser::readItemAfterKeyword(std::istream& input) {
    std::unique_ptr<MenuItem> item = std::make_unique<MenuItem>();

    // После ITEM читаем id и текст пункта.
    item->setId(readToken(input));
    item->setText(readToken(input));

    expectToken(input, "{");

    // ITEM может содержать размер, действие и цвета.
    while (true) {
        std::string token = readToken(input);

        if (token == "}") {
            break;
        }

        if (token == "SIZE") {
            float width = 0.0f;
            float height = 0.0f;

            input >> width >> height;
            item->setSize(width, height);
        }
        else if (token == "ACTION") {
            item->setActionName(readToken(input));
        }
        else if (token == "COLOR") {
            Color color;
            input >> color;
            item->setNormalColor(color);
        }
        else if (token == "SELECTED_COLOR") {
            Color color;
            input >> color;
            item->setSelectedColor(color);
        }
        else if (token == "TEXT_COLOR") {
            Color color;
            input >> color;
            item->setTextColor(color);
        }
        else {
            throw std::runtime_error("Unexpected token inside ITEM: " + token);
        }
    }

    return item;
}

std::unique_ptr<DialogWindow> MenuParser::readDialogAfterKeyword(std::istream& input) {
    std::unique_ptr<DialogWindow> dialog = std::make_unique<DialogWindow>();

    // После DIALOG читаем id и заголовок окна.
    dialog->setId(readToken(input));
    dialog->setTitle(readToken(input));

    expectToken(input, "{");

    // DIALOG может содержать SIZE, TEXT и BUTTON.
    while (true) {
        std::string token = readToken(input);

        if (token == "}") {
            break;
        }

        if (token == "SIZE") {
            float width = 0.0f;
            float height = 0.0f;

            input >> width >> height;
            dialog->setSize(width, height);
        }
        else if (token == "TEXT") {
            dialog->setText(readToken(input));
        }
        else if (token == "BUTTON") {
            dialog->addButton(readDialogButtonAfterKeyword(input));
        }
        else {
            throw std::runtime_error("Unexpected token inside DIALOG: " + token);
        }
    }

    return dialog;
}

DialogButton MenuParser::readDialogButtonAfterKeyword(std::istream& input) {
    DialogButton button;

    // После BUTTON читаем id и текст кнопки.
    button.setId(readToken(input));
    button.setText(readToken(input));

    expectToken(input, "{");

    // BUTTON может содержать SIZE и ACTION.
    while (true) {
        std::string token = readToken(input);

        if (token == "}") {
            break;
        }

        if (token == "SIZE") {
            float width = 0.0f;
            float height = 0.0f;

            input >> width >> height;
            button.setSize(width, height);
        }
        else if (token == "ACTION") {
            button.setActionName(readToken(input));
        }
        else {
            throw std::runtime_error("Unexpected token inside BUTTON: " + token);
        }
    }

    return button;
}

std::string MenuParser::readToken(std::istream& input) {
    // Перед чтением пропускаем пробелы, переносы строк и комментарии.
    skipSpacesAndComments(input);

    // Если поток уже в ошибочном состоянии или файл закончился, возвращаем пустую строку.
    if (!input.good()) {
        return "";
    }

    // peek() смотрит на следующий символ, но не забирает его из потока.
    const int nextCharacter = input.peek();

    // eof означает конец файла.
    if (nextCharacter == std::char_traits<char>::eof()) {
        return "";
    }

    // Фигурные скобки считаются отдельными токенами.
    if (nextCharacter == '{' || nextCharacter == '}') {
        char character = '\0';
        input.get(character);
        return std::string(1, character);
    }

    // Строка в кавычках читается целиком как один токен.
    if (nextCharacter == '"') {
        input.get();

        std::string result;
        char character = '\0';

        while (input.get(character)) {
            if (character == '"') {
                return result;
            }

            result += character;
        }

        throw std::runtime_error("Unclosed quoted string in resource file");
    }

    // Обычный токен читается до пробела или фигурной скобки.
    std::string token;
    char character = '\0';

    while (input.get(character)) {
        if (std::isspace(static_cast<unsigned char>(character)) ||
            character == '{' ||
            character == '}') {
            // Если встретили скобку, возвращаем её обратно,
            // чтобы следующий readToken прочитал её отдельным токеном.
            if (character == '{' || character == '}') {
                input.unget();
            }

            break;
        }

        token += character;
    }

    return token;
}

void MenuParser::expectToken(std::istream& input, const std::string& expectedToken) {
    // Читаем следующий токен и проверяем, что он совпадает с ожидаемым.
    const std::string actualToken = readToken(input);

    if (actualToken != expectedToken) {
        throw std::runtime_error(
            "Expected token '" + expectedToken + "', but got '" + actualToken + "'"
        );
    }
}

void MenuParser::skipSpacesAndComments(std::istream& input) {
    while (true) {
        // Пропускаем пробелы, табуляции и переносы строк.
        input >> std::ws;

        if (!input.good()) {
            return;
        }

        // Комментарий формата # comment.
        if (input.peek() == '#') {
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Комментарий формата // comment.
        if (input.peek() == '/') {
            input.get();

            if (input.peek() == '/') {
                input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            // Если это не комментарий, возвращаем символ / обратно в поток.
            input.unget();
        }

        return;
    }
}
