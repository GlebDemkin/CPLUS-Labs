#include "GeoVector.h"

GeoVector::GeoVector() {
    x = 0.0f;
    y = 0.0f;
}

GeoVector::GeoVector(float xValue, float yValue) {
    x = xValue;
    y = yValue;
}

GeoVector& GeoVector::operator+=(const GeoVector& other) {
    x += other.x;
    y += other.y;
    return *this;
}

GeoVector& GeoVector::operator-=(const GeoVector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

GeoVector& GeoVector::operator*=(float number) {
    x *= number;
    y *= number;
    return *this;
}

GeoVector operator+(GeoVector left, const GeoVector& right) {
    left += right;
    return left;
}

GeoVector operator-(GeoVector left, const GeoVector& right) {
    left -= right;
    return left;
}

GeoVector operator*(GeoVector vector, float number) {
    vector *= number;
    return vector;
}