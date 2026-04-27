#pragma once

class GeoVector {
public:
    float x;
    float y;

    // Вектор по умолчанию: (0, 0)
    GeoVector();

    // Вектор с заданными координатами
    GeoVector(float xValue, float yValue);

    // Прибавление вектора
    GeoVector& operator+=(const GeoVector& other);

    // Вычитание вектора
    GeoVector& operator-=(const GeoVector& other);

    // Умножение вектора на число
    GeoVector& operator*=(float number);
};

// Сложение двух векторов
GeoVector operator+(GeoVector left, const GeoVector& right);

// Вычитание двух векторов
GeoVector operator-(GeoVector left, const GeoVector& right);

// Умножение вектора на число
GeoVector operator*(GeoVector vector, float number);

// Псевдоним: точка представляется тем же классом GeoVector
using GeoPoint = GeoVector;