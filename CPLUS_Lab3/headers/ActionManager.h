#pragma once

#include <functional> 
#include <map>        
#include <string>    

// Класс ActionManager связывает строковые имена действий
// из .gleb-файла с реальными C++-функциями.
class ActionManager {
private:
    // Таблица действий.
    // Ключ — имя действия, например "go_back" (std::map — это ассоциативный контейнер: он хранит пары «ключ — значение»).
    // Значение — функция, которая должна выполниться (std::function<void()> — это объект, который хранит функцию без параметров и без возвращаемого значения).
    std::map<std::string, std::function<void()>> actions;

public:
    // Регистрирует новое действие.
    void registerAction(const std::string& name, std::function<void()> action);

    // Выполняет действие по имени.
    // Возвращает true, если действие найдено и выполнено.
    bool execute(const std::string& name) const;

    // Проверяет, есть ли действие с таким именем.
    bool contains(const std::string& name) const;
};