#include "game/GameWorld.h"

#include <algorithm>  // std::remove_if для удаления неактивных объектов
#include <array>      // std::array для набора направлений в BFS
#include <queue>      // std::queue для поиска в ширину
#include <stdexcept>  // std::runtime_error для ошибок загрузки и координат
#include <utility>    // std::move для передачи владения

#include "game/LevelLoader.h" // загрузка уровней из .gleb-файлов

#include "objects/Emerald.h"  // изумруд
#include "objects/Fireball.h" // огненный шар
#include "objects/GoldBag.h"  // мешок золота
#include "objects/GoldPile.h" // кучка золота
#include "objects/Nobbin.h"   // враг
#include "objects/Player.h"   // игрок


// Начинает новую игру с первого уровня.
void GameWorld::startNewGame() {
    currentLevelIndex = 0;
    lives = 3;
    money = 0;
    state = GameState::Playing;
    levelCompleteTimer = 0.0f;

    // true означает полный сброс жизней и денег.
    loadCurrentLevel(true);
}


// Полностью перезапускает текущий уровень.
void GameWorld::restartCurrentLevel() {
    state = GameState::Playing;
    levelCompleteTimer = 0.0f;

    // false означает, что это не новая игра с первого уровня.
    loadCurrentLevel(false);
}


// Обновляет игровой мир за один кадр.
void GameWorld::update(float deltaTime) {
    // Если уровень пройден, двигается только таймер перехода.
    if (state == GameState::LevelCompleted) {
        updateLevelCompleteTransition(deltaTime);
        inputState.clearOneFrameActions();
        return;
    }

    // Если игра не в рабочем состоянии, объекты не обновляются.
    if (state != GameState::Playing) {
        inputState.clearOneFrameActions();
        return;
    }

    // Во время смерти игрока игра замирает, обновляется только взрыв.
    if (playerDeathAnimationActive) {
        updatePlayerDeathAnimation(deltaTime);
        inputState.clearOneFrameActions();
        return;
    }

    // Порождает нового врага, если пришло время.
    spawnEnemyIfNeeded(deltaTime);

    // Запоминаем исходное число объектов, чтобы новые объекты обновлялись со следующего кадра.
    const std::size_t initialCount = objects.size();

    // Полиморфно обновляем все активные объекты.
    for (std::size_t i = 0; i < initialCount && i < objects.size(); ++i) {
        if (objects[i]->isActive()) {
            objects[i]->update(*this, deltaTime);
        }

        // Если объект убил игрока, оставшаяся логика кадра уже не выполняется.
        if (playerDeathAnimationActive) {
            inputState.clearOneFrameActions();
            return;
        }
    }

    // Удаляем всё, что было собрано, уничтожено или погасло.
    removeInactiveObjects();

    // Проверяем, выполнено ли условие прохождения уровня.
    checkLevelCompleted();

    // Сбрасываем одноразовые команды движения и выстрела.
    inputState.clearOneFrameActions();
}


// Проверяет, есть ли в клетке мешок, который блокирует движение.
bool GameWorld::hasBlockingBagAt(Vec2i position) const {
    for (const auto& object : objects) {
        if (object->isActive() && object->getType() == ObjectType::GoldBag && object->getPosition() == position) {
            return true;
        }
    }

    return false;
}


// Ищет активный мешок золота в указанной клетке.
GoldBag* GameWorld::findGoldBagAt(Vec2i position) {
    for (auto& object : objects) {
        if (object->isActive() && object->getType() == ObjectType::GoldBag && object->getPosition() == position) {
            // Тип уже проверен через ObjectType, поэтому приводим GameObject к GoldBag.
            return static_cast<GoldBag*>(object.get());
        }
    }

    return nullptr;
}


// Пытается сдвинуть мешок золота в выбранном направлении.
bool GameWorld::tryPushGoldBag(Vec2i bagPosition, Direction direction) {
    GoldBag* bag = findGoldBagAt(bagPosition);

    if (bag == nullptr) {
        return false;
    }

    // Считаем клетку, куда должен сдвинуться мешок.
    const Vec2i offset = directionToVector(direction);
    const Vec2i target { bagPosition.x + offset.x, bagPosition.y + offset.y };

    if (!isInside(target)) {
        return false;
    }

    // Мешок можно толкнуть только в уже выкопанный тоннель.
    if (!isTunnel(target) || hasBlockingBagAt(target)) {
        return false;
    }

    bag->setPosition(target);
    return true;
}


// Проверяет, может ли мешок упасть в указанную клетку.
bool GameWorld::canBagFallInto(Vec2i position) const {
    if (!isInside(position)) {
        return false;
    }

    if (getTile(position) != TileType::Empty) {
        return false;
    }

    // Мешок не падает сквозь другой мешок.
    return !hasBlockingBagAt(position);
}


// Собирает изумруды и золото в клетке игрока.
void GameWorld::collectObjectsAtPlayer() {
    if (player == nullptr) {
        return;
    }

    const Vec2i playerPosition = player->getPosition();

    for (auto& object : objects) {
        // Самого игрока не обрабатываем как собираемый объект.
        if (!object->isActive() || object.get() == player) {
            continue;
        }

        if (object->getPosition() != playerPosition) {
            continue;
        }

        // Изумруд даёт 100 очков.
        if (object->getType() == ObjectType::Emerald) {
            object->deactivate();
            money += 100;
        }
        // Кучка золота даёт 250 очков.
        else if (object->getType() == ObjectType::GoldPile) {
            object->deactivate();
            money += 250;
        }
    }
}


// Собирает кучку золота в указанной клетке, например когда туда пришёл враг.
void GameWorld::collectGoldPileAt(Vec2i position) {
    for (auto& object : objects) {
        if (object->isActive() && object->getType() == ObjectType::GoldPile && object->getPosition() == position) {
            object->deactivate();
        }
    }
}


// Создаёт огненный шар перед игроком.
void GameWorld::spawnFireball(Vec2i position, Direction direction) {
    // Нельзя стрелять за карту, в землю или в мешок.
    if (!isInside(position) || getTile(position) == TileType::Earth || hasBlockingBagAt(position)) {
        return;
    }

    // Если враг стоит прямо рядом, уничтожаем его без создания Fireball.
    if (hasEnemyAt(position)) {
        killEnemiesAt(position, 200);
        return;
    }

    objects.push_back(std::make_unique<Fireball>(position, direction));
}


// Добавляет кучку золота после разбивания мешка.
void GameWorld::addGoldPile(Vec2i position) {
    if (!isInside(position)) {
        return;
    }

    objects.push_back(std::make_unique<GoldPile>(position));
}


// Уничтожает всех врагов в указанной клетке и начисляет награду.
void GameWorld::killEnemiesAt(Vec2i position, int reward) {
    for (auto& object : objects) {
        if (object->isActive() && object->getType() == ObjectType::Nobbin && object->getPosition() == position) {
            object->deactivate();
            money += reward;
        }
    }
}


// Проверяет, есть ли активный враг в клетке.
bool GameWorld::hasEnemyAt(Vec2i position) const {
    for (const auto& object : objects) {
        if (object->isActive() && object->getType() == ObjectType::Nobbin && object->getPosition() == position) {
            return true;
        }
    }

    return false;
}


// Раздавливает игрока и врагов в указанной клетке.
void GameWorld::crushObjectsAt(Vec2i position) {
    if (player != nullptr && player->isActive() && player->getPosition() == position) {
        killPlayer();
    }

    killEnemiesAt(position, 200);
}


// Запускает смерть игрока через короткую анимацию взрыва.
void GameWorld::killPlayer() {
    if (player == nullptr || playerDeathAnimationActive || state != GameState::Playing) {
        return;
    }

    // Запоминаем место смерти для отрисовки огня вместо машины.
    playerDeathPosition = player->getPosition();
    playerDeathTimer = 0.0f;
    playerDeathAnimationActive = true;
}


// Ищет первый шаг кратчайшего пути от врага к игроку.
// Если из клетки есть несколько разрешённых направлений, 
// добавляет все эти направления в очередь как разные ветки поиска. 
// Потом он расширяет эти ветки слоями: сначала все варианты за 1 шаг, 
// потом все варианты за 2 шага, потом за 3 шага. Поэтому первый найденный 
// путь до игрока является кратчайшим. После этого по previous восстанавливается 
// путь назад, и Nobbin получает первую клетку, куда нужно сделать шаг.
//
// Все возможные направления BFS рассматривает через очередь.
// Но когда игрок найден, обратный путь восстанавливается только по previous той ветки, которая реально привела к игроку.
Vec2i GameWorld::findNextStepToPlayer(Vec2i startPosition) const {
    if (player == nullptr) {
        return startPosition;
    }

    const Vec2i targetPosition = player->getPosition();

    if (startPosition == targetPosition) {
        return startPosition;
    }

    const int cellCount = width * height;

    // visited хранит, была ли клетка уже просмотрена.
    std::vector<bool> visited(static_cast<std::size_t>(cellCount), false);

    // previous хранит предыдущую клетку для восстановления пути.
    std::vector<Vec2i> previous(static_cast<std::size_t>(cellCount), { -1, -1 });

    // Очередь для поиска в ширину.
    std::queue<Vec2i> queue;
    queue.push(startPosition);
    visited.at(indexOf(startPosition)) = true;

    // Четыре возможных направления движения по сетке.
    const std::array<Vec2i, 4> moves = {
        Vec2i { 1, 0 },
        Vec2i { -1, 0 },
        Vec2i { 0, 1 },
        Vec2i { 0, -1 }
    };

    bool found = false;

    // BFS ищет кратчайший путь по тоннелям.
    while (!queue.empty()) {
        const Vec2i current = queue.front();
        queue.pop();

        if (current == targetPosition) {
            found = true;
            break;
        }

        for (const Vec2i& move : moves) {
            const Vec2i next { current.x + move.x, current.y + move.y };

            if (!isInside(next)) {
                continue;
            }

            const int nextIndex = indexOf(next);

            if (visited.at(nextIndex)) {
                continue;
            }

            // Nobbin не роет землю и не проходит через мешки.
            if (!isTunnel(next) || hasBlockingBagAt(next)) {
                continue;
            }

            visited.at(nextIndex) = true;
            previous.at(nextIndex) = current;
            queue.push(next);
        }
    }

    if (!found) {
        return startPosition;
    }

    Vec2i step = targetPosition;

    // Идём назад от игрока к врагу, пока не найдём первый шаг.
    while (previous.at(indexOf(step)) != startPosition) {
        step = previous.at(indexOf(step));

        if (step.x == -1 || step.y == -1) {
            return startPosition;
        }
    }

    return step;
}


// Загружает текущий уровень из файла.
void GameWorld::loadCurrentLevel(bool resetMoneyAndLives) {
    if (levelFiles.empty()) {
        throw std::runtime_error("No level files were set");
    }

    if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levelFiles.size())) {
        throw std::runtime_error("Invalid level index");
    }

    // При новой игре сбрасываем общий прогресс.
    if (resetMoneyAndLives) {
        lives = 3;
        money = 0;
    }

    LevelLoader loader;
    Level level = loader.loadFromFile(levelFiles.at(static_cast<std::size_t>(currentLevelIndex)));

    // Переносим параметры уровня в текущий игровой мир.
    width = level.getWidth();
    height = level.getHeight();
    levelNumber = level.getLevelNumber();
    playerSpawn = level.getPlayerSpawn();
    enemySpawn = level.getEnemySpawn();
    enemyReserve = level.getEnemyCount();
    enemySpawnTimer = 0.0f;
    playerDeathAnimationActive = false;
    playerDeathTimer = 0.0f;
    playerDeathPosition = playerSpawn;
    levelCompleteTimer = 0.0f;

    // tiles — это текущая карта GameWorld.
    // Она хранит клетки: TileType::Earth и TileType::Empty
    tiles.clear();
    tiles.reserve(static_cast<std::size_t>(width * height));

    // Копируем сетку земли и тоннелей из Level.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles.push_back(level.getTile({ x, y }));
        }
    }

    // Забираем стартовые объекты уровня.
    objects = level.takeObjects();

    // Создаём игрока и сохраняем быстрый указатель на него.
    auto playerObject = std::make_unique<Player>(playerSpawn);
    player = playerObject.get();
    objects.push_back(std::move(playerObject));

    // Первый враг появляется в начале уровня, остальные — по таймеру.
    if (enemyReserve > 0 && isTunnel(enemySpawn)) {
        objects.push_back(std::make_unique<Nobbin>(enemySpawn));
        --enemyReserve;
    }
}


// Переходит к следующему уровню.
void GameWorld::loadNextLevel() {
    ++currentLevelIndex;

    // Если уровней больше нет, игра выиграна.
    if (currentLevelIndex >= static_cast<int>(levelFiles.size())) {
        state = GameState::Win;
        return;
    }

    state = GameState::Playing;
    loadCurrentLevel(false);
}


// Создаёт очередного врага, если остались враги в резерве.
void GameWorld::spawnEnemyIfNeeded(float deltaTime) {
    if (enemyReserve <= 0) {
        return;
    }

    enemySpawnTimer += deltaTime;

    // Враги появляются не сразу, а с задержкой.
    if (enemySpawnTimer < 5.0f) {
        return;
    }

    enemySpawnTimer = 0.0f;

    // Точка появления должна быть свободным тоннелем.
    if (!isTunnel(enemySpawn) || hasBlockingBagAt(enemySpawn) || hasEnemyAt(enemySpawn)) {
        return;
    }

    objects.push_back(std::make_unique<Nobbin>(enemySpawn));
    --enemyReserve;
}


// Удаляет все неактивные объекты из вектора.
void GameWorld::removeInactiveObjects() {
    objects.erase(
        std::remove_if(
            objects.begin(),
            objects.end(),
            [](const std::unique_ptr<GameObject>& object) {
                return !object->isActive();
            }
        ),
        objects.end()
    );
}


// Проверяет условия прохождения уровня.
void GameWorld::checkLevelCompleted() {
    const bool allEmeraldsCollected = countActiveObjects(ObjectType::Emerald) == 0;
    const bool allEnemiesDestroyed = enemyReserve == 0 && countActiveObjects(ObjectType::Nobbin) == 0;

    if (allEmeraldsCollected || allEnemiesDestroyed) {
        startLevelCompleteTransition();
    }
}


// Запускает экран перехода после прохождения уровня.
void GameWorld::startLevelCompleteTransition() {
    if (state != GameState::Playing) {
        return;
    }

    state = GameState::LevelCompleted;
    levelCompleteTimer = 0.0f;
}


// Обновляет таймер перехода между уровнями.
void GameWorld::updateLevelCompleteTransition(float deltaTime) {
    constexpr float levelCompleteDuration = 1.5f;

    levelCompleteTimer += deltaTime;

    if (levelCompleteTimer >= levelCompleteDuration) {
        finishLevelCompleteTransition();
    }
}


// Завершает переход между уровнями.
void GameWorld::finishLevelCompleteTransition() {
    levelCompleteTimer = 0.0f;
    loadNextLevel();
}


// Обновляет таймер анимации смерти игрока.
void GameWorld::updatePlayerDeathAnimation(float deltaTime) {
    constexpr float deathAnimationDuration = 0.85f;

    playerDeathTimer += deltaTime;

    if (playerDeathTimer >= deathAnimationDuration) {
        finishPlayerDeathAnimation();
    }
}


// Завершает смерть игрока: отнимает жизнь или включает Game Over.
void GameWorld::finishPlayerDeathAnimation() {
    playerDeathAnimationActive = false;
    playerDeathTimer = 0.0f;

    --lives;

    if (lives <= 0) {
        state = GameState::GameOver;
        return;
    }

    // При потере одной жизни уровень не загружается заново.
    // Остаются выкопанные тоннели, собранные предметы, сдвинутые мешки и убитые враги.
    if (player != nullptr) {
        player->respawn(playerSpawn);
    }

    removeInactiveObjects();
}


// Переводит координаты клетки в индекс одномерного vector.
int GameWorld::indexOf(Vec2i position) const {
    if (!isInside(position)) {
        throw std::runtime_error("GameWorld position is outside of map");
    }

    return position.y * width + position.x;
}


// Считает активные объекты заданного типа.
int GameWorld::countActiveObjects(ObjectType objectType) const {
    int count = 0;

    for (const auto& object : objects) {
        if (object->isActive() && object->getType() == objectType) {
            ++count;
        }
    }

    return count;
}
