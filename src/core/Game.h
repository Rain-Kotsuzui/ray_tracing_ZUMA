#pragma once
#include "Ball.h"
#include "Path.h"
#include "../mySTL/MyList.h"
#include "Shooter.h"
#include "CollisionSystem.h"
#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    ~Game();
    void initialize();

    void update(float deltaTime);
    void handleClick(const float theta,const float phi);

    const Path &getPath() const { return path; }
    const QVector<Ball> &getBalls() const { return balls; }
    const QVector<Ball> &getShootingBalls() const { return shootingBalls; }
    MyList<Ball> &getBallList() { return ballList; }
    Shooter &getShooter() { return shooter; }

    bool isGameRunning() { return gameRunning; }
    void setGameRunningChange() { gameRunning = !gameRunning; }
signals:
    void gameOver();
private:
    Path path;
    Shooter shooter;
    QVector<Ball> balls;
    MyList<Ball> ballList;
    QVector<Ball> shootingBalls;
    CollisionSystem collisionSystem;
    bool gameRunning;

    void checkMatches();
    void createInitialBalls();
    Material generateRandomMaterial()const;
    void updateShootingBalls(float deltaTime);
    void checkGameOver();
};