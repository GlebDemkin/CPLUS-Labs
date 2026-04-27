#include "Trail.h"

#include <algorithm>

Trail::Trail() {
    lifetime_ = 0.5f;     // сколько секунд живёт след
    expansion_ = 0.12f;   // насколько след расширяется
    spawnDelay_ = 0.007f; // как часто добавляется новая часть следа
}

void Trail::addPart(const GeoVector& offset, float expansionDirection, float expansionScale) {
    TrailPart part;
    part.offset = offset;
    part.age = 0.0f;
    part.expansionDirection = expansionDirection;
    part.expansionScale = expansionScale;

    parts_.push_back(part);
}

void Trail::update(float deltaTime) {
    // Увеличиваем возраст всех частей следа
    for (TrailPart& part : parts_) {
        part.age += deltaTime;
    }

    // Удаляем старые части следа
    for (int i = 0; i < parts_.size(); ) {
        if (parts_[i].age >= lifetime_) {
            parts_.erase(parts_.begin() + i);
        }
        else {
            ++i;
        }
    }
}