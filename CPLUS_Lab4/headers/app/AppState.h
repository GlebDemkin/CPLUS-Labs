#pragma once

// Общее состояние приложения: меню, игра, пауза, экран проигрыша или победы.
enum class AppState {
    MainMenu,
    Playing,
    Paused,
    GameOver,
    Win
};
