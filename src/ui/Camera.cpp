#include "Camera.h"
#include <QDebug>

Camera::Camera()
    : theta(0),
      phi(PI / 2),
      focalLength(1),
      viewLength(VIEW_DISTANCE),
      pos(0, 0, 100),
      focal(1, 0, 0),
      e_x(0, -1, 0),
      e_y(0, 0, 1) {}
Camera::~Camera() {}

void Camera::zoom(float zoom)
{
    if (this->focalLength * (zoom + 1) > 0.1)
    {
        this->focalLength += focalLength * zoom;
        VIEW_DISTANCE = focalLength > 20 ? focalLength * 100 : 2000;
        CAMERA_SENSITIVITY = focalLength > 3 ? focalLength * 100 : 300;
    }
    else if (zoom > 0)
        focalLength *= 2;
    else
        focalLength = 0.5 * focalLength;
    vecUpdate();
}
void Camera::rotate(QPointF rotation)
{
    this->theta -= rotation.x() / CAMERA_SENSITIVITY;
    if (this->phi + rotation.y() / CAMERA_SENSITIVITY < M_PI && this->phi + rotation.y() / CAMERA_SENSITIVITY > 0)
        this->phi += rotation.y() / CAMERA_SENSITIVITY;
    vecUpdate();
}

QRgb Camera::getColor(MyList<Ball> &objs, float x, float y)
{
    float r = 0, g = 0, b = 0;
    bool cut = true;
    int count = 0;
    Ray ray;
    for (; count < SAMPLE_COUNT && cut; count++)
    {
        cut = false;
        ray.reset(e_x * x + e_y * y + focal, this->pos);
        Ball nowObj = Ball(Vector3D(0, 0, 0), -1);
        for (int times = 0; ray.intersect(objs, nowObj, cut) && ray.getLength() < VIEW_DISTANCE && times < RAYREFLECTION_MAX_COUNT; times++)
        {
            ray.addColor();
        }
        ray.addColor();
        if (ray.getReflectCount() == 0)
            ray.setReflectCount(1);
        ray.setRGB(ray.getR() / ray.getReflectCount(), ray.getG() / ray.getReflectCount(), ray.getB() / ray.getReflectCount());
        ray.addFoggy(this->pos);

        r += ray.getR() / SAMPLE_COUNT;
        g += ray.getG() / SAMPLE_COUNT;
        b += ray.getB() / SAMPLE_COUNT;
    }
    return qRgb(qMin(round(r * (SAMPLE_COUNT / count)), 255.0), qMin(round(g * (SAMPLE_COUNT / count)), 255.0), qMin(round(b * (SAMPLE_COUNT / count)), 255.0));
}

void Camera::vecUpdate()
{
    if (theta >= 2 * PI)
        theta -= 2 * PI;
    if (theta < 0)
        theta += 2 * PI;
    focal = Vector3D(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)) * focalLength;
    e_x = Vector3D(sin(theta), -cos(theta), 0);
    e_y = e_x ^ focal / focalLength;
}
