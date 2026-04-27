#pragma once

#include "GeoVector.h"
#include "SegmentStructs.h"

class Segment {
public:
    // Состояние клавиш
    struct ControlState {
        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
    };

    // Результат обновления за кадр
    struct UpdateResult {
        GeoVector offset;                // новое смещение
        bool moved = false;              // было ли движение
        float expansionDirection = 1.0f; // направление следа
        float expansionScale = 1.0f;     // сила расширения следа
    };

    Segment();

    GeoPoint begin() const { return geometry_.begin; }
    GeoPoint end() const { return geometry_.end; }
    float width() const { return geometry_.width; }
    GeoVector offset() const { return motion_.offset; }

    UpdateResult update(const ControlState& control, float deltaTime);

private:
    SegmentGeometry geometry_;
    SegmentMotionState motion_;
    SegmentMotionSettings motionSettings_;

    GeoVector buildAcceleration(const ControlState& control) const; // ускорение по клавишам
    void updateMotion(const GeoVector& accelerationVector, float deltaTime); // обновление за кадр
    void bounceFromWalls(); // отскок от стен
};