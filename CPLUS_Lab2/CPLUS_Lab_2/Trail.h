#pragma once

#include <vector>
#include "GeoVector.h"

struct TrailPart {
    GeoVector offset;       // старая позиция отрезка
    float age = 0.0f;         // возраст части следа
    float expansionDirection = 1.0f; // направление расширения: вверх или вниз
    float expansionScale = 1.0f; // коэффициент расширения следа.
};
class Trail {
public:
    Trail();

    void addPart(const GeoVector& offset, float expansionDirection, float expansionScale);
    void update(float deltaTime);

    const std::vector<TrailPart>& parts() const { return parts_; }

    float lifetime() const { return lifetime_; }
    float expansion() const { return expansion_; }
    float spawnDelay() const { return spawnDelay_; }

private:
    std::vector<TrailPart> parts_;

    float lifetime_;
    float expansion_;
    float spawnDelay_;
};