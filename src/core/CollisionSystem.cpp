#include "CollisionSystem.h"
#include "Ball.h"
#include "Parameter.h"
#include "Game.h"
#include <algorithm>

CollisionSystem::CollisionSystem() {}
CollisionSystem::~CollisionSystem() {}

bool CollisionSystem::checkPathCollisions(const Ball &shootingBall, MyList<Ball> &ballList, const Path &path)
{
    CollisionResult result = findCollision(shootingBall, ballList);
    if (result.hasCollision)
        handleCollision(shootingBall, ballList, path, result.collisionIndex);
    return result.hasCollision;
}

CollisionSystem::CollisionResult CollisionSystem::findCollision(
    const Ball &ball, MyList<Ball> &ballList) const
{
    CollisionResult result = {false, MyList<Ball>::iterator(nullptr)};

    for (MyList<Ball>::iterator it = ballList.begin(); it != ballList.end(); ++it)
    {
        float distance = ball.distanceTo((*it)->data);
        if (distance < ball.getRadius() + (*it)->data.getRadius() + COLLISION_THRESHOLD)
        {
            result.hasCollision = true;
            result.collisionIndex = it;
            break;
        }
    }
    return result;
}

void CollisionSystem::handleCollision(Ball ball, MyList<Ball> &ballList, const Path &path, const MyList<Ball>::iterator &collisionIndex)
{
    // 获取碰撞点前后的球
    MyList<Ball>::iterator prevIndex = (collisionIndex == ballList.begin()) ? collisionIndex : collisionIndex - 1;
    MyList<Ball>::iterator nextIndex = (collisionIndex + 1 == ballList.end()) ? collisionIndex : collisionIndex + 1;

    const Ball &prevBall = (*prevIndex)->data;
    const Ball &nextBall = (*nextIndex)->data;

    float prevBallDistance = ball.distanceTo(prevBall);
    float nextBallDistance = ball.distanceTo(nextBall);

    // 计算插入位置
    MyList<Ball>::iterator insertIndex = (prevBallDistance < nextBallDistance) ? collisionIndex : collisionIndex + 1;
    if (insertIndex != ballList.end())
        ball.setPosition((*insertIndex)->data.getPosition());
    else
    {
        float currentDistance = path.getDistanceAtPoint((*collisionIndex)->data.getPosition());
        float newDistance = currentDistance + BALL_RADIUS * 2;
        Vector3D newPos = path.getPointAtDistance(newDistance);
        ball.setPosition(newPos);
    }
    for (MyList<Ball>::iterator it = insertIndex; it != ballList.end(); ++it)
    {
        float currentDistance = path.getDistanceAtPoint((*it)->data.getPosition());
        float newDistance = currentDistance + BALL_RADIUS * 2;
        Vector3D newPos = path.getPointAtDistance(newDistance);
        (*it)->data.setPosition(newPos);
        if (it + 1 != ballList.end())
        {
            float distance = (*it)->data.distanceTo((*(it + 1))->data);
            if (distance > BALL_RADIUS * 2 + EPS)
                break;
        }
    }

    // 插入球
    ball.setVelocity(Vector3D());
    ballList.insert(insertIndex, ball);
}
