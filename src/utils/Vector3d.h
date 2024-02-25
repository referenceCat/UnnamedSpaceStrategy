//
// Created by referencecat on 09.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_VECTOR3D_H
#define UNNAMEDSPACESTRATEGY_VECTOR3D_H

#include <cmath>


struct Vector3d {
    double x, y, z;

    Vector3d(double x, double y, double z) : x(x), y(y), z(z) {};

    Vector3d(double x, double y) : x(x), y(y), z(0) {};

    Vector3d() : x(0), y(0), z(0) {};

    double mag() const;

    Vector3d operator+(const Vector3d &other) const;

    Vector3d operator-(const Vector3d &other) const;

    double operator*(const Vector3d &other) const;

    Vector3d operator*(double other) const;

    Vector3d operator/(double other) const;

    Vector3d operator^(const Vector3d &other) const;

    Vector3d operator-() const;

    Vector3d rotateAroundZ(double angle) const;

    Vector3d norm() const;

};


#endif //UNNAMEDSPACESTRATEGY_VECTOR3D_H
