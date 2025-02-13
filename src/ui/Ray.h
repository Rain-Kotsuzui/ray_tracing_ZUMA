#pragma once
#include "../mySTL/MyList.h"
#include "../mySTL/Vector3D.h"
#include "../core/Ball.h"

class Ray
{
public:
    Ray();
    ~Ray();

    bool intersect(MyList<Ball> &obj, Ball &nowObj, bool &cut);
    void addColor();
    void addFoggy(Vector3D &pos);

    float getR();
    float getG();
    float getB();
    float getLength();
    float getLastRate();
    float getReflectCount();
    Material getMaterial();
    Vector3D &getDirection();
    Vector3D &getStartPos();

    void setRGB(float r, float g, float b);
    void setReflectCount(float count);
    void reset(Vector3D direction, Vector3D startPos);

private:
    float length;
    float r, g, b;
    float lastRate;
    Material nowMaterial;
    Vector3D direction;
    Vector3D startPos;
    float reflectCount;

    void setLength(float len);
    void setLastRate(float rate);
    void setMaterial(Material mat);
    void setStartPos(Vector3D pos);
    void setDirection(Vector3D dir);
};
