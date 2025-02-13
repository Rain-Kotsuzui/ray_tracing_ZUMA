#pragma once
#include <cmath>
#include "../core/Parameter.h"
#include <qdebug.h>

class Vector3D
{
public:
    Vector3D(const float &x = 0, const float &y = 0, const float &z = 0) : x(x), y(y), z(z) {};

    float operator*(const Vector3D &other) const { return x * other.x + y * other.y + z * other.z; };
    const Vector3D operator^(const Vector3D &other) const { return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }; // cross product
    const Vector3D operator+(const Vector3D &other) const { return Vector3D(x + other.x, y + other.y, z + other.z); };
    const Vector3D operator-(const Vector3D &other) const { return Vector3D(x - other.x, y - other.y, z - other.z); };
    const Vector3D operator*(float scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); };
    const Vector3D operator/(float scalar) const { return Vector3D(x / scalar, y / scalar, z / scalar); };
    const Vector3D operator-() const { return Vector3D(-x, -y, -z); };
    Vector3D &operator+=(const Vector3D &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };
    Vector3D &operator-=(const Vector3D &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    };
    bool operator==(const Vector3D &other) const { return abs(x - other.x) < EPS && abs(y - other.y) < EPS && abs(z - other.z) < EPS; };
    bool operator!=(const Vector3D &other) const { return !(*this == other); };

    float length() const { return std::sqrt(x * x + y * y + z * z); };
    Vector3D normal() { return (*this) = (*this) / length(); };
    Vector3D normal() const { return (*this) / length(); };
    float angle(Vector3D &other) { return std::acos((*this) * other / (length() * other.length())); };
    float getPhi() { return acos(z); }
    float getTheta() { return atan2(y, x) < 0 ? 2 * PI + atan2(y, x) : atan2(y, x); }
    // phi=[0,pi]
    Vector3D rotateTheta(float theta)
    {
        return (*this) = Vector3D(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta), z);
    }
    Vector3D rotatePhi(float phi)
    {
        return (*this) = Vector3D(x * cos(phi) + z * sin(phi), y, -x * sin(phi) + z * cos(phi));
    }
    Vector3D randVector(Vector3D n, float leng) const
    {
        Vector3D res = (*this);
        n.normal();
        float ntheta = n.getTheta(), nphi = n.getPhi();
        res.rotateTheta(-ntheta);
        res.rotatePhi(-nphi);
        res = (res * (1 - leng) + Vector3D(0, 0, 1).rotatePhi(rand() * PI / (2 * RAND_MAX)).rotateTheta(rand() * 2 * PI / RAND_MAX) * leng);
        res.rotatePhi(nphi);
        res.rotateTheta(ntheta);
        return res;
    };
    float x, y, z;
};