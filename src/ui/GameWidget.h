#pragma once
#include "../core/Game.h"
#include "Camera.h"
#include "../mySTL/Vector3D.h"
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <qtimer.h>
#include <QImage>
#include <QtOpenGLWidgets/QOpenGLWidget>

class GameWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    Game *getGame() const { return game; }
    QTimer *getTimer() const { return gameTimer; }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void handleGameOver();
    void gameStopState();

private:
    Game *game;
    QTimer *gameTimer;
    Camera camera;
    QImage myPixmap;

    void Rend(int height, int width);
    void drawPath();
    void drawSight();
    QImage applyEffect(QImage image, QGraphicsEffect *effect, int extent);

signals:
    void gameStop();
};