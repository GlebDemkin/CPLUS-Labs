# CPLUS_Lab3

## Запуск

Из корня проекта:

```cmd
out\build\ninja-debug\CPLUS_Lab3.exe
```

Полный пример:

```cmd
D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3> out\build\ninja-debug\CPLUS_Lab3.exe
```

## Назначение программы

Этот проект реализует графическую систему меню на C++.

Меню описывается не прямо в коде программы, а во внешнем ресурсном файле:

```txt
resources/menu.gleb
```

Программа читает этот файл, строит дерево меню, создаёт диалоговые окна и отображает их в окне приложения.

Общая схема работы:

```txt
resources/menu.gleb
        ↓
MenuParser
        ↓
MenuResource
        ↓
MenuSystem
        ↓
отрисовка через NanoVG
```

Главная идея проекта в том, что структуру меню, цвета и размеры элементов можно менять через ресурсный файл, не переписывая основную логику программы.

## Возможности программы

Программа создаёт окно и показывает главное меню. В меню можно:

```txt
перемещаться по пунктам;
открывать подменю;
возвращаться назад;
вызывать действия;
открывать диалоговые окна;
закрывать меню;
завершать программу.
```

Управление:

```txt
стрелка вверх       выбрать предыдущий пункт
стрелка вниз        выбрать следующий пункт
Enter               выбрать текущий пункт
Delete/Backspace    вернуться назад или закрыть диалог
Escape              закрыть меню или приложение
M                   снова открыть меню, если оно было закрыто
```

## Используемые технологии

В проекте используются:

```txt
C++20
CMake
Ninja
MSVC
GLFW
GLAD
NanoVG
OpenGL
```

### CMake

CMake описывает сборку проекта. Он указывает, какие файлы компилировать, где искать заголовки, какие библиотеки подключать и что делать после сборки.

В нашем проекте CMake нужен для:

```txt
подключения зависимостей;
создания файлов сборки для Ninja;
добавления .cpp файлов в цель CPLUS_Lab3;
подключения папки headers;
копирования папки resources рядом с .exe.
```

### Ninja

Ninja выполняет сборку по файлам, которые создал CMake.

Обычная команда сборки:

```cmd
cmake --build --preset ninja-debug --verbose
```

### GLFW

GLFW создаёт окно и OpenGL-контекст. Также через GLFW мы получаем события клавиатуры.

GLFW отвечает за:

```txt
создание окна;
обработку клавиш;
проверку закрытия окна;
обмен буферов кадра;
получение размера окна.
```

### OpenGL

OpenGL используется как графическая основа. NanoVG рисует интерфейс поверх OpenGL.

### GLAD

GLAD загружает функции OpenGL.

На Windows нельзя просто вызвать все современные функции OpenGL напрямую. Сначала нужно получить их адреса для текущего OpenGL-контекста.

В программе это делается так:

```cpp
gladLoadGL(glfwGetProcAddress);
```

После этого становятся доступны функции OpenGL, например:

```cpp
glViewport
glClearColor
glClear
```

### NanoVG

NanoVG используется для 2D-отрисовки интерфейса.

Через NanoVG рисуются:

```txt
кнопки меню;
скруглённые прямоугольники;
текст;
диалоговые окна;
затемнение фона;
цвета пунктов меню.
```

## Структура проекта

Текущая структура проекта:

```txt
CPLUS_Lab3/
│
├── CMakeLists.txt
├── CMakePresets.json
├── CMAKE_NANOVG_BUILD_GUIDE.md
│
├── resources/
│   └── menu.gleb
│
├── headers/
│   ├── Application.h
│   ├── Color.h
│   ├── DialogWindow.h
│   ├── KeyCode.h
│   ├── MenuItem.h
│   ├── MenuParser.h
│   ├── MenuResource.h
│   └── MenuSystem.h
│
└── src/
    ├── Application.cpp
    ├── main.cpp
    ├── MenuParser.cpp
    ├── MenuSystem.cpp
    └── NanoVGBackend.cpp
```

В текущей версии проекта больше нет файлов:

```txt
headers/ActionManager.h
src/ActionManager.cpp
headers/Rectangle.h
```

`ActionManager` убран, потому что действия теперь выполняются напрямую в `Application::executeAction()`.

`Rectangle` убран, потому что управление мышью не используется. Вместо прямоугольника у элементов хранятся только размеры:

```txt
width
height
```

Координаты элементов вычисляются во время отрисовки.

## Описание файлов

### src/main.cpp

Точка входа в программу.

Файл создаёт объект `Application` и запускает приложение:

```cpp
#include "Application.h"

int main() {
    Application application;
    return application.run();
}
```

### Application.h и Application.cpp

Класс `Application` управляет всем приложением.

Он отвечает за:

```txt
инициализацию GLFW;
создание окна;
инициализацию GLAD;
инициализацию NanoVG;
загрузку меню;
главный цикл программы;
отрисовку кадров;
обработку нажатий клавиш;
выполнение действий из menu.gleb;
освобождение ресурсов.
```

Главный метод:

```cpp
int run();
```

Он запускает приложение и возвращает код завершения.

В текущей версии действия выполняются в методе:

```cpp
void Application::executeAction(const std::string& actionName);
```

Пример:

```cpp
void Application::executeAction(const std::string& actionName) {
    if (actionName == "start_game") {
        menuSystem->openDialog("start_dialog");
    }
    else if (actionName == "close_dialog") {
        menuSystem->closeDialog();
    }
    else if (actionName == "exit_application") {
        closeApplication();
    }
}
```

То есть `ActionManager` больше не нужен.

### MenuItem.h

`MenuItem` описывает один пункт меню.

Один и тот же класс используется для:

```txt
обычного пункта меню;
пункта с действием;
подменю с дочерними пунктами.
```

Внутри хранятся:

```txt
id;
текст пункта;
имя действия;
обычный цвет;
цвет выбранного пункта;
цвет текста;
ширина пункта;
высота пункта;
дочерние пункты.
```

Если у пункта есть дочерние элементы, он считается подменю.

Размер пункта меню задаётся через поля:

```cpp
float width = 360.0f;
float height = 52.0f;
```

В `menu.gleb` размер задаётся так:

```txt
SIZE 400 58
```

Пример:

```txt
ITEM start "Начать"
{
    SIZE 400 58
    ACTION start_game
    COLOR 60 110 220 255
    SELECTED_COLOR 90 150 255 255
    TEXT_COLOR 255 255 255 255
}
```

Для подменю размер тоже можно задавать:

```txt
SUBMENU settings "Настройки"
{
    SIZE 400 58

    ITEM graphics "Графика"
    {
        SIZE 360 54
        ACTION open_graphics_dialog
    }
}
```

### DialogWindow.h

Файл описывает два класса:

```txt
DialogButton
DialogWindow
```

`DialogButton` хранит:

```txt
id;
текст кнопки;
имя действия;
ширину кнопки;
высоту кнопки.
```

`DialogWindow` хранит:

```txt
id;
заголовок;
текст;
ширину окна;
высоту окна;
список кнопок.
```

Размеры окна и кнопок задаются в `menu.gleb` через `SIZE`.

Пример:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
    TEXT "Лабораторная работа: система меню."

    BUTTON ok "ОК"
    {
        SIZE 180 48
        ACTION close_dialog
    }
}
```

Координаты окна и кнопок не хранятся в классах. Они вычисляются во время отрисовки в `MenuSystem.cpp`.

### MenuResource.h

`MenuResource` хранит результат чтения файла ресурсов:

```cpp
std::unique_ptr<MenuItem> rootMenu;
std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;
```

То есть после чтения `menu.gleb` мы получаем:

```txt
главное меню;
набор диалоговых окон.
```

Диалоги хранятся по строковому идентификатору.

Например:

```txt
about_dialog
graphics_dialog
sound_dialog
```

### MenuParser.h и MenuParser.cpp

`MenuParser` читает файл:

```txt
resources/menu.gleb
```

Он понимает ключевые слова:

```txt
MENU
SUBMENU
ITEM
DIALOG
BUTTON
ACTION
COLOR
SELECTED_COLOR
TEXT_COLOR
SIZE
TEXT
```

Парсер поддерживает комментарии:

```txt
# комментарий
// комментарий
```

Строки с пробелами нужно писать в кавычках:

```txt
TEXT "Это текст диалогового окна"
```

`MenuParser` создаёт объекты:

```txt
MenuItem;
DialogWindow;
DialogButton;
MenuResource.
```

После этого `Application` передаёт готовые данные в `MenuSystem`.

### MenuSystem.h и MenuSystem.cpp

`MenuSystem` хранит состояние меню и отвечает за его поведение.

Он отвечает за:

```txt
открытие и закрытие меню;
переход вверх и вниз;
вход в подменю;
возврат назад;
открытие диалогов;
закрытие диалогов;
выполнение действий;
отрисовку меню;
отрисовку диалогов.
```

Внутри используются:

```txt
rootMenu                   корневое меню
dialogs                    диалоговые окна
currentPath                путь к текущему подменю
selectedIndices            выбранный пункт на каждом уровне меню
menuOpen                   открыто ли меню
dialogOpen                 открыто ли диалоговое окно
activeDialogId             id открытого диалога
selectedDialogButtonIndex  выбранная кнопка диалога
actionExecutor             функция выполнения действий
```

`actionExecutor` заменяет старый `ActionManager`.

Он хранит функцию типа:

```cpp
std::function<void(const std::string&)>
```

Эта функция задаётся в `Application.cpp`:

```cpp
menuSystem->setActionExecutor([this](const std::string& actionName) {
    executeAction(actionName);
});
```

Когда пользователь выбирает пункт меню, `MenuSystem` берёт строку действия и передаёт её в `Application::executeAction()`.

### Color.h

`Color` описывает цвет в формате RGBA:

```cpp
struct Color {
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
};
```

Формат цвета в `menu.gleb`:

```txt
COLOR 70 70 70 255
```

Значения каналов находятся в диапазоне:

```txt
0..255
```

Каналы:

```txt
R красный
G зелёный
B синий
A прозрачность
```

В `Color.h` также есть функция ограничения значения канала:

```cpp
inline int clampColorChannel(int value) {
    return std::clamp(value, 0, 255);
}
```

Она нужна, чтобы NanoVG получил корректные значения цвета.

### KeyCode.h

`KeyCode` — это внутреннее перечисление клавиш меню.

Оно нужно, чтобы `MenuSystem` не зависел напрямую от GLFW.

Вместо кодов GLFW внутри меню используются понятные значения:

```cpp
KeyCode::Up
KeyCode::Down
KeyCode::Enter
KeyCode::Delete
KeyCode::Escape
```

В `Application.cpp` есть функция, которая переводит клавиши GLFW во внутренний тип `KeyCode`:

```cpp
static KeyCode convertGLFWKeyToKeyCode(int glfwKey);
```

Благодаря этому `MenuSystem` не знает о GLFW и работает только с собственными кодами клавиш.

### NanoVGBackend.cpp

Этот файл подключает реализацию OpenGL-бэкенда NanoVG.

NanoVG устроен так, что часть реализации нужно один раз скомпилировать в отдельном `.cpp` файле.

Если удалить `NanoVGBackend.cpp`, могут появиться ошибки линковки с функциями:

```cpp
nvgCreateGL3
nvgDeleteGL3
```

Поэтому файл должен оставаться в проекте и должен быть указан в `CMakeLists.txt`.

## Формат menu.gleb

Файл `resources/menu.gleb` описывает меню и диалоги.

### Главное меню

Пример:

```txt
MENU main "Главное меню"
{
    ITEM start "Начать"
    {
        SIZE 400 58
        ACTION start_game
        COLOR 60 110 220 255
        SELECTED_COLOR 90 150 255 255
        TEXT_COLOR 255 255 255 255
    }

    SUBMENU settings "Настройки"
    {
        SIZE 400 58

        ITEM graphics "Графика"
        {
            SIZE 360 54
            ACTION open_graphics_dialog
            COLOR 70 70 70 255
            SELECTED_COLOR 110 110 110 255
            TEXT_COLOR 255 255 255 255
        }
    }
}
```

### Диалоговое окно

Пример:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
    TEXT "Лабораторная работа: система меню."

    BUTTON ok "ОК"
    {
        SIZE 180 48
        ACTION close_dialog
    }
}
```

## Ключевые слова menu.gleb

### MENU

Создаёт главное меню.

Формат:

```txt
MENU id "текст"
{
    ...
}
```

Пример:

```txt
MENU main "Главное меню"
{
    ...
}
```

### SUBMENU

Создаёт подменю.

Формат:

```txt
SUBMENU id "текст"
{
    ...
}
```

Пример:

```txt
SUBMENU settings "Настройки"
{
    ...
}
```

### ITEM

Создаёт обычный пункт меню.

Формат:

```txt
ITEM id "текст"
{
    ...
}
```

Пример:

```txt
ITEM start "Начать"
{
    ACTION start_game
}
```

### DIALOG

Создаёт диалоговое окно.

Формат:

```txt
DIALOG id "заголовок"
{
    ...
}
```

Пример:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
    TEXT "Описание программы."
}
```

### BUTTON

Создаёт кнопку внутри диалогового окна.

Формат:

```txt
BUTTON id "текст"
{
    ...
}
```

Пример:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
    ACTION close_dialog
}
```

### SIZE

Задаёт размер элемента.

Сейчас `SIZE` поддерживается для:

```txt
ITEM
SUBMENU
DIALOG
BUTTON
```

Формат:

```txt
SIZE width height
```

Пример для пункта меню:

```txt
ITEM start "Начать"
{
    SIZE 400 58
    ACTION start_game
}
```

Пример для подменю:

```txt
SUBMENU settings "Настройки"
{
    SIZE 400 58
}
```

Пример для диалогового окна:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
}
```

Пример для кнопки диалога:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
}
```

### ACTION

Задаёт имя действия.

Формат:

```txt
ACTION имя_действия
```

Пример:

```txt
ACTION open_about_dialog
```

Все действия обрабатываются в `Application::executeAction()`.

### COLOR

Задаёт обычный цвет пункта меню.

```txt
COLOR 70 70 70 255
```

### SELECTED_COLOR

Задаёт цвет выбранного пункта меню.

```txt
SELECTED_COLOR 110 110 110 255
```

### TEXT_COLOR

Задаёт цвет текста пункта меню.

```txt
TEXT_COLOR 255 255 255 255
```

### TEXT

Задаёт текст диалогового окна.

```txt
TEXT "Это текст диалогового окна"
```

Если текст содержит пробелы, его нужно писать в кавычках.

## Правило для ACTION

Все действия из `menu.gleb` должны быть обработаны в методе:

```cpp
Application::executeAction(const std::string& actionName)
```

Например, если в `menu.gleb` написано:

```txt
ACTION open_sound_dialog
```

то в `Application.cpp` должно быть:

```cpp
else if (actionName == "open_sound_dialog") {
    menuSystem->openDialog("sound_dialog");
}
```

Если действие не обработано, программа выведет сообщение:

```txt
Unknown action: имя_действия
```

## CMakeLists.txt

В `add_executable` должны быть указаны `.cpp` файлы проекта:

```cmake
add_executable(CPLUS_Lab3
    src/main.cpp
    src/NanoVGBackend.cpp

    src/Application.cpp
    src/MenuParser.cpp
    src/MenuSystem.cpp
)
```

В текущей версии не нужно добавлять:

```cmake
src/ActionManager.cpp
src/MenuItem.cpp
src/Color.cpp
```

`ActionManager.cpp` удалён из проекта.

`MenuItem.cpp` не нужен, потому что `MenuItem` полностью находится в `MenuItem.h`.

`Color.cpp` не нужен, если `Color.h` содержит `inline`-функции.

Папку `headers` нужно добавить в пути подключения:

```cmake
target_include_directories(CPLUS_Lab3 PRIVATE
    "${CMAKE_SOURCE_DIR}/headers"
    "${nanovg_SOURCE_DIR}/src"
)
```

Для копирования ресурсов после сборки желательно добавить:

```cmake
add_custom_command(TARGET CPLUS_Lab3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:CPLUS_Lab3>/resources"
)
```

Это нужно, чтобы `resources/menu.gleb` оказался рядом с `CPLUS_Lab3.exe`.

## Полный пример CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)

project(CPLUS_Lab3 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
    glad_gl_core_33
    STATIC
    API gl:core=3.3
)

FetchContent_Declare(
    nanovg
    GIT_REPOSITORY https://github.com/memononen/nanovg.git
    GIT_TAG master
)

FetchContent_GetProperties(nanovg)

if(NOT nanovg_POPULATED)
    FetchContent_Populate(nanovg)
endif()

add_library(nanovg STATIC
    "${nanovg_SOURCE_DIR}/src/nanovg.c"
)

target_include_directories(nanovg PUBLIC
    "${nanovg_SOURCE_DIR}/src"
)

add_executable(CPLUS_Lab3
    src/main.cpp
    src/NanoVGBackend.cpp

    src/Application.cpp
    src/MenuParser.cpp
    src/MenuSystem.cpp
)

target_include_directories(CPLUS_Lab3 PRIVATE
    "${CMAKE_SOURCE_DIR}/headers"
    "${nanovg_SOURCE_DIR}/src"
)

target_link_libraries(CPLUS_Lab3 PRIVATE
    glfw
    glad_gl_core_33
    nanovg
    opengl32
)

add_custom_command(TARGET CPLUS_Lab3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:CPLUS_Lab3>/resources"
)
```

## Как собирать проект

Лучше собирать проект из обычного `cmd`, предварительно настроив среду Visual Studio под x64.

Полная последовательность:

```cmd
cd /d D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Если нужно полностью пересобрать проект:

```cmd
cd /d D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
rmdir /s /q out
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Полная пересборка полезна, если CMake сохранил неправильные пути, старую конфигурацию или старый список файлов.

## Как запускать программу

После успешной сборки исполняемый файл находится здесь:

```txt
out/build/ninja-debug/CPLUS_Lab3.exe
```

Запуск из корня проекта:

```cmd
out\build\ninja-debug\CPLUS_Lab3.exe
```

Если программа не находит `menu.gleb`, нужно проверить, есть ли папка:

```cmd
dir out\build\ninja-debug\resources
```

Если папки нет, нужно добавить копирование ресурсов в `CMakeLists.txt` или скопировать папку `resources` вручную рядом с `.exe`.

## Как понять, что сборка успешна

Успешная сборка заканчивается без строк:

```txt
FAILED
error
fatal error
```

В конце должно быть примерно так:

```txt
[34/34] ... link.exe ... /out:CPLUS_Lab3.exe ...
```

Если после этого Ninja не пишет ошибку, значит программа собрана.

## Частые ошибки и решения

### Cannot find source file src/ActionManager.cpp

Причина: `ActionManager.cpp` удалён, но всё ещё указан в `CMakeLists.txt`.

Решение: удалить строку:

```cmake
src/ActionManager.cpp
```

из `add_executable`.

### Cannot find source file src/MenuItem.cpp

Причина: в `CMakeLists.txt` указан файл `src/MenuItem.cpp`, но такого файла нет.

Решение: удалить строку:

```cmake
src/MenuItem.cpp
```

из `add_executable`.

`MenuItem` сейчас полностью находится в `MenuItem.h`.

### Cannot open include file: Application.h

Причина: CMake не знает, где лежат заголовки.

Решение: добавить папку `headers`:

```cmake
target_include_directories(CPLUS_Lab3 PRIVATE
    "${CMAKE_SOURCE_DIR}/headers"
    "${nanovg_SOURCE_DIR}/src"
)
```

### Cannot open include file: KeyCode.h

Причина: файл может иметь неправильное имя. Например, визуально `KeyCode.h`, но одна буква набрана русской раскладкой.

Проверка:

```powershell
Test-Path .\headers\KeyCode.h
```

Должно быть:

```txt
True
```

Если вывод `False`, файл нужно переименовать строго в:

```txt
KeyCode.h
```

### Color не определён в MenuItem.h

Причина: в `MenuItem.h` не подключён `Color.h`.

Решение:

```cpp
#include "Color.h"
```

### Ошибка с index в getChildrenCount

Неправильно:

```cpp
std::size_t getChildrenCount() const { return *children.at(index); }
```

Правильно:

```cpp
std::size_t getChildrenCount() const { return children.size(); }
```

### Остались старые Rectangle или ActionManager

Если после изменений появляются ошибки с `Rectangle` или `ActionManager`, нужно проверить, что старые подключения удалены.

Команда для поиска:

```powershell
Select-String -Path .\headers\*.h,.\src\*.cpp -Pattern "Rectangle|ActionManager"
```

В текущей версии программа не должна использовать:

```cpp
#include "Rectangle.h"
#include "ActionManager.h"
```

### x86 конфликтует с x64

Важный признак:

```txt
warning LNK4272: тип компьютера библиотеки "x86" конфликтует с типом целевого компьютера "x64"
```

Причина: проект собирается как x64, но линковщик берёт x86-библиотеки.

Решение:

```cmd
cd /d D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
rmdir /s /q out
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Проверить среду можно так:

```cmd
echo %LIB%
```

В путях должны быть `x64`, например:

```txt
...\Windows Kits\10\lib\10.0.26100.0\um\x64
...\MSVC\14.50...\lib\x64
```

Если там есть `x86`, среда настроена неправильно.

### No module named jinja2

Причина: GLAD использует Python и модуль `jinja2` для генерации файлов.

Решение:

```cmd
python -m pip install --user jinja2
```

После этого нужно снова выполнить сборку.

### Предупреждение FetchContent_Populate deprecated

Пример:

```txt
Calling FetchContent_Populate(nanovg) is deprecated
```

Это предупреждение CMake, а не ошибка. Оно не мешает сборке.

## Как добавить новый пункт меню

Сначала нужно добавить пункт в `resources/menu.gleb`:

```txt
ITEM help "Помощь"
{
    SIZE 400 58
    ACTION open_help_dialog
    COLOR 70 70 70 255
    SELECTED_COLOR 110 110 110 255
    TEXT_COLOR 255 255 255 255
}
```

Потом нужно добавить обработку действия в `Application::executeAction()`:

```cpp
else if (actionName == "open_help_dialog") {
    menuSystem->openDialog("help_dialog");
}
```

Если пункт открывает диалог, нужно добавить сам диалог:

```txt
DIALOG help_dialog "Помощь"
{
    SIZE 650 320
    TEXT "Здесь находится справочная информация."

    BUTTON ok "ОК"
    {
        SIZE 180 48
        ACTION close_dialog
    }
}
```

## Как добавить новое подменю

Пример:

```txt
SUBMENU profile "Профиль"
{
    SIZE 400 58
    COLOR 70 70 70 255
    SELECTED_COLOR 110 110 110 255
    TEXT_COLOR 255 255 255 255

    ITEM user_info "Информация"
    {
        SIZE 360 54
        ACTION open_user_info_dialog
        COLOR 70 70 70 255
        SELECTED_COLOR 110 110 110 255
        TEXT_COLOR 255 255 255 255
    }

    ITEM back "Назад"
    {
        SIZE 360 54
        ACTION go_back
        COLOR 50 50 50 255
        SELECTED_COLOR 90 90 90 255
        TEXT_COLOR 255 255 255 255
    }
}
```

Для нового действия также нужно добавить обработку в `Application::executeAction()`.

## Как изменить размеры

Размеры задаются через `SIZE`.

Формат:

```txt
SIZE width height
```

Пример для пункта меню:

```txt
ITEM start "Начать"
{
    SIZE 400 58
}
```

Пример для кнопки диалога:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
}
```

Пример для окна:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
}
```

## Как изменить цвета

Цвета пунктов меню задаются в `menu.gleb`.

Обычный цвет:

```txt
COLOR 70 70 70 255
```

Цвет выбранного пункта:

```txt
SELECTED_COLOR 110 110 110 255
```

Цвет текста:

```txt
TEXT_COLOR 255 255 255 255
```

Формат:

```txt
R G B A
```

Где:

```txt
R красный канал
G зелёный канал
B синий канал
A прозрачность
```

Диапазон каждого значения:

```txt
0..255
```

## Что происходит при запуске

Последовательность запуска:

```txt
main()
    ↓
Application::run()
    ↓
Application::initialize()
    ↓
initializeGLFW()
    ↓
createWindow()
    ↓
initializeGLAD()
    ↓
initializeNanoVG()
    ↓
initializeMenu()
    ↓
mainLoop()
```

В `initializeMenu()`:

```txt
создаётся MenuSystem;
в MenuSystem передаётся функция executeAction;
читается resources/menu.gleb;
корневое меню передаётся в MenuSystem;
диалоги передаются в MenuSystem;
меню открывается.
```

Каждый кадр:

```txt
очищается экран;
начинается кадр NanoVG;
рисуется меню;
рисуется диалог, если он открыт;
завершается кадр NanoVG;
GLFW меняет буферы окна.
```

## Почему используется unique_ptr

`std::unique_ptr` означает единственного владельца объекта.

Это удобно для дерева меню:

```txt
один пункт принадлежит только одному родителю;
дочерние пункты удаляются автоматически;
объекты нельзя случайно скопировать.
```

Поэтому в `MenuItem` копирование запрещено:

```cpp
MenuItem(const MenuItem&) = delete;
MenuItem& operator=(const MenuItem&) = delete;
```

А перемещение разрешено:

```cpp
MenuItem(MenuItem&&) noexcept = default;
MenuItem& operator=(MenuItem&&) noexcept = default;
```

`std::unique_ptr` также используется для хранения:

```txt
корневого меню;
дочерних пунктов меню;
диалоговых окон;
окна GLFW;
контекста NanoVG;
MenuSystem.
```

## Почему убрали ActionManager

Раньше действия хранились в отдельном классе `ActionManager`.

Это усложняло проект, потому что появлялся дополнительный слой:

```txt
MenuSystem
    ↓
ActionManager
    ↓
Application
```

В текущей версии действия выполняются проще:

```txt
MenuSystem
    ↓
actionExecutor
    ↓
Application::executeAction()
```

Теперь вся логика действий находится в одном месте:

```cpp
Application::executeAction()
```

Так проще понять, какое действие что делает.

## Почему убрали Rectangle

Раньше `Rectangle` хранил:

```txt
x
y
width
height
```

Но в проекте нет управления мышью, поэтому постоянное хранение `x` и `y` не нужно.

Сейчас у элементов хранятся только размеры:

```txt
width
height
```

А координаты вычисляются во время отрисовки.

Например, кнопка меню центрируется так:

```cpp
const float x = (screenWidth - width) / 2.0f;
```

Так код стал проще, а возможность задавать размеры через `menu.gleb` сохранилась.

## Главные правила работы с проектом

1. После изменения `CMakeLists.txt` нужно выполнять:

```cmd
cmake --preset ninja-debug
```

2. После изменения `.cpp` или `.h` обычно достаточно:

```cmd
cmake --build --preset ninja-debug --verbose
```

3. Если появились странные ошибки, нужно удалить `out` и собрать заново:

```cmd
rmdir /s /q out
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

4. Если появились ошибки `x86` против `x64`, нужно заново вызвать `VsDevCmd.bat` с параметрами x64.

5. В `add_executable` нужно добавлять `.cpp`, а не `.h`.

6. `resources/menu.gleb` должен быть доступен во время запуска.

7. Все действия из `menu.gleb` должны быть обработаны в `Application::executeAction()`.

8. В текущей версии не должно быть подключений `ActionManager.h` и `Rectangle.h`.

## Итог

Проект представляет собой графическую систему меню, где:

```txt
C++ отвечает за логику;
GLFW создаёт окно;
GLAD загружает функции OpenGL;
NanoVG рисует интерфейс;
menu.gleb описывает меню, диалоги, цвета и размеры;
CMake собирает проект и подключает зависимости.
```

Главное преимущество такой структуры в том, что меню можно изменять через ресурсный файл, не переписывая основную программу.

Текущая версия стала проще:

```txt
ActionManager удалён;
Rectangle удалён;
unique_ptr оставлен;
размеры меню, подменю, диалогов и кнопок задаются через SIZE;
действия выполняются через Application::executeAction().
```
