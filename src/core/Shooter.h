#pragma once
#include "Ball.h"
#include <QPointF>

class Shooter
{
public:
    Shooter(const Vector3D &position);
    ~Shooter();

    void setPosition(const Vector3D &pos);
    void setNextBall(const Ball &ball);
    void setShootingSpeed(float speed);
    void setAngle(float theta, float phi);

    Ball shoot();
    Vector3D getPosition() const;
    Ball getNextBall() const;
    bool isReady() const;

private:
    float theta, phi;
    Vector3D position;
    Ball nextBall;       
    bool ready;          
    float shootingSpeed; 
};