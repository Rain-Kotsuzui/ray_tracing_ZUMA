#include "Game.h"
#include <QDebug>
#include <QRandomGenerator>

Game::Game(QObject *parent)
    : QObject(parent),
      shooter(Vector3D(0, 0, 0)),
      gameRunning(true) {}
Game::~Game() {}

void Game::initialize()
{
    ballList.clear();
    shootingBalls.clear();
    gameRunning = true;

    // 创建路径
    path.clear();
    Vector3D pos = Vector3D();
    // 贝塞尔控制点
    path.addControlPoint(Vector3D(0, 0, 150));
    for (int i = 0; i < CORENER; i++)
    {
        float theta = (rand() * 2 * PI) / (RAND_MAX);
        float length = qMax((rand() * 2000) / (RAND_MAX), 500);
        float z = cos(rand()) * 500;
        pos = Vector3D(cos(theta) * length, sin(theta) * length, z + 50);
        path.addControlPoint(pos);
    }
    path.addControlPoint(Vector3D(0, 0, 50));
    // 计算贝塞尔曲线上的路径
    QVector<Vector3D> controlPoints = path.getControlPoints();
    auto Com = [](int n, int k) -> float
    {
        if (k > n)
            return 0;
        if (k == 0 || k == n)
            return 1;
        float result = 1;
        for (int i = 1; i <= k; ++i)
            result *= (n - i + 1) / (float)(i);
        return result;
    };

    for (int i = 0; i < controlPoints.size(); i++)
        controlPoints[i] = controlPoints[i] * Com(controlPoints.size() - 1, i);
    for (int i = 0; i <= PATH_SMOOTH; i++)
    {
        pos = Vector3D();
        float t = (i * 1.0 / PATH_SMOOTH);
        for (int j = 0; j < controlPoints.size(); j++)
            pos += controlPoints[j] * pow(1 - t, controlPoints.size() - j - 1) * pow(t, j);
        path.addPoint(pos);
    }

    createInitialBalls();
    shooter.setNextBall(Ball(shooter.getPosition(), BALL_RADIUS, generateRandomMaterial()));
}

void Game::update(float deltaTime)
{
    if (!gameRunning)
        return;
    updateShootingBalls(deltaTime);
    // 更新路径上的球，从前向后更新
    for (MyList<Ball>::iterator it = ballList.begin(); it != ballList.end(); ++it)
    {
        float currentDistance = path.getDistanceAtPoint((*it)->data.getPosition()); // 计算球在路径上的当前位置
        float newDistance = currentDistance + BALL_SPEED * deltaTime;               // 计算球在路径上的新位置
        Vector3D newPos = path.getPointAtDistance(newDistance);                     // 计算球的新位置
        (*it)->data.setPosition(newPos);                                            // 设置球的新位置
        if (it + 1 != ballList.end() && (*it)->data.distanceTo((*(it + 1))->data) > BALL_RADIUS * 2 + EPS)
            break; // 如果这个球和下一个球之间有空隙，则后面的所有球都不移动
    }
    checkGameOver();
}

void Game::handleClick(const float theta, const float phi)
{
    if (!gameRunning || !shooter.isReady())
        return;
    shooter.setAngle(theta, phi);
    Ball shotBall = shooter.shoot();
    shootingBalls.append(shotBall);
    shooter.setNextBall(Ball(shooter.getPosition(), BALL_RADIUS, generateRandomMaterial()));
}

void Game::updateShootingBalls(float deltaTime)
{
    for (int i = shootingBalls.size() - 1; i >= 0; i--)
    {
        Ball &ball = shootingBalls[i];
        ball.update(deltaTime);
        if ((ball.getPosition() - shooter.getPosition()).length() > 1000)
        {
            shootingBalls.removeAt(i);
            continue;
        }

        // 检查碰撞
        bool hasCollision = collisionSystem.checkPathCollisions(ball, ballList, path);
        if (hasCollision)
        {
            shootingBalls.removeAt(i);
            checkMatches();
        }
    }
    checkGameOver();
}

void Game::checkMatches()
{
    bool Continu = true;
    while (Continu)
    {
        int match = 1;
        Continu = false;
        for (MyList<Ball>::iterator it = ballList.begin() + 1; it != ballList.end(); ++it)
        {
            if ((*it)->data.getMaterial() == (*(it - 1))->data.getMaterial())
            {
                match++;
                if (it + 1 == ballList.end() && match >= 3)
                {
                    for (MyList<Ball>::iterator jt = it; match > 0; --match)
                    {
                        auto tem = jt;
                        --jt;
                        ballList.erase(tem);
                        Continu = true;
                    }
                    break;
                }
                continue;
            }
            else if (match >= 3)
                for (MyList<Ball>::iterator jt = it - 1; match > 0; --match)
                {
                    auto tem = jt;
                    --jt;
                    ballList.erase(tem);
                    Continu = true;
                }
            match = 1;
        }
    }
}
Material Game::generateRandomMaterial() const
{
    return MATERIAL[QRandomGenerator::global()->bounded(MATERIAL_TOTAL)];
}
void Game::createInitialBalls()
{

    const float ballSpacing = BALL_RADIUS * 2;
    float distance = 0;

    while (distance < qMin(path.getTotalLength() / 2, 2000.0))
    { 
        Vector3D pos = path.getPointAtDistance(distance);
        // 创建球并设置飞行速度为0
        Ball ball(pos, BALL_RADIUS, MATERIAL[rand() % MATERIAL_TOTAL]);
        ballList.insert(ballList.end(), ball);
        distance += ballSpacing;
    }
}

void Game::checkGameOver()
{
    if (!ballList.empty())
    {
        Vector3D ballPos = ballList.last()->data.getPosition();
        Vector3D endPoint = path.getPoints().last();
        if ((ballPos - endPoint).length() < BALL_RADIUS)
        {
            gameRunning = false;
            emit gameOver();
        }
    }
    else
    {
        // emit scoreChanged(score);
        gameRunning = false;
        emit gameOver();
    }
}