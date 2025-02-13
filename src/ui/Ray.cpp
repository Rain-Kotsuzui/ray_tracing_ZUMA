#include "Ray.h"
#include <QTime>

Ray::Ray()
    : length(-0.1),
      r(0),
      g(0),
      b(0),
      lastRate(1),
      direction(0, 0, 0),
      reflectCount(0) {}
Ray::~Ray() {}

bool Ray::intersect(MyList<Ball> &objs, Ball &nowObj, bool &cut)
{
    Vector3D n = this->getDirection();
    Vector3D s = this->getStartPos();
    float nowRayLength = -1;
    // Ball
    for (auto obj : objs)
    {
        Vector3D o = obj->data.getPosition();
        float r = obj->data.getRadius();
        auto delta = [](float a, float b, float c) -> float
        { return b * b - 4 * a * c; };
        
        float del = delta(1, n * (s - o) * 2, (s - o) * (s - o) - r * r);
        if (del < 0 || nowObj == obj->data)
            continue;
        float newRayLength = n * (o - s) - sqrt(del) / 2;
        if (newRayLength > 0 && (nowRayLength < 0 || nowRayLength > newRayLength))
        {
            nowRayLength = newRayLength;
            nowObj = obj->data;
        }
    }
    // ground
    if (SET_GROUND)
    {
        float groundRayLength = -s.z / n.z;
        if (groundRayLength > 0.1 + EPS)
        {
            Vector3D groundPos = s + n * groundRayLength;
            QRgb *pix = (QRgb *)GROUND.bits();
            QRgb groundColor = pix[(int)abs(groundPos.x * 5) % GROUND.height() + ((int)abs(groundPos.y * 5) % GROUND.width()) * GROUND.width()];
            if (nowRayLength < 0 || (nowRayLength > groundRayLength))
            {
                this->setStartPos(groundPos);
                this->setMaterial(Material(GROUND_LIGHT, GROUND_SMOOTH, GROUND_GLASS, QColor(groundColor)));
                this->setReflectCount(this->getReflectCount() + GROUND_GLASS);
                this->setLength(this->getLength() + groundRayLength);
                this->setDirection(Vector3D(this->getDirection().x,
                                            this->getDirection().y,
                                            -this->getDirection().z)
                                       .randVector(Vector3D(0, 0, 1), 1 - GROUND_SMOOTH));
                cut = true;
                return true;
            }
        }
    }
    // color
    if (nowRayLength < 0 || this->getLength() > VIEW_DISTANCE)
    {
        this->setStartPos(this->getDirection() * 10 * VIEW_DISTANCE * (this->getLastRate()) + this->getStartPos());
        this->setReflectCount(this->getReflectCount());
        return false;
    }
    else
    {
        Vector3D o = nowObj.getPosition();
        float r = nowObj.getRadius();
        this->setLength(nowRayLength + this->getLength());
        this->setMaterial(nowObj.getMaterial());
        this->setReflectCount(this->getReflectCount() + 1 - nowObj.getMaterial().glass);
        this->setStartPos(this->getDirection() * nowRayLength + this->getStartPos());
        float lambda = (s - o) * (s - o + n * nowRayLength) / (r * r);
        this->setDirection((s - o + n * nowRayLength) * (2 * lambda - 1) + o - s);
        this->setDirection(this->getDirection().randVector(this->getStartPos() - o, 1 - nowObj.getMaterial().smooth));
        if (nowObj.getMaterial().smooth < 0.9)
            cut = true;
        return true;
    }
}
void Ray::addColor()
{
    this->setRGB(this->getR() * (1 - this->getLastRate()) + this->getMaterial().color.red() * (this->getMaterial().light + this->getLastRate()), this->getG() * (1 - this->getLastRate()) + this->getMaterial().color.green() * (this->getMaterial().light + this->getLastRate()), this->getB() * (1 - this->getLastRate()) + this->getMaterial().color.blue() * (this->getMaterial().light + this->getLastRate()));
    this->setLastRate(this->getLastRate() * (this->getMaterial().glass));
}
void Ray::addFoggy(Vector3D &cameraPos)
{
    static qint64 STARTTIME = QDateTime::currentMSecsSinceEpoch();
    qint64 currentTime = (QDateTime::currentMSecsSinceEpoch() - STARTTIME); // 秒
    double t = sin(currentTime / (1000.0 * 5)) * 0.5 + 0.5;                 // 0~60s->0~1
    QColor color = QColor(ENVIRONMENT_COLOR_DAY.red() * t * t + ENVIRONMENT_COLOR_DUSK.red() * t * (1 - t) * 2 + ENVIRONMENT_COLOR_NIGHT.red() * (1 - t) * (1 - t), ENVIRONMENT_COLOR_DAY.green() * t * t + ENVIRONMENT_COLOR_DUSK.green() * t * (1 - t) * 2 + ENVIRONMENT_COLOR_NIGHT.green() * (1 - t) * (1 - t), ENVIRONMENT_COLOR_DAY.blue() * t * t + ENVIRONMENT_COLOR_DUSK.blue() * t * (1 - t) * 2 + ENVIRONMENT_COLOR_NIGHT.blue() * (1 - t) * (1 - t));
    if (this->getLength() < 0)
        this->setRGB(color.red(), color.green(), color.blue());
    this->setRGB((this->getR() * VIEW_DISTANCE + color.red() * ((this->startPos - cameraPos).length()) * FOGGYNESS) / (VIEW_DISTANCE + ((this->startPos - cameraPos).length()) * FOGGYNESS), (this->getG() * VIEW_DISTANCE + color.green() * ((this->startPos - cameraPos).length()) * FOGGYNESS) / (VIEW_DISTANCE + ((this->startPos - cameraPos).length()) * FOGGYNESS), (this->getB() * VIEW_DISTANCE + color.blue() * ((this->startPos - cameraPos).length()) * FOGGYNESS) / (VIEW_DISTANCE + ((this->startPos - cameraPos).length()) * FOGGYNESS));
}

float Ray::getR() { return r; }
float Ray::getG() { return g; }
float Ray::getB() { return b; }
float Ray::getLength() { return length; }
float Ray::getLastRate() { return lastRate; }
float Ray::getReflectCount() { return reflectCount; }
Material Ray::getMaterial() { return nowMaterial; }
Vector3D &Ray::getDirection() { return direction; }
Vector3D &Ray::getStartPos() { return startPos; }

void Ray::setRGB(float R, float G, float B) { r = R, g = G, b = B; }
void Ray::setLength(float len) { length = len; }
void Ray::setLastRate(float rate) { lastRate = rate; }
void Ray::setMaterial(Material m) { nowMaterial = m; }
void Ray::setStartPos(Vector3D pos) { startPos = pos; }
void Ray::setDirection(Vector3D dir) { direction = dir.normal(); }
void Ray::setReflectCount(float count) { reflectCount = count; }
void Ray::reset(Vector3D Direction, Vector3D startPos)
{
    this->setMaterial(Material());
    this->setDirection(Direction);
    this->setStartPos(startPos);
    this->getDirection().normal();
    this->setLength(-1);
    this->setReflectCount(0);
}
