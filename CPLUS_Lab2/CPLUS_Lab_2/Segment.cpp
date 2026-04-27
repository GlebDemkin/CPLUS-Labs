#include "Segment.h"

#include <cmath>

Segment::Segment() {
    // Локальная геометрия отрезка
    geometry_.begin = GeoPoint(-0.5f, 0.0f);
    geometry_.end = GeoPoint(0.5f, 0.0f);
    geometry_.width = 0.05f;
}

GeoVector Segment::buildAcceleration(const ControlState& control) const {
    GeoVector accelerationVector;

    if (control.left) {
        accelerationVector += GeoVector(-motionSettings_.acceleration, 0.0f);
    }

    if (control.right) {
        accelerationVector += GeoVector(motionSettings_.acceleration, 0.0f);
    }

    if (control.up) {
        accelerationVector += GeoVector(0.0f, motionSettings_.acceleration);
    }

    if (control.down) {
        accelerationVector += GeoVector(0.0f, -motionSettings_.acceleration);
    }

    return accelerationVector;
}

void Segment::updateMotion(const GeoVector& accelerationVector, float deltaTime) {
    // Разгон от клавиш
    motion_.velocity += accelerationVector * deltaTime;

    // Ограничение скорости
    if (motion_.velocity.x > motionSettings_.maxSpeed) {
        motion_.velocity.x = motionSettings_.maxSpeed;
    }

    if (motion_.velocity.x < -motionSettings_.maxSpeed) {
        motion_.velocity.x = -motionSettings_.maxSpeed;
    }

    if (motion_.velocity.y > motionSettings_.maxSpeed) {
        motion_.velocity.y = motionSettings_.maxSpeed;
    }

    if (motion_.velocity.y < -motionSettings_.maxSpeed) {
        motion_.velocity.y = -motionSettings_.maxSpeed;
    }

    // Трение
    float frictionMultiplier = 1.0f - motionSettings_.friction * deltaTime;

    if (frictionMultiplier < 0.0f) {
        frictionMultiplier = 0.0f;
    }

    motion_.velocity *= frictionMultiplier;

    // Смещение по скорости
    motion_.offset += motion_.velocity * deltaTime;

    // Убираем дрожание на малой скорости
    if (motion_.velocity.x > -0.001f && motion_.velocity.x < 0.001f) {
        motion_.velocity.x = 0.0f;
    }

    if (motion_.velocity.y > -0.001f && motion_.velocity.y < 0.001f) {
        motion_.velocity.y = 0.0f;
    }
}

void Segment::bounceFromWalls() {
    float left = geometry_.begin.x;
    float right = geometry_.end.x;
    float bottom = geometry_.begin.y;
    float top = geometry_.begin.y + geometry_.width;

    if (left + motion_.offset.x < -1.0f) {
        motion_.offset.x = -1.0f - left;
        motion_.velocity.x = -motion_.velocity.x * motionSettings_.bounce;
    }

    if (right + motion_.offset.x > 1.0f) {
        motion_.offset.x = 1.0f - right;
        motion_.velocity.x = -motion_.velocity.x * motionSettings_.bounce;
    }

    if (bottom + motion_.offset.y < -1.0f) {
        motion_.offset.y = -1.0f - bottom;
        motion_.velocity.y = -motion_.velocity.y * motionSettings_.bounce;
    }

    if (top + motion_.offset.y > 1.0f) {
        motion_.offset.y = 1.0f - top;
        motion_.velocity.y = -motion_.velocity.y * motionSettings_.bounce;
    }
}

Segment::UpdateResult Segment::update(const ControlState& control, float deltaTime) {
    UpdateResult result;
    GeoVector oldOffset = motion_.offset;

    GeoVector accelerationVector = buildAcceleration(control);

    updateMotion(accelerationVector, deltaTime);
    bounceFromWalls();

    result.offset = motion_.offset;
    result.moved = (oldOffset.x != motion_.offset.x || oldOffset.y != motion_.offset.y);

    GeoVector frameMovement = motion_.offset - oldOffset;

    if (frameMovement.y > 0.0f) {
        result.expansionDirection = -1.0f;
    }
    else if (frameMovement.y < 0.0f) {
        result.expansionDirection = 1.0f;
    }
    else {
        result.expansionDirection = 1.0f;
    }

    float speedValue = std::sqrt(motion_.velocity.x * motion_.velocity.x + motion_.velocity.y * motion_.velocity.y); // высчитываем текущую скорость

    result.expansionScale = speedValue / motionSettings_.maxSpeed;

    if (result.expansionScale > 1.0f) {
        result.expansionScale = 1.0f;
    }

    if (result.expansionScale < 0.2f) {
        result.expansionScale = 0.2f;
    }

    return result;
}