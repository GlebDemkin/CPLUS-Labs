#pragma once

// Общий тип игрового объекта. Вынесен отдельно, чтобы Renderer и GameWorld
// могли определять вид объекта без лишней зависимости от конкретных классов.

// Тип активной игровой сущности.
enum class ObjectType {
    Player,   // игрок Diggermobile
    Nobbin,   // враг
    Emerald,  // изумруд
    GoldBag,  // мешок золота
    GoldPile, // кучка золота
    Fireball  // огненный шар
};