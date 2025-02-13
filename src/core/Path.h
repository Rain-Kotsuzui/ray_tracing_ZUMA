#pragma once
#include "../mySTL/Vector3D.h"

class Path
{
public:
    Path();
    ~Path();
    void clear();
    const QVector<Vector3D> &getPoints() const;
    float getTotalLength() const;
    float getDistanceAtPoint(const Vector3D &point) const;
    Vector3D getPointAtDistance(float distance) const;
    const QVector<Vector3D> &getControlPoints() const;

    void addPoint(const Vector3D &point);
    void addControlPoint(const Vector3D &point);

private:
    struct ProjectionResult
    {
        float startDistance; // start到投影点的距离
        float pointDistance; // point到投影点的距离
    };

    QVector<Vector3D> controlPoints;
    QVector<Vector3D> points;

    ProjectionResult findClosestPointOnSegment(const Vector3D &point, const Vector3D &start, const Vector3D &end) const;
    float calculateSegmentLength(const Vector3D &start, const Vector3D &end) const;
};