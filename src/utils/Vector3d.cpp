//
// Created by referencecat on 09.02.24.
//

#include "Vector3d.h"

double Vector3d::mag() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3d Vector3d::operator+(const Vector3d &other) const {
    return {x + other.x, y + other.y, z + other.z};
}

double Vector3d::operator*(const Vector3d &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3d Vector3d::operator^(const Vector3d &other) const {
    return {z * other.y - y * other.z, x * other.z - z * other.x, y * other.x - x * other.y};
}

Vector3d Vector3d::operator-(const Vector3d &other) const {
    return *this + -other;
}

Vector3d Vector3d::operator*(double other) const {
    return {x * other, y * other, z * other};
}

Vector3d Vector3d::operator-() const {
    return *this * -1;
}

Vector3d Vector3d::rotateAroundZ(double angle) const {
    return {x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle), z};
}

Vector3d Vector3d::operator/(double other) const {
    return {x / other, y / other, z / other};
}

Vector3d Vector3d::norm() const {
    return *this / this->mag();
}

