#pragma once
#include "../mySTL/MyList.h"
#include "Path.h"
#include "Ball.h"

class CollisionSystem
{
public:
    CollisionSystem();
    ~CollisionSystem();

    bool checkPathCollisions(const Ball &shootingBall, MyList<Ball> &ballList, const Path &path);
    struct CollisionResult
    {
        bool hasCollision;
        MyList<Ball>::iterator collisionIndex;
    };

private:
    CollisionResult findCollision(const Ball &ball, MyList<Ball> &ballList) const;
    void handleCollision(Ball ball, MyList<Ball> &ballList, const Path &path, const MyList<Ball>::iterator &collisionIndex);
};