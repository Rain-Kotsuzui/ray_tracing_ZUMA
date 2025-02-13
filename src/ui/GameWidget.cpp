#include "GameWidget.h"
#include <QPainter>
#include <QPixmap>
#include <QDateTime>
#include <QThread>

GameWidget::GameWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      game(new Game(this)),
      gameTimer(new QTimer(this))
{
    setMinimumSize(SCREEN_MIN_X, SCREEN_MIN_Y);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);

    this->installEventFilter(this);

    gameTimer->setInterval(1000 / FRAME_RATE);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::updateGame);
    connect(game, &Game::gameOver, this, &GameWidget::handleGameOver);

    game->initialize();
    gameTimer->start();
}
GameWidget::~GameWidget()
{
    gameTimer->stop();
}

void GameWidget::paintEvent(QPaintEvent *)
{
    if (game->isGameRunning())
    {
        if (SCREEN_RESOLUTION_X <= SCREEN_RESOLUTION_MAX_X)
            SCREEN_RESOLUTION_X += 5;
        if (SCREEN_RESOLUTION_Y <= SCREEN_RESOLUTION_MAX_Y)
            SCREEN_RESOLUTION_Y += 5;
    }
    else
    {
        if (SCREEN_RESOLUTION_X > 30)
            SCREEN_RESOLUTION_X /= 1.1;
        if (SCREEN_RESOLUTION_Y > 30)
            SCREEN_RESOLUTION_Y /= 1.1;
    }
    Rend((*this).width(), (*this).height());
    if (game->isGameRunning())
    {
        if (DRAWPATH)
            drawPath();
        drawSight();
    }
    QPainter painter(this);
    painter.drawImage(0, 0, myPixmap);
}

QImage GameWidget::applyEffect(QImage src, QGraphicsEffect *effect, int extent = 0)
{
    if (src.isNull())
        return QImage();
    if (!effect)
        return src;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);

    QImage res(src.size() + QSize(extent * 2, extent * 2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF(-extent, -extent, src.width() + extent * 2, src.height() + extent * 2));
    return res;
}

void GameWidget::Rend(int width, int height)
{
    MyList<Ball> obj = game->getBallList();
    foreach (auto ball, game->getShootingBalls())
        obj.push_front(ball);

    QImage img(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, QImage::Format_RGB32);
    QImage lightImg(img.size(), QImage::Format_RGB32);
    lightImg.fill(Qt::transparent);

    QRgb *pix = (QRgb *)img.bits();
    QRgb *lightPix = (QRgb *)lightImg.bits();

    int lwidth = img.size().width();
    int lheight = img.size().height();
#pragma omp parallel for
    for (int i = 0; i < lwidth; i += 1)
        for (int j = 0; j < lheight; j += 1)
        {
            QRgb color = camera.getColor(obj, (i * 2.0 / lwidth - 1) * width / SCREEN_MIN_X, (1 - j * 2.0 / lheight) * height / SCREEN_MIN_Y);
            pix[j * lwidth + i] = color;
            if (qMax(qRed(color), qMax(qGreen(color), qBlue(color))) / 255 > 0.8)
                lightPix[j * lwidth + i] = color;
        }
    if (LIGHTING)
    {
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
        blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        blur->setBlurRadius(10);
        QImage result = applyEffect(lightImg.scaled(lightImg.size() / 2), blur).scaled(img.size());
        QRgb *res = (QRgb *)result.bits();
        for (int i = 0; i < lwidth; i++)
            for (int j = 0; j < lheight; j++)
                pix[j * lwidth + i] = qRgb(qRed(pix[j * lwidth + i]) + GLOW * qRed(res[j * lwidth + i]) > 255 ? 255 : qRed(pix[j * lwidth + i]) + GLOW * qRed(res[j * lwidth + i]),
                                           qGreen(pix[j * lwidth + i]) + GLOW * qGreen(res[j * lwidth + i]) > 255 ? 255 : qGreen(pix[j * lwidth + i]) + GLOW * qGreen(res[j * lwidth + i]),
                                           qBlue(pix[j * lwidth + i]) + GLOW * qBlue(res[j * lwidth + i]) > 255 ? 255 : qBlue(pix[j * lwidth + i]) + GLOW * qBlue(res[j * lwidth + i]));
    }
    if (game->isGameRunning())
        img = img.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else
        img = img.scaled(width, height, Qt::IgnoreAspectRatio);
    myPixmap = img;
}

void GameWidget::drawSight()
{
    double sight = 60 * atan(camera.focalLength / 4);
    QPainter painter(&myPixmap);
    painter.setPen(QPen(game->getShooter().getNextBall().getMaterial().color, 3));
    painter.drawEllipse(this->width() / 2 - sight / 2, this->height() / 2 - sight / 2, sight, sight);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.setCompositionMode(QPainter::CompositionMode_Difference);
    painter.drawEllipse(this->width() / 2 - sight / 6, this->height() / 2 - sight / 6, sight / 3, sight / 3);
}
void GameWidget::drawPath()
{
    const Path &path = game->getPath();
    const QVector<Vector3D> &points = path.getPoints();
    QPainter painter(&myPixmap);
    int width = myPixmap.width();
    int height = myPixmap.height();
    if (points.size() < 2)
        return;

    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < points.size() - 1; ++i)
    {
        float t1 = camera.focalLength * camera.focalLength / (camera.focal * (points[i] - camera.pos));
        float t2 = camera.focalLength * camera.focalLength / (camera.focal * (points[i + 1] - camera.pos));
        if (t1 < 0 || t2 < 0)
            continue;
        QPointF p1 = QPointF(((points[i] - camera.pos) * t1 - camera.focal) * camera.e_x * 0.5 * SCREEN_MIN_X + width * 0.5, -((points[i] - camera.pos) * t1 - camera.focal) * camera.e_y * 0.5 * SCREEN_MIN_Y + height * 0.5);
        QPointF p2 = QPointF(((points[i + 1] - camera.pos) * t2 - camera.focal) * camera.e_x * 0.5 * SCREEN_MIN_X + width * 0.5, -((points[i + 1] - camera.pos) * t2 - camera.focal) * camera.e_y * 0.5 * SCREEN_MIN_Y + height * 0.5);
        painter.drawLine(p1, p2);
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        game->handleClick(camera.theta, camera.phi);
        update();
    }
}
void GameWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint center = this->mapToGlobal(this->rect().center());
    QPoint currentPos = event->pos();
    camera.rotate(this->mapToGlobal(currentPos) - center);
    QCursor::setPos(center);
}

void GameWidget::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->angleDelta().y() / 1000.0);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        camera.speed = Vector3D(camera.focal.x, camera.focal.y, 0).normal();
    if (event->key() == Qt::Key_S)
        camera.speed = Vector3D(-camera.focal.x, -camera.focal.y, 0).normal();
    if (event->key() == Qt::Key_A)
        camera.speed = Vector3D(-camera.focal.y, camera.focal.x, 0).normal();
    if (event->key() == Qt::Key_D)
        camera.speed = Vector3D(camera.focal.y, -camera.focal.x, 0).normal();
    if (event->key() == Qt::Key_L)
        LIGHTING = !LIGHTING;
    if (event->key() == Qt::Key_K)
        GLOW_DELTA = 0.1;
    if (event->key() == Qt::Key_J)
        GLOW_DELTA = -0.1;
    if (event->key() == Qt::Key_P)
        DRAWPATH = !DRAWPATH;
    if (event->key() == Qt::Key_Space)
        camera.speed = Vector3D(0, 0, 1);
    if (event->key() == Qt::Key_Shift)
        camera.speed = Vector3D(0, 0, -1);
    if (event->key() == Qt::Key_Escape)
        emit gameStopState();
}
void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_S || event->key() == Qt::Key_A || event->key() == Qt::Key_D || event->key() == Qt::Key_Space || event->key() == Qt::Key_Shift)
        camera.speed = Vector3D(0, 0, 0);
    if (event->key() == Qt::Key_K || event->key() == Qt::Key_J)
        GLOW_DELTA = 0;
}

void GameWidget::updateGame()
{
    if (game->isGameRunning())
    {
        game->update(DELTA_TIME);
        Vector3D pos = camera.pos + camera.speed * DELTA_TIME - Vector3D(0, 0, (camera.pos + camera.speed * DELTA_TIME).z);
        if (pos.length() >= MOVE_RADIUS_LIMIT)
            camera.pos += camera.speed * DELTA_TIME * 0.1 + Vector3D(0, 0, (camera.speed * DELTA_TIME * 0.9).z);
        else
            camera.pos += camera.speed * DELTA_TIME;
        if (GLOW + GLOW_DELTA >= 0)
            GLOW += GLOW_DELTA;
        game->getShooter().setPosition(camera.pos);
        this->setCursor(Qt::BlankCursor);
    }
    update();
}

void GameWidget::gameStopState()
{
    game->setGameRunningChange();
    if (game->isGameRunning())
        update();
    else
    {
        this->setCursor(Qt::ArrowCursor);
        emit gameStop();
        update();
    }
}

void GameWidget::handleGameOver()
{
    gameTimer->stop();
    this->setCursor(Qt::ArrowCursor);
    update();
}

bool GameWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (!game->isGameRunning() && (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress))
        return true;
    else
        return QWidget::eventFilter(obj, event);
}
