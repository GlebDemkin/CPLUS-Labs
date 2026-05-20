#include "objects/Player.h"

#include "game/GameWorld.h"  // игровой мир
#include "objects/GoldBag.h" // мешок золота


// Обновляет игрока.
void Player::update(GameWorld& world, float deltaTime) {
    // Задержка между шагами игрока.
    constexpr float moveDelay = 0.10f;

    // Задержка между выстрелами.
    constexpr float fireDelay = 0.75f;

    // Накапливаем время движения.
    moveTimer += deltaTime;

    // Уменьшаем перезарядку выстрела.
    if (fireCooldown > 0.0f) {
        fireCooldown -= deltaTime;
    }

    // Получаем состояние ввода.
    InputState& input = world.getInputState();

    // Если запрошен выстрел и перезарядка закончилась.
    if (input.fireRequested && fireCooldown <= 0.0f) {
        Vec2i offset = directionToVector(direction);

        Vec2i firePosition{
            getPosition().x + offset.x,
            getPosition().y + offset.y
        };

        // Создаём огненный шар.
        world.spawnFireball(firePosition, direction);

        // Запускаем перезарядку.
        fireCooldown = fireDelay;
    }

    // Если запрошено движение и задержка прошла.
    if (input.moveRequested && moveTimer >= moveDelay) {
        tryMove(world, input.moveDirection);
        moveTimer = 0.0f;
    }
}

// Возрождает игрока после потери жизни.
void Player::respawn(Vec2i startPosition) {
    setPosition(startPosition);
    direction = Direction::Right;
    moveTimer = 0.0f;
    fireCooldown = 0.0f;
}

// Пытается сдвинуть игрока на одну клетку.
void Player::tryMove(GameWorld& world, Direction moveDirection) {
    // Сразу поворачиваем игрока в сторону движения.
    direction = moveDirection;

    // Получаем смещение по направлению.
    const Vec2i offset = directionToVector(moveDirection);

    // Текущая позиция игрока.
    const Vec2i current = getPosition();

    // Целевая клетка.
    const Vec2i target{
        current.x + offset.x,
        current.y + offset.y
    };

    // Нельзя выйти за границы карты.
    if (!world.isInside(target)) {
        return;
    }

    // Если в целевой клетке есть мешок золота.
    if (world.findGoldBagAt(target) != nullptr) {
        // Мешок можно толкать только влево или вправо.
        if (moveDirection != Direction::Left && moveDirection != Direction::Right) {
            return;
        }

        // Если мешок нельзя толкнуть, игрок не двигается.
        if (!world.tryPushGoldBag(target, moveDirection)) {
            return;
        }

        // После успешного толкания клетка становится тоннелем.
        world.setTile(target, TileType::Empty);
    }

    // Если в целевой клетке земля, игрок её выкапывает.
    else if (world.getTile(target) == TileType::Earth) {
        world.setTile(target, TileType::Empty);
    }

    // Перемещаем игрока.
    setPosition(target);

    // Собираем предметы в клетке игрока.
    world.collectObjectsAtPlayer();

    // Если в клетке враг, игрок погибает.
    if (world.hasEnemyAt(target)) {
        world.killPlayer();
    }
}