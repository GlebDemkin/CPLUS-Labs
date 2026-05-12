# CPLUS_Lab3

## Назначение лабораторной работы

`CPLUS_Lab3` — это графическое приложение на C++20, реализующее систему меню с подменю и диалоговыми окнами.

Главная особенность программы заключается в том, что структура меню описывается не напрямую в C++-коде, а во внешнем ресурсном файле:

```txt
resources/menu.gleb
```

Программа при запуске читает этот файл, разбирает его содержимое, создаёт внутренние объекты меню и диалогов, а затем отображает их в окне приложения.

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
Application
        ↓
GLFW + OpenGL + NanoVG
        ↓
графическое окно с меню
```

Такой подход позволяет менять пункты меню, подменю, диалоговые окна, размеры и цвета элементов через текстовый файл, не переписывая основную логику программы.

## Что умеет программа

Программа создаёт графическое окно и отображает главное меню. Пользователь может перемещаться по пунктам меню с клавиатуры, открывать подменю, возвращаться назад, открывать диалоговые окна, закрывать меню и завершать приложение.

Поддерживаются следующие возможности:

```txt
отображение главного меню;
отображение вложенных подменю;
перемещение по пунктам меню;
выбор текущего пункта;
выполнение действия, заданного в menu.gleb;
открытие диалоговых окон;
перемещение по кнопкам диалога;
закрытие диалоговых окон;
настройка размеров пунктов меню;
настройка размеров диалоговых окон;
настройка размеров кнопок диалога;
настройка цветов пунктов меню;
чтение структуры интерфейса из внешнего файла.
```

## Управление

Управление выполняется с клавиатуры.

```txt
Стрелка вверх       выбрать предыдущий пункт
Стрелка вниз        выбрать следующий пункт
Enter               выбрать текущий пункт
Delete              вернуться назад или закрыть диалог
Backspace           вернуться назад или закрыть диалог
Escape              закрыть меню или приложение
M                   снова открыть меню, если оно было закрыто
```

Если открыто обычное меню, клавиши вверх и вниз меняют выбранный пункт. Если открыт диалог, эти же клавиши переключают выбранную кнопку диалога.

Клавиша `Enter` выполняет действие выбранного пункта меню или выбранной кнопки диалога.

## Используемые технологии

В проекте используются:

```txt
C++20
CMake
Ninja
MSVC
GLFW
GLAD
OpenGL
NanoVG
```

### C++20

C++ используется для всей логики программы: хранения меню, чтения ресурсного файла, обработки клавиш, управления приложением и вызова графических библиотек.

В проекте активно используются стандартные типы C++:

```cpp
std::string
std::vector
std::map
std::unique_ptr
std::function
std::ifstream
std::runtime_error
```

### CMake

CMake описывает процесс сборки проекта. Он указывает, какие файлы нужно компилировать, где искать заголовочные файлы, какие библиотеки подключать и какие дополнительные действия выполнить после сборки.

В проекте CMake отвечает за:

```txt
создание цели CPLUS_Lab3;
подключение GLFW;
подключение GLAD;
подключение NanoVG;
подключение OpenGL;
добавление папки headers в пути поиска заголовков;
копирование папки resources рядом с exe-файлом.
```

### Ninja

Ninja выполняет фактическую сборку по файлам, которые подготовил CMake.

Команда сборки:

```cmd
cmake --build --preset ninja-debug --verbose
```

### GLFW

GLFW используется для создания окна и получения событий клавиатуры.

В программе GLFW отвечает за:

```txt
инициализацию оконной системы;
создание окна;
создание OpenGL-контекста;
обработку нажатий клавиш;
проверку закрытия окна;
обмен буферов кадра;
получение текущего размера окна.
```

### OpenGL

OpenGL является графической основой приложения. Программа очищает экран через OpenGL, задаёт область вывода и использует OpenGL как основу для NanoVG.

Примеры используемых функций:

```cpp
glViewport(...);
glClearColor(...);
glClear(...);
```

### GLAD

GLAD загружает адреса функций OpenGL. На Windows современные функции OpenGL нельзя использовать напрямую без предварительной загрузки. Поэтому после создания окна и OpenGL-контекста вызывается:

```cpp
gladLoadGL(glfwGetProcAddress);
```

После этого программа может безопасно использовать функции OpenGL.

### NanoVG

NanoVG используется для 2D-отрисовки интерфейса.

Через NanoVG рисуются:

```txt
фон меню;
прямоугольники пунктов меню;
скруглённые кнопки;
текст пунктов меню;
стрелка у подменю;
диалоговые окна;
затемнение фона при открытом диалоге;
кнопки диалогов.
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

## Описание файлов проекта

### CMakeLists.txt

`CMakeLists.txt` — главный файл сборки проекта.

В нём задаются:

```txt
название проекта;
стандарт C++20;
зависимости;
исходные .cpp-файлы;
пути к заголовочным файлам;
подключаемые библиотеки;
копирование resources после сборки.
```

Основная цель проекта называется:

```cmake
CPLUS_Lab3
```

В неё должны входить исходные файлы:

```cmake
src/main.cpp
src/NanoVGBackend.cpp
src/Application.cpp
src/MenuParser.cpp
src/MenuSystem.cpp
```

Заголовочные файлы из папки `headers` не добавляются в `add_executable`, потому что они подключаются через `#include`.

### CMakePresets.json

`CMakePresets.json` хранит готовые настройки сборки.

Благодаря этому можно не писать длинную команду CMake вручную, а использовать preset:

```cmd
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Preset обычно задаёт:

```txt
генератор Ninja;
папку сборки;
режим Debug;
компилятор и параметры окружения.
```

### resources/menu.gleb

`menu.gleb` — ресурсный файл программы.

В нём описываются:

```txt
главное меню;
пункты меню;
подменю;
диалоговые окна;
кнопки диалогов;
действия;
размеры элементов;
цвета пунктов меню.
```

Этот файл является входными данными для `MenuParser`.

### headers/Application.h

`Application.h` содержит объявление класса `Application`.

`Application` — главный класс программы. Он объединяет все части приложения:

```txt
окно GLFW;
контекст NanoVG;
систему меню;
загрузку ресурсов;
обработку клавиатуры;
главный цикл;
завершение программы.
```

В заголовочном файле обычно объявлены поля и методы класса, а реализация находится в `Application.cpp`.

Основные методы класса:

```cpp
int run();
void initialize();
void mainLoop();
void render();
void shutdown();
void executeAction(const std::string& actionName);
```

### src/Application.cpp

`Application.cpp` содержит реализацию класса `Application`.

Этот файл отвечает за реальное выполнение программы.

Основные задачи:

```txt
запустить приложение;
создать окно;
инициализировать OpenGL;
инициализировать NanoVG;
загрузить menu.gleb;
создать MenuSystem;
передать MenuSystem функцию выполнения действий;
отрисовывать каждый кадр;
передавать клавиши в MenuSystem;
закрывать приложение.
```

#### Application::run

Метод `run()` запускает приложение.

Логика работы:

```txt
попытаться инициализировать приложение;
запустить главный цикл;
если всё прошло успешно — вернуть EXIT_SUCCESS;
если возникла ошибка — вывести сообщение и вернуть EXIT_FAILURE.
```

Упрощённая схема:

```cpp
int Application::run() {
    try {
        initialize();
        mainLoop();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        shutdown();
        return EXIT_FAILURE;
    }
}
```

#### Application::initialize

Метод `initialize()` выполняет полную инициализацию приложения.

Порядок важен:

```txt
initializeGLFW();
createWindow();
initializeGLAD();
initializeNanoVG();
initializeMenu();
```

Сначала нужно создать окно и OpenGL-контекст, и только потом можно загружать функции OpenGL и создавать NanoVG-контекст.

#### Application::initializeGLFW

Метод инициализирует GLFW и задаёт параметры будущего окна.

Он настраивает:

```txt
версию OpenGL;
профиль OpenGL;
stencil buffer для NanoVG.
```

Если GLFW не удалось инициализировать, выбрасывается исключение.

#### Application::createWindow

Метод создаёт окно приложения.

После создания окна он:

```txt
передаёт владение окном в unique_ptr;
делает OpenGL-контекст текущим;
включает вертикальную синхронизацию;
сохраняет указатель this внутри GLFW-окна;
назначает callback для клавиатуры.
```

Сохранение `this` внутри окна нужно потому, что callback GLFW является статической функцией, а программе нужно попасть обратно в объект `Application`.

#### Application::initializeGLAD

Метод загружает функции OpenGL через GLAD.

Без этого современные функции OpenGL могут быть недоступны.

#### Application::initializeNanoVG

Метод создаёт NanoVG-контекст и загружает шрифт.

NanoVG-контекст нужен для отрисовки 2D-интерфейса.

#### Application::initializeMenu

Метод создаёт систему меню и загружает ресурсный файл.

Внутри происходит:

```txt
создаётся MenuSystem;
в MenuSystem передаётся функция executeAction;
создаётся MenuParser;
читается resources/menu.gleb;
корневое меню передаётся в MenuSystem;
диалоги передаются в MenuSystem;
меню открывается.
```

#### Application::executeAction

Метод `executeAction()` выполняет действия, имена которых указаны в `menu.gleb`.

Например, в `menu.gleb` может быть:

```txt
ACTION open_about_dialog
```

Тогда в `Application::executeAction()` должно быть условие:

```cpp
else if (actionName == "open_about_dialog") {
    menuSystem->openDialog("about_dialog");
}
```

То есть `menu.gleb` хранит только имя действия, а реальный C++-код действия находится в `Application.cpp`.

#### Application::mainLoop

Главный цикл работает, пока окно не закрыто.

Каждый проход цикла:

```txt
рисует кадр;
меняет буферы окна;
обрабатывает события GLFW.
```

Типовая схема:

```cpp
while (!glfwWindowShouldClose(window.get())) {
    render();
    glfwSwapBuffers(window.get());
    glfwPollEvents();
}
```

#### Application::render

Метод `render()` рисует один кадр.

В нём выполняется:

```txt
получение размера окна;
получение размера framebuffer;
вычисление pixelRatio;
настройка glViewport;
очистка экрана;
начало кадра NanoVG;
отрисовка MenuSystem;
завершение кадра NanoVG.
```

#### Application::keyCallback

Это статическая функция, которую вызывает GLFW при нажатии клавиши.

Так как функция статическая, она не имеет прямого доступа к полям объекта `Application`. Поэтому из GLFW-окна достаётся сохранённый ранее указатель:

```cpp
Application* application = static_cast<Application*>(
    glfwGetWindowUserPointer(window)
);
```

После этого нажатие передаётся в метод объекта:

```cpp
application->handleKeyPress(key);
```

#### Application::handleKeyPress

Метод обрабатывает нажатую клавишу.

Он делает две основные вещи:

```txt
если нажата M — открывает меню;
остальные клавиши переводит из GLFW-кодов в KeyCode и передаёт в MenuSystem.
```

Коды GLFW не передаются в `MenuSystem` напрямую. Сначала они преобразуются во внутренний тип `KeyCode`.

#### Application::shutdown

Метод освобождает ресурсы приложения.

Порядок освобождения:

```txt
сначала логика меню;
потом NanoVG;
потом окно GLFW;
потом завершение GLFW.
```

### headers/KeyCode.h

`KeyCode.h` содержит перечисление клавиш, понятных системе меню.

Пример:

```cpp
enum class KeyCode {
    Unknown,
    Up,
    Down,
    Enter,
    Delete,
    Escape
};
```

Это нужно, чтобы `MenuSystem` не зависел от GLFW. GLFW используется только в `Application.cpp`, а дальше приложение работает со своими внутренними кодами клавиш.

### headers/Color.h

`Color.h` описывает цвет.

Обычно цвет хранится в формате RGBA:

```cpp
struct Color {
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
};
```

Поля:

```txt
r — красный канал;
g — зелёный канал;
b — синий канал;
a — прозрачность.
```

Значения каналов находятся в диапазоне от `0` до `255`.

В `menu.gleb` цвет задаётся так:

```txt
COLOR 70 70 70 255
```

Также в `Color.h` может находиться оператор чтения цвета из потока:

```cpp
std::istream& operator>>(std::istream& input, Color& color);
```

Он позволяет в парсере писать:

```cpp
Color color;
input >> color;
```

После этого из файла будут прочитаны четыре числа: `r`, `g`, `b`, `a`.

### headers/MenuItem.h

`MenuItem.h` описывает один пункт меню.

Один и тот же класс используется для:

```txt
обычного пункта меню;
пункта с действием;
подменю.
```

Основные данные пункта:

```txt
id — внутренний идентификатор;
text — текст на экране;
actionName — имя действия;
normalColor — обычный цвет;
selectedColor — цвет выбранного пункта;
textColor — цвет текста;
width — ширина пункта;
height — высота пункта;
children — дочерние пункты.
```

Если у пункта есть дочерние элементы, он является подменю.

Например:

```txt
SUBMENU settings "Настройки"
{
    ITEM graphics "Графика"
    {
        ACTION open_graphics_dialog
    }
}
```

Здесь `settings` — это `MenuItem`, у которого есть дочерний пункт `graphics`.

#### hasChildren

Метод проверяет, есть ли у пункта дочерние пункты.

```cpp
bool hasChildren() const;
```

Если `true`, пункт считается подменю.

#### hasAction

Метод проверяет, есть ли у пункта действие.

```cpp
bool hasAction() const;
```

Если `actionName` не пустой, значит при выборе пункта нужно выполнить действие.

#### addChild

Метод добавляет дочерний пункт меню.

```cpp
void addChild(std::unique_ptr<MenuItem> child);
```

Так как используется `std::unique_ptr`, дочерний пункт передаётся во владение родительскому пункту.

#### getChild

Метод возвращает дочерний пункт по индексу.

```cpp
MenuItem& getChild(std::size_t index);
```

Он используется, когда нужно получить выбранный пункт меню.

#### getChildrenCount

Метод возвращает количество дочерних пунктов.

```cpp
std::size_t getChildrenCount() const;
```

Он нужен для навигации вверх и вниз, чтобы не выйти за пределы списка пунктов.

### headers/DialogWindow.h

`DialogWindow.h` содержит два класса:

```txt
DialogButton
DialogWindow
```

#### DialogButton

`DialogButton` описывает кнопку внутри диалогового окна.

Кнопка хранит:

```txt
id — внутренний идентификатор;
text — надпись на кнопке;
actionName — имя действия;
width — ширина кнопки;
height — высота кнопки.
```

Пример из `menu.gleb`:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
    ACTION close_dialog
}
```

После парсинга получится кнопка:

```txt
id = ok
text = ОК
width = 180
height = 48
actionName = close_dialog
```

#### DialogWindow

`DialogWindow` описывает диалоговое окно.

Окно хранит:

```txt
id — внутренний идентификатор;
title — заголовок окна;
text — основной текст окна;
width — ширина окна;
height — высота окна;
buttons — список кнопок.
```

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

### headers/MenuResource.h

`MenuResource.h` содержит структуру, в которую парсер складывает результат чтения `menu.gleb`.

Обычно она содержит:

```cpp
std::unique_ptr<MenuItem> rootMenu;
std::map<std::string, std::unique_ptr<DialogWindow>> dialogs;
```

`rootMenu` — главное меню.

`dialogs` — набор диалоговых окон, доступных по их `id`.

Например, если в файле есть:

```txt
DIALOG about_dialog "О программе"
```

то после парсинга в `dialogs` будет запись с ключом:

```txt
about_dialog
```

### headers/MenuParser.h

`MenuParser.h` содержит объявление класса `MenuParser`.

Парсер отвечает за чтение файла `menu.gleb`.

Основной публичный метод:

```cpp
MenuResource parse(const std::string& filename);
```

Вспомогательные методы обычно закрыты в `private`, потому что они нужны только самому парсеру:

```cpp
std::unique_ptr<MenuItem> readMenuAfterKeyword(std::istream& input);
std::unique_ptr<MenuItem> readSubmenuAfterKeyword(std::istream& input);
std::unique_ptr<MenuItem> readItemAfterKeyword(std::istream& input);
std::unique_ptr<DialogWindow> readDialogAfterKeyword(std::istream& input);
DialogButton readDialogButtonAfterKeyword(std::istream& input);
std::string readToken(std::istream& input);
void expectToken(std::istream& input, const std::string& expectedToken);
void skipSpacesAndComments(std::istream& input);
```

### src/MenuParser.cpp

`MenuParser.cpp` содержит реализацию парсера. Это один из ключевых файлов проекта, потому что он переводит текстовый ресурсный файл в C++-объекты.

Подробная работа парсера описана в отдельном разделе ниже.

### headers/MenuSystem.h

`MenuSystem.h` содержит объявление класса `MenuSystem`.

`MenuSystem` отвечает за состояние меню и его поведение.

Он хранит:

```txt
корневое меню;
диалоговые окна;
открыто ли меню;
какое подменю активно;
какой пункт выбран;
какой диалог открыт;
какая кнопка диалога выбрана;
функцию выполнения действий.
```

Основные методы:

```cpp
void setRootMenu(std::unique_ptr<MenuItem> newRootMenu);
void addDialog(std::unique_ptr<DialogWindow> dialog);
void setActionExecutor(std::function<void(const std::string&)> executor);
void open();
void close();
bool isOpen() const;
void handleKey(KeyCode key);
void draw(NVGcontext* context, float screenWidth, float screenHeight);
void openDialog(const std::string& dialogId);
void closeDialog();
void goBack();
```

### src/MenuSystem.cpp

`MenuSystem.cpp` содержит реализацию логики меню.

Он отвечает за:

```txt
отрисовку меню;
отрисовку диалогов;
переключение выбранного пункта;
открытие подменю;
возврат назад;
выполнение действия;
работу с диалоговыми окнами.
```

#### setRootMenu

Метод принимает главное меню, которое создал `MenuParser`.

Так как меню хранится через `std::unique_ptr`, владение передаётся в `MenuSystem`:

```cpp
rootMenu = std::move(newRootMenu);
```

После этого `MenuSystem` становится владельцем дерева меню.

#### addDialog

Метод добавляет диалоговое окно в коллекцию диалогов.

Диалоги хранятся по `id`, чтобы потом можно было открыть нужный диалог командой:

```cpp
openDialog("about_dialog");
```

#### setActionExecutor

Метод задаёт функцию, которая будет выполнять действия.

`MenuSystem` сам не знает, что именно значит `open_about_dialog` или `exit_application`. Он только получает строку из пункта меню и передаёт её наружу.

Функция задаётся в `Application.cpp`:

```cpp
menuSystem->setActionExecutor([this](const std::string& actionName) {
    executeAction(actionName);
});
```

#### open и close

`open()` открывает меню.

`close()` закрывает меню.

Если меню закрыто, оно не обрабатывает большинство клавиш и не рисуется как активное меню.

#### handleKey

Метод принимает внутренний код клавиши `KeyCode` и обрабатывает его.

Если открыт диалог, клавиши передаются логике диалога.

Если диалог не открыт, клавиши управляют меню.

Примерная логика:

```txt
Up       перейти к предыдущему пункту
Down     перейти к следующему пункту
Enter    выбрать пункт
Delete   вернуться назад
Escape   закрыть меню или диалог
```

#### draw

Метод рисует меню и диалоговые окна.

Он получает:

```cpp
NVGcontext* context
float screenWidth
float screenHeight
```

`context` нужен для вызовов NanoVG. `screenWidth` и `screenHeight` нужны, чтобы центрировать меню и диалоговые окна.

#### openDialog

Метод открывает диалог по его `id`.

Например:

```cpp
openDialog("about_dialog");
```

Если диалог с таким идентификатором есть в `dialogs`, он становится активным.

#### closeDialog

Метод закрывает активное диалоговое окно.

#### goBack

Метод возвращает пользователя на уровень выше в меню.

Если пользователь находится в подменю, он возвращается в родительское меню. Если пользователь находится в главном меню, можно закрыть меню или остаться на месте в зависимости от реализации.

### src/NanoVGBackend.cpp

`NanoVGBackend.cpp` нужен для подключения реализации OpenGL-бэкенда NanoVG.

NanoVG устроен так, что одни заголовочные файлы дают объявления функций, а реализацию нужно один раз подключить в `.cpp` файле.

Этот файл обычно содержит:

```cpp
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>
```

Если его не будет, компилятор может успешно собрать `.cpp` файлы, но линковщик не найдёт функции:

```cpp
nvgCreateGL3
nvgDeleteGL3
```

## Подробное устройство menu.gleb

`menu.gleb` — это простой текстовый язык описания меню.

Он состоит из ключевых слов, идентификаторов, строк, блоков и параметров.

Пример полного фрагмента:

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

### Общие правила файла

Файл читается сверху вниз.

На верхнем уровне разрешены только:

```txt
MENU
DIALOG
```

`MENU` задаёт главное меню. `DIALOG` задаёт отдельное диалоговое окно.

Блоки оформляются фигурными скобками:

```txt
ITEM start "Начать"
{
    ACTION start_game
}
```

Каждый блок должен быть закрыт.

Строки с пробелами нужно писать в кавычках:

```txt
"Главное меню"
"О программе"
"Лабораторная работа: система меню."
```

Однословные значения можно писать без кавычек:

```txt
start
main
open_about_dialog
```

### Комментарии

Парсер поддерживает комментарии двух видов.

Комментарий через `#`:

```txt
# Это комментарий
```

Комментарий через `//`:

```txt
// Это тоже комментарий
```

Комментарии игнорируются парсером и не влияют на программу.

### MENU

`MENU` создаёт корневое меню.

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
    ITEM start "Начать"
    {
        ACTION start_game
    }
}
```

Значения:

```txt
main — внутренний идентификатор меню;
Главное меню — заголовок или текст меню.
```

Внутри `MENU` можно размещать:

```txt
ITEM
SUBMENU
```

### ITEM

`ITEM` создаёт обычный пункт меню.

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
    SIZE 400 58
    ACTION start_game
}
```

Значения:

```txt
start — внутренний идентификатор пункта;
Начать — текст, который увидит пользователь.
```

Внутри `ITEM` можно использовать:

```txt
SIZE
ACTION
COLOR
SELECTED_COLOR
TEXT_COLOR
```

### SUBMENU

`SUBMENU` создаёт пункт меню, внутри которого есть другие пункты.

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
    SIZE 400 58

    ITEM graphics "Графика"
    {
        ACTION open_graphics_dialog
    }
}
```

Внутри `SUBMENU` можно использовать:

```txt
ITEM
SUBMENU
SIZE
ACTION
COLOR
SELECTED_COLOR
TEXT_COLOR
```

Подменю может содержать другое подменю, поэтому меню может быть вложенным.

### DIALOG

`DIALOG` создаёт диалоговое окно.

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
    TEXT "Лабораторная работа: система меню."

    BUTTON ok "ОК"
    {
        ACTION close_dialog
    }
}
```

Значения:

```txt
about_dialog — внутренний идентификатор диалога;
О программе — заголовок окна.
```

Внутри `DIALOG` можно использовать:

```txt
SIZE
TEXT
BUTTON
```

### BUTTON

`BUTTON` создаёт кнопку внутри диалога.

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

Значения:

```txt
ok — внутренний идентификатор кнопки;
ОК — текст на кнопке.
```

Внутри `BUTTON` можно использовать:

```txt
SIZE
ACTION
```

### SIZE

`SIZE` задаёт размер элемента.

Формат:

```txt
SIZE width height
```

Пример:

```txt
SIZE 400 58
```

Значения читаются как числа с плавающей точкой:

```cpp
float width;
float height;
```

`SIZE` можно задавать для:

```txt
ITEM
SUBMENU
DIALOG
BUTTON
```

Для пункта меню `SIZE` задаёт размер кнопки меню.

Для диалога `SIZE` задаёт размер окна.

Для кнопки диалога `SIZE` задаёт размер кнопки.

### ACTION

`ACTION` задаёт имя действия.

Формат:

```txt
ACTION имя_действия
```

Пример:

```txt
ACTION open_about_dialog
```

Само действие не выполняется в `menu.gleb`. Файл только хранит строковое имя. Реальное действие описывается в C++-коде, в методе:

```cpp
Application::executeAction(const std::string& actionName)
```

Пример соответствия:

```txt
menu.gleb:
ACTION open_about_dialog
```

```cpp
Application.cpp:
else if (actionName == "open_about_dialog") {
    menuSystem->openDialog("about_dialog");
}
```

### COLOR

`COLOR` задаёт обычный цвет пункта меню.

Формат:

```txt
COLOR R G B A
```

Пример:

```txt
COLOR 70 70 70 255
```

### SELECTED_COLOR

`SELECTED_COLOR` задаёт цвет выбранного пункта меню.

Формат:

```txt
SELECTED_COLOR R G B A
```

Пример:

```txt
SELECTED_COLOR 110 110 110 255
```

### TEXT_COLOR

`TEXT_COLOR` задаёт цвет текста пункта меню.

Формат:

```txt
TEXT_COLOR R G B A
```

Пример:

```txt
TEXT_COLOR 255 255 255 255
```

### TEXT

`TEXT` задаёт основной текст диалогового окна.

Формат:

```txt
TEXT "текст диалога"
```

Пример:

```txt
TEXT "Лабораторная работа: система меню."
```

Если текст содержит пробелы, кавычки обязательны.

## Подробная работа парсера

`MenuParser` читает файл `menu.gleb` и превращает его в объекты C++.

Важно: парсер не рисует меню и не выполняет действия. Его задача — только прочитать текстовый файл и создать структуру данных.

### Что получает парсер на вход

На вход парсер получает путь к файлу:

```cpp
MenuResource resource = parser.parse("resources/menu.gleb");
```

То есть входные данные:

```txt
resources/menu.gleb
```

### Что парсер возвращает

Парсер возвращает объект `MenuResource`.

Он содержит:

```txt
rootMenu — главное меню;
dialogs — набор диалоговых окон.
```

После этого `Application` передаёт эти данные в `MenuSystem`.

### Главный метод parse

Метод:

```cpp
MenuResource MenuParser::parse(const std::string& filename)
```

является входной точкой парсера.

Он выполняет следующие шаги:

```txt
открывает файл;
создаёт пустой MenuResource;
читает токены верхнего уровня;
если встречает MENU — читает главное меню;
если встречает DIALOG — читает диалог;
если встречает неизвестное слово — выдаёт ошибку;
проверяет, что главное меню было найдено;
возвращает заполненный MenuResource.
```

Пример логики:

```cpp
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
        resource.dialogs.emplace(dialog->getId(), std::move(dialog));
    }
    else {
        throw std::runtime_error("Unexpected token in resource file: " + token);
    }
}
```

### Что такое токен

Токен — это отдельная значимая часть файла.

Например строка:

```txt
ITEM start "Начать"
```

для парсера состоит из токенов:

```txt
ITEM
start
Начать
```

Фигурные скобки тоже считаются токенами:

```txt
{
}
```

Строка:

```txt
ITEM start "Начать"
{
```

даёт токены:

```txt
ITEM
start
Начать
{
```

### readToken

Метод:

```cpp
std::string MenuParser::readToken(std::istream& input)
```

читает следующий токен из файла.

Он нужен потому, что обычное чтение через `input >> token` не подходит. В `menu.gleb` нужно отдельно обрабатывать:

```txt
строки в кавычках;
фигурные скобки;
комментарии;
конец файла;
обычные слова.
```

Алгоритм `readToken`:

```txt
пропустить пробелы и комментарии;
проверить состояние потока;
посмотреть следующий символ через peek();
если конец файла — вернуть пустую строку;
если символ { или } — вернуть его как отдельный токен;
если символ " — прочитать строку до закрывающей кавычки;
иначе читать обычное слово до пробела или скобки.
```

### skipSpacesAndComments

Перед чтением каждого токена парсер вызывает:

```cpp
skipSpacesAndComments(input);
```

Этот метод пропускает всё, что не является значимой частью файла:

```txt
пробелы;
табуляции;
переносы строк;
комментарии через #;
комментарии через //.
```

Пробелы пропускаются так:

```cpp
input >> std::ws;
```

`std::ws` пропускает пробельные символы в потоке.

Если следующий символ `#`, парсер пропускает всё до конца строки:

```cpp
input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

Если следующий символ `/`, парсер проверяет, не идёт ли после него второй `/`. Если это `//`, строка тоже считается комментарием.

### input.good

Проверка:

```cpp
if (!input.good()) {
    return "";
}
```

означает: если поток больше не находится в нормальном состоянии, токен читать нельзя.

Поток может быть не в нормальном состоянии, если:

```txt
достигнут конец файла;
произошла ошибка чтения;
файл закрыт или повреждён.
```

В таком случае `readToken` возвращает пустую строку.

### peek

Вызов:

```cpp
const int nextCharacter = input.peek();
```

смотрит на следующий символ в файле, но не забирает его из потока.

Это важно, потому что парсеру сначала нужно понять, что именно он собирается читать:

```txt
обычное слово;
строку в кавычках;
фигурную скобку;
конец файла.
```

Например, если следующий символ `{`, парсер понимает, что нужно вернуть отдельный токен `{`.

### eof

`eof` означает `end of file`, то есть конец файла.

Проверка:

```cpp
if (nextCharacter == std::char_traits<char>::eof()) {
    return "";
}
```

означает: если впереди больше нет символов, возвращаем пустую строку.

В `parse()` пустая строка используется как признак завершения файла.

### Чтение фигурных скобок

Если следующий символ — `{` или `}`, парсер читает его как отдельный токен.

```cpp
if (nextCharacter == '{' || nextCharacter == '}') {
    char character = '\0';
    input.get(character);
    return std::string(1, character);
}
```

Фигурные скобки нужны для обозначения блоков.

Пример:

```txt
ITEM start "Начать"
{
    ACTION start_game
}
```

Здесь `{` означает начало блока пункта, а `}` — конец блока.

### Чтение строк в кавычках

Если следующий символ — кавычка, парсер читает строку целиком.

Пример:

```txt
"Главное меню"
```

возвращается как:

```txt
Главное меню
```

Кавычки в результат не входят.

Если закрывающая кавычка не найдена, выбрасывается ошибка:

```txt
Unclosed quoted string in resource file
```

Это защищает программу от неправильного формата файла.

### Чтение обычного слова

Обычный токен читается до:

```txt
пробела;
табуляции;
переноса строки;
символа {;
символа }.
```

Например:

```txt
ACTION start_game
```

даёт токены:

```txt
ACTION
start_game
```

Если во время чтения обычного слова парсер встретил `{` или `}`, он возвращает этот символ обратно в поток через:

```cpp
input.unget();
```

Это нужно, чтобы скобка была прочитана отдельно следующим вызовом `readToken()`.

### std::isspace

Проверка:

```cpp
std::isspace(static_cast<unsigned char>(character))
```

определяет, является ли символ пробельным.

Пробельные символы:

```txt
пробел;
табуляция;
перенос строки;
возврат каретки.
```

`static_cast<unsigned char>` нужен для безопасной передачи символа в `std::isspace`.

### expectToken

Метод:

```cpp
void MenuParser::expectToken(std::istream& input, const std::string& expectedToken)
```

читает следующий токен и проверяет, совпадает ли он с ожидаемым.

Например:

```cpp
expectToken(input, "{");
```

означает: после объявления `ITEM`, `MENU`, `SUBMENU`, `DIALOG` или `BUTTON` обязательно должна идти открывающая фигурная скобка.

Если вместо ожидаемого токена найден другой, выбрасывается ошибка.

Например неправильный фрагмент:

```txt
ITEM start "Начать"
    ACTION start_game
}
```

Здесь после `"Начать"` нет `{`, поэтому парсер выдаст ошибку.

### readMenuAfterKeyword

Метод:

```cpp
std::unique_ptr<MenuItem> readMenuAfterKeyword(std::istream& input)
```

читает главное меню после того, как слово `MENU` уже было прочитано.

Например файл содержит:

```txt
MENU main "Главное меню"
{
    ITEM start "Начать"
    {
        ACTION start_game
    }
}
```

Когда вызывается `readMenuAfterKeyword`, слово `MENU` уже прочитано. Поэтому метод читает:

```txt
main
Главное меню
{
```

Затем внутри блока он читает `ITEM` и `SUBMENU`.

Результатом является `std::unique_ptr<MenuItem>`, который указывает на корневой пункт меню.

### readSubmenuAfterKeyword

Метод:

```cpp
std::unique_ptr<MenuItem> readSubmenuAfterKeyword(std::istream& input)
```

читает подменю после слова `SUBMENU`.

Подменю тоже является `MenuItem`, но у него есть дочерние пункты.

Внутри `SUBMENU` могут быть:

```txt
ITEM
SUBMENU
SIZE
ACTION
COLOR
SELECTED_COLOR
TEXT_COLOR
```

Если внутри подменю встречается `ITEM`, создаётся обычный дочерний пункт.

Если встречается `SUBMENU`, создаётся вложенное подменю.

### readItemAfterKeyword

Метод:

```cpp
std::unique_ptr<MenuItem> readItemAfterKeyword(std::istream& input)
```

читает обычный пункт меню после слова `ITEM`.

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

Парсер создаёт `MenuItem` и заполняет его поля:

```txt
id = start
text = Начать
width = 400
height = 58
actionName = start_game
normalColor = 60 110 220 255
selectedColor = 90 150 255 255
textColor = 255 255 255 255
```

### readDialogAfterKeyword

Метод:

```cpp
std::unique_ptr<DialogWindow> readDialogAfterKeyword(std::istream& input)
```

читает диалоговое окно после слова `DIALOG`.

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

Парсер создаёт `DialogWindow` и заполняет:

```txt
id = about_dialog
title = О программе
width = 700
height = 330
text = Лабораторная работа: система меню.
buttons = список кнопок
```

### readDialogButtonAfterKeyword

Метод:

```cpp
DialogButton readDialogButtonAfterKeyword(std::istream& input)
```

читает кнопку диалогового окна после слова `BUTTON`.

Пример:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
    ACTION close_dialog
}
```

Парсер создаёт `DialogButton` и заполняет:

```txt
id = ok
text = ОК
width = 180
height = 48
actionName = close_dialog
```

### Как работают setId и setText в парсере

В коде парсера встречаются строки:

```cpp
button.setId(readToken(input));
button.setText(readToken(input));
```

Рассмотрим фрагмент:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
    ACTION close_dialog
}
```

Когда слово `BUTTON` уже прочитано, следующий токен — `ok`.

Поэтому:

```cpp
button.setId(readToken(input));
```

превращается в:

```cpp
button.setId("ok");
```

Следующий токен — `ОК`.

Поэтому:

```cpp
button.setText(readToken(input));
```

превращается в:

```cpp
button.setText("ОК");
```

`setId` записывает внутренний идентификатор, а `setText` записывает текст, который будет показан пользователю.

То же самое используется для `MENU`, `ITEM`, `SUBMENU` и `DIALOG`.

### Как парсер понимает конец блока

В каждом методе чтения блока используется цикл:

```cpp
while (true) {
    std::string token = readToken(input);

    if (token == "}") {
        break;
    }

    ...
}
```

Цикл продолжается, пока не встретится закрывающая фигурная скобка.

Например:

```txt
ITEM start "Начать"
{
    ACTION start_game
}
```

Когда парсер прочитает `}`, он завершит чтение этого `ITEM` и вернёт готовый объект.

### Как обрабатываются ошибки формата

Если парсер встречает неизвестное ключевое слово, он выбрасывает ошибку.

Например неправильный файл:

```txt
ITEM start "Начать"
{
    WRONG_TOKEN something
}
```

Внутри `ITEM` слово `WRONG_TOKEN` не разрешено, поэтому будет ошибка:

```txt
Unexpected token inside ITEM: WRONG_TOKEN
```

Если забыть закрыть строку кавычкой:

```txt
TEXT "Незакрытая строка
```

будет ошибка:

```txt
Unclosed quoted string in resource file
```

Если забыть блок `{`, сработает `expectToken`.

Такая проверка помогает быстро найти ошибку в `menu.gleb`.

### Как данные проходят от файла до экрана

Рассмотрим пример:

```txt
ITEM start "Начать"
{
    SIZE 400 58
    ACTION start_game
}
```

Парсер создаёт объект `MenuItem`:

```txt
id = start
text = Начать
width = 400
height = 58
actionName = start_game
```

Затем этот объект попадает в дерево меню.

`MenuSystem` берёт этот объект и рисует кнопку меню с текстом `Начать`.

Если пользователь выбирает этот пункт и нажимает `Enter`, `MenuSystem` берёт строку:

```txt
start_game
```

и передаёт её в `Application::executeAction()`.

Там уже выполняется реальное действие.

## Как собирается проект

Перед сборкой нужно открыть командную строку и настроить окружение Visual Studio под x64.

Полная последовательность:

```cmd
cd /d D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Команда:

```cmd
cmake --preset ninja-debug
```

создаёт файлы сборки.

Команда:

```cmd
cmake --build --preset ninja-debug --verbose
```

компилирует и линкует программу.

## Полная пересборка

Если появляются странные ошибки сборки, лучше удалить папку `out` и собрать проект заново.

```cmd
cd /d D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
rmdir /s /q out
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

Полная пересборка полезна, если:

```txt
изменялся CMakeLists.txt;
переименовывались файлы;
перемещались заголовки;
менялась архитектура x86/x64;
CMake сохранил старую конфигурацию.
```

## Как запускать программу

После успешной сборки исполняемый файл находится здесь:

```txt
out/build/ninja-debug/CPLUS_Lab3.exe
```

Запуск из корня проекта:

```cmd
out\build\ninja-debug\CPLUS_Lab3.exe
```

Полный пример:

```cmd
D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3> out\build\ninja-debug\CPLUS_Lab3.exe
```

## Где должен лежать menu.gleb при запуске

Программа читает файл:

```txt
resources/menu.gleb
```

При запуске из корня проекта это путь:

```txt
CPLUS_Lab3/resources/menu.gleb
```

После сборки CMake должен скопировать папку `resources` рядом с `.exe`:

```txt
out/build/ninja-debug/resources/menu.gleb
```

Проверить наличие файла можно командой:

```cmd
dir out\build\ninja-debug\resources
```

Если файла нет, нужно проверить команду копирования ресурсов в `CMakeLists.txt`.

## Пример CMakeLists.txt

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

## Как добавить новый пункт меню

Чтобы добавить новый пункт меню, нужно изменить `resources/menu.gleb`.

Пример:

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

Если пункт должен открывать диалог, нужно добавить сам диалог:

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

После этого нужно добавить обработку действия в `Application::executeAction()`:

```cpp
else if (actionName == "open_help_dialog") {
    menuSystem->openDialog("help_dialog");
}
```

## Как добавить новое подменю

Пример подменю:

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

Если внутри подменю используются новые действия, они должны быть обработаны в `Application::executeAction()`.

## Как добавить новое действие

Действие добавляется в два места.

Сначала в `menu.gleb`:

```txt
ACTION open_statistics_dialog
```

Потом в `Application.cpp`:

```cpp
else if (actionName == "open_statistics_dialog") {
    menuSystem->openDialog("statistics_dialog");
}
```

Если действие не обработать в `Application::executeAction()`, программа не будет знать, что делать с этой строкой.

## Как изменить размеры элементов

Размеры задаются через `SIZE`.

Для пункта меню:

```txt
ITEM start "Начать"
{
    SIZE 400 58
}
```

Для подменю:

```txt
SUBMENU settings "Настройки"
{
    SIZE 400 58
}
```

Для диалогового окна:

```txt
DIALOG about_dialog "О программе"
{
    SIZE 700 330
}
```

Для кнопки диалога:

```txt
BUTTON ok "ОК"
{
    SIZE 180 48
}
```

## Как изменить цвета пунктов меню

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

Формат цвета:

```txt
R G B A
```

Где:

```txt
R — красный канал;
G — зелёный канал;
B — синий канал;
A — прозрачность.
```

Диапазон каждого значения:

```txt
0..255
```

## Как программа работает при запуске

Последовательность запуска:

```txt
main()
    ↓
создание Application
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

Внутри `initializeMenu()`:

```txt
создаётся MenuSystem;
задаётся функция выполнения действий;
создаётся MenuParser;
читается resources/menu.gleb;
получается MenuResource;
главное меню передаётся в MenuSystem;
диалоги передаются в MenuSystem;
меню открывается.
```

Каждый кадр:

```txt
очищается экран;
начинается кадр NanoVG;
рисуется меню;
если открыт диалог — рисуется диалог;
завершается кадр NanoVG;
GLFW меняет буферы окна;
GLFW обрабатывает события.
```

## Почему используется std::unique_ptr

`std::unique_ptr` означает единственного владельца объекта.

Это удобно для дерева меню.

Меню имеет структуру:

```txt
MENU
 ├── ITEM
 ├── SUBMENU
 │    ├── ITEM
 │    └── ITEM
 └── ITEM
```

Каждый дочерний пункт принадлежит только одному родителю. Поэтому `std::unique_ptr` хорошо подходит для хранения дочерних пунктов:

```cpp
std::vector<std::unique_ptr<MenuItem>> children;
```

Преимущества:

```txt
объекты удаляются автоматически;
невозможно случайно создать двух владельцев одного пункта;
структура дерева становится понятнее;
не нужно вручную вызывать delete.
```

Для передачи владения используется `std::move`:

```cpp
children.push_back(std::move(child));
```

После этого дочерний пункт принадлежит вектору `children`.

## Частые ошибки и решения

### Программа не находит menu.gleb

Нужно проверить, существует ли файл:

```cmd
dir resources
```

И есть ли копия рядом с exe:

```cmd
dir out\build\ninja-debug\resources
```

Если папка не копируется, нужно проверить `add_custom_command` в `CMakeLists.txt`.

### Ошибка Cannot open include file

Если компилятор не видит заголовочный файл, нужно проверить:

```txt
лежит ли файл в headers;
правильно ли написано имя файла;
добавлена ли папка headers в target_include_directories.
```

Проверка файла в PowerShell:

```powershell
Test-Path .\headers\KeyCode.h
```

Ожидаемый результат:

```txt
True
```

### Ошибки x86 против x64

Если в выводе есть:

```txt
тип компьютера библиотеки "x86" конфликтует с типом целевого компьютера "x64"
```

значит среда сборки настроена неправильно.

Нужно открыть `cmd` и выполнить:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
```

После этого лучше удалить `out` и собрать заново.

### Ошибка No module named jinja2

GLAD использует Python-модуль `jinja2`.

Установка:

```cmd
python -m pip install --user jinja2
```

После установки нужно снова запустить сборку.

### Ошибка Unexpected token

Если программа пишет:

```txt
Unexpected token inside ITEM
```

или похожее сообщение, значит в `menu.gleb` внутри блока написано ключевое слово, которое парсер там не ожидает.

Нужно проверить, разрешено ли это слово внутри конкретного блока.

Например, внутри `BUTTON` разрешены только:

```txt
SIZE
ACTION
```

А внутри `DIALOG` разрешены:

```txt
SIZE
TEXT
BUTTON
```

### Ошибка Unclosed quoted string

Эта ошибка означает, что в `menu.gleb` открыли кавычку, но не закрыли её.

Неправильно:

```txt
TEXT "Описание диалога
```

Правильно:

```txt
TEXT "Описание диалога"
```

## Итог

`CPLUS_Lab3` — это лабораторная работа, в которой реализована графическая система меню на C++.

В проекте разделены:

```txt
описание интерфейса — resources/menu.gleb;
чтение интерфейса — MenuParser;
хранение меню и диалогов — MenuItem, DialogWindow, MenuResource;
логика поведения меню — MenuSystem;
управление приложением — Application;
отрисовка — NanoVG;
окно и клавиатура — GLFW;
графическая основа — OpenGL.
```

Главная идея программы — внешний ресурсный файл. Благодаря `menu.gleb` структуру меню можно менять без изменения основной логики приложения.
