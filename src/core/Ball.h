#pragma once
#include "../mySTL/Vector3D.h"
#include "Parameter.h"

class Ball
{
public:
    Ball(const Vector3D &pos = Vector3D(), const float radius=BALL_RADIUS,const Material &material = MATERIAL[0]);
    ~Ball();
    
    void setPosition(const Vector3D &pos);
    void setVelocity(const Vector3D &vel);
    void setMaterial(const Material &material);

    Vector3D getPosition() const;
    Vector3D getVelocity() const;
    Material getMaterial() const;
    float getRadius() const;

    void update(float deltaTime);
    bool collidesWith(const Ball &other) const;
    float distanceTo(const Ball &other) const;

    bool operator==(const Ball &other) const;
private:
    float radius;
    Vector3D position;
    Vector3D velocity;
    Material material;
};