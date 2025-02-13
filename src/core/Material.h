#pragma once
#include <QColor>

class Material
{
public:
    Material(const float light = 0, const float smooth = 0, const float glass = 1, const QColor color = QColor(0, 0, 0));
    ~Material();
    
    float light;  // 发光度0~1
    float smooth; // 光滑度0~1
    float glass;  // 着色度0~1,1为全反射
    QColor color;

    bool operator==(const Material &other) const;
};