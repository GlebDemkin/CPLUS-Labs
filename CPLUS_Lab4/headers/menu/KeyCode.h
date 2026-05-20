#pragma once

// Внутреннее представление клавиш для MenuSystem.
// Благодаря этому MenuSystem не зависит напрямую от GLFW.
enum class KeyCode {
    Unknown,
    Up,
    Down,
    Enter,
    Delete,
    Escape
};
