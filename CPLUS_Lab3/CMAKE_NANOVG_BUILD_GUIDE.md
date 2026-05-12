# CMAKE_NANOVG_BUILD_GUIDE.md

Этот файл нужен для того, чтобы в будущем быстро вспомнить, как собирать проект `CPLUS_Lab3` через CMake, GLFW, GLAD, OpenGL и NanoVG, а также как исправлять ошибку, которая возникла при первой настройке проекта.

## 1. Что делает CMake в этом проекте

В этом проекте CMake отвечает за сборку всей программы.

Он:

1. читает `CMakeLists.txt`;
2. скачивает внешние зависимости через `FetchContent`;
3. подключает GLFW, GLAD, NanoVG и OpenGL;
4. создаёт правила сборки для Ninja;
5. собирает исполняемый файл `CPLUS_Lab3.exe`.

Сама программа пишется на C++, а CMake только описывает, как её собрать.

## 2. Используемые библиотеки

В проекте используются:

- GLFW — создаёт окно, OpenGL-контекст и обрабатывает ввод;
- GLAD — загружает функции OpenGL;
- OpenGL — графический API;
- NanoVG — библиотека для 2D-отрисовки поверх OpenGL.

NanoVG нужен для рисования интерфейса: кнопок, текста, меню, выделения выбранного пункта и диалоговых окон.

## 3. Правильная структура проекта

Рекомендуемая структура проекта:

```txt
CPLUS_Lab3/
│
├── CMakeLists.txt
├── CMakePresets.json
│
├── src/
│   ├── main.cpp
│   └── NanoVGBackend.cpp
│
└── resources/
    └── menu.gleb
```

Папка `out/` не создаётся вручную. Её создаёт CMake.

## 4. Как правильно открывать проект в Visual Studio

Проект нужно открывать не через `.sln`, а как папку:

```txt
Visual Studio → Файл → Открыть → Папка → CPLUS_Lab3
```

Если открыть старый `.sln` или `.vcxproj`, CMake может не использоваться.

## 5. Как правильно запускать CMake из терминала

Перед сборкой нужно открыть x64-окружение Visual Studio.

Вариант через `cmd`:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
```

После этого нужно перейти в папку проекта:

```cmd
cd /d "D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3"
```

Затем выполнить настройку проекта:

```cmd
cmake --preset ninja-debug
```

После успешной настройки выполнить сборку:

```cmd
cmake --build --preset ninja-debug --verbose
```

После успешной сборки запустить программу:

```cmd
out\build\ninja-debug\CPLUS_Lab3.exe
```

## 6. Почему важно использовать x64-окружение

При первой попытке сборки возникла ошибка:

```txt
warning LNK4272: тип компьютера библиотеки "x86" конфликтует с типом целевого компьютера "x64"
```

Это означало, что компилятор запускался как x64, но при линковке подтягивались x86-библиотеки.

Из-за этого CMake не мог даже проверить обычный C-компилятор.

Правильное решение:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
```

После этого обязательно удалить старую папку сборки:

```cmd
rmdir /s /q out
```

И заново выполнить:

```cmd
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
```

## 7. Почему нужно удалять папку out

CMake сохраняет результаты настройки в папке:

```txt
out/build/ninja-debug
```

Если первая настройка прошла с неправильным окружением, CMake может запомнить неправильные пути к компилятору и библиотекам.

Поэтому при странных ошибках нужно удалить `out`:

```cmd
rmdir /s /q out
```

или в PowerShell:

```powershell
Remove-Item "out" -Recurse -Force -ErrorAction SilentlyContinue
```

После этого проект нужно настроить заново.

## 8. Ошибка с jinja2 при сборке GLAD

Во время сборки возникла ошибка:

```txt
ModuleNotFoundError: No module named 'jinja2'
```

Причина: GLAD генерирует файлы `glad/gl.h` и `gl.c` с помощью Python. Для генерации ему нужен модуль `jinja2`.

Решение:

```cmd
python -m pip install --user jinja2
```

Если команда `python` не работает:

```cmd
py -3.11 -m pip install --user jinja2
```

После установки `jinja2` можно повторить сборку:

```cmd
cmake --build --preset ninja-debug --verbose
```

## 9. Ошибки вида "не удается открыть glad/gl.h"

Если Visual Studio пишет:

```txt
не удается открыть источник файл "glad/gl.h"
не удается открыть источник файл "GLFW/glfw3.h"
не удается открыть источник файл "nanovg.h"
```

нужно различать две ситуации.

### Ситуация 1. Это ошибка IntelliSense

Visual Studio может показывать ложные ошибки редактора, хотя реальная сборка проходит.

Нужно смотреть не только "Список ошибок", а окно:

```txt
Вид → Вывод → Сборка
```

Или проверить сборку в терминале:

```cmd
cmake --build --preset ninja-debug --verbose
```

Если терминальная сборка прошла — проект собран правильно.

### Ситуация 2. GLAD действительно не сгенерировался

Если сборка падает и в выводе есть ошибка Python или GLAD, нужно смотреть первую реальную ошибку выше по логу.

Например, ошибка `No module named 'jinja2'` исправляется установкой:

```cmd
python -m pip install --user jinja2
```

## 10. Правильный порядок полной пересборки

Если проект начинает вести себя странно, нужно выполнить полную чистую пересборку:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

cd /d "D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3"

rmdir /s /q out

cmake --preset ninja-debug

cmake --build --preset ninja-debug --verbose

out\build\ninja-debug\CPLUS_Lab3.exe
```

## 11. Как понять, что всё работает

Настройка CMake прошла успешно, если в конце есть строки:

```txt
-- Configuring done
-- Generating done
-- Build files have been written to: ...
```

Сборка прошла успешно, если в конце есть строка вида:

```txt
[30/30] ... /out:CPLUS_Lab3.exe ...
```

и после неё нет ошибок.

После запуска должно открыться окно с тестовой отрисовкой NanoVG.

## 12. Что делать при добавлении новых .cpp-файлов

Если в проект добавляется новый файл, например:

```txt
src/Application.cpp
src/MenuSystem.cpp
src/MenuParser.cpp
```

его нужно добавить в `CMakeLists.txt` в список исходников:

```cmake
add_executable(CPLUS_Lab3
    src/main.cpp
    src/NanoVGBackend.cpp
    src/Application.cpp
    src/MenuSystem.cpp
    src/MenuParser.cpp
)
```

Если `.cpp`-файл не добавить в `CMakeLists.txt`, он не будет участвовать в сборке.

## 13. Назначение NanoVGBackend.cpp

Файл:

```txt
src/NanoVGBackend.cpp
```

нужен для подключения реализации OpenGL-бэкенда NanoVG.

В нём должен быть код:

```cpp
#include <glad/gl.h>

#include <nanovg.h>

#define NANOVG_GL3_IMPLEMENTATION
#define NANOVG_GL_NO_HEADER

#include <nanovg_gl.h>
```

Важно: `NANOVG_GL3_IMPLEMENTATION` должен быть только в одном `.cpp`-файле.

Если написать его в нескольких файлах, появятся ошибки линковки из-за повторного определения функций.

## 14. Краткая памятка

Обычный запуск сборки:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cd /d "D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3"
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
out\build\ninja-debug\CPLUS_Lab3.exe
```

Полная пересборка:

```cmd
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cd /d "D:\Учёба\C++\CPLUS-Labs\CPLUS_Lab3"
rmdir /s /q out
cmake --preset ninja-debug
cmake --build --preset ninja-debug --verbose
out\build\ninja-debug\CPLUS_Lab3.exe
```
