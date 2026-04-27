#pragma once

#include "GeoVector.h"

// Геометрия отрезка
struct SegmentGeometry {
    GeoPoint begin;
    GeoPoint end;
    float width = 0.0f;
};

// Текущее состояние движения
struct SegmentMotionState {
    GeoVector offset;
    GeoVector velocity;
};

// Параметры физики
struct SegmentMotionSettings {
    float acceleration = 4.0f;
    float friction = 2.5f;
    float bounce = 0.65f;
    float maxSpeed = 1.5f;
};