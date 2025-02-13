#pragma once
#include "../core/Parameter.h"
#include "../core/Ball.h"
#include "../mySTL/MyList.h"
#include "../mySTL/Vector3D.h"
#include "Ray.h"
#include <QTime>
#include <QPointF>

class Camera
{
public:
    Camera();
    ~Camera();

    float theta;
    float phi;
    float focalLength;
    int viewLength;

    Vector3D speed;
    Vector3D pos;
    Vector3D focal;
    Vector3D e_x; // x轴方向
    Vector3D e_y; // y轴方向

    void zoom(const float zoom);
    void rotate(QPointF rotation);
    QRgb getColor(MyList<Ball> &obj, float x, float y);
    void vecUpdate();
};