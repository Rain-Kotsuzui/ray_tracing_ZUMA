#include "Ball.h"

Ball::Ball(const Vector3D &pos, const float radius, const Material &mat)
    : position(pos),
      velocity(Vector3D()),
      radius(radius),
      material(mat){}
Ball::~Ball() {}

void Ball::setPosition(const Vector3D &pos) { position = pos; }
void Ball::setVelocity(const Vector3D &vel) { velocity = vel; }
void Ball::setMaterial(const Material &mat) { material = mat; }
Vector3D Ball::getPosition() const { return position; }
Vector3D Ball::getVelocity() const { return velocity; }
Material Ball::getMaterial() const { return material; }
float Ball::getRadius() const { return radius; };

void Ball::update(float deltaTime) { position += velocity * deltaTime; }

bool Ball::collidesWith(const Ball &other) const
{
    float dist = radius + other.radius;
    return distanceTo(other) <= dist;
}

float Ball::distanceTo(const Ball &other) const
{
    Vector3D dis = position - other.position;
    return dis.length();
}
bool Ball::operator==(const Ball &other) const { return (position == other.position) &&
                                                        (velocity == other.velocity) &&
                                                        (material == other.material); }