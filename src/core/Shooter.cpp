#include "Shooter.h"
#include "Parameter.h"
#include <cmath>

Shooter::Shooter(const Vector3D &pos)
    : position(pos),
      nextBall(pos),
      ready(true),
      shootingSpeed(SHOOTING_BALL_SPEED) {}
Shooter::~Shooter() {}

void Shooter::setPosition(const Vector3D &pos)
{
    position = pos;
    if (ready)
        nextBall.setPosition(pos);
}
void Shooter::setNextBall(const Ball &ball)
{
    nextBall = ball;
    nextBall.setPosition(position);
    ready = true;
}
void Shooter::setShootingSpeed(float speed) { shootingSpeed = speed; }
void Shooter::setAngle(float theta, float phi) { this->theta = theta, this->phi = phi; }

Vector3D Shooter::getPosition() const { return position; }
Ball Shooter::getNextBall() const { return nextBall; }

Ball Shooter::shoot()
{
    if (!ready)
        return Ball();
    Ball shootingBall = nextBall;
    shootingBall.setVelocity(Vector3D(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)) * shootingSpeed);
    ready = false;
    return shootingBall;
}

bool Shooter::isReady() const { return ready; }