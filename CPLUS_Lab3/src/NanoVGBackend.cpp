#include <glad/gl.h>

#include <nanovg.h>

// Этот файл подключает реализацию OpenGL 3-бэкенда NanoVG.
//
// NanoVG состоит из основной части, которая находится в nanovg.c,
// и графического бэкенда, который связывает NanoVG с конкретным API отрисовки.
// В нашем случае используется OpenGL 3, поэтому нужен файл nanovg_gl.h.
//
// Важно: NANOVG_GL3_IMPLEMENTATION должен быть определён ровно в одном .cpp-файле.
// Если определить его в нескольких файлах, компоновщик получит несколько одинаковых
// реализаций функций nvgCreateGL3, nvgDeleteGL3 и других функций бэкенда.

#define NANOVG_GL3_IMPLEMENTATION

// Этот макрос запрещает NanoVG подключать OpenGL-заголовки самостоятельно.
// Мы уже используем GLAD как загрузчик функций OpenGL, поэтому OpenGL-типы и функции
// должны приходить из glad/gl.h, а не из стандартных системных заголовков OpenGL.

#define NANOVG_GL_NO_HEADER

// После объявления макросов подключается OpenGL-бэкенд NanoVG.
// Благодаря NANOVG_GL3_IMPLEMENTATION здесь создаются реализации функций:
//
// nvgCreateGL3(...)
// nvgDeleteGL3(...)
// nvglCreateImageFromHandleGL3(...)
// nvglImageHandleGL3(...)
//
// Остальные файлы проекта могут просто подключать nanovg.h и nanovg_gl.h
// без NANOVG_GL3_IMPLEMENTATION.

#include <nanovg_gl.h>