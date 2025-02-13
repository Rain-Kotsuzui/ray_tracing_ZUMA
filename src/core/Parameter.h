#pragma once
#include "Material.h"
#include <QVector>
#include <QColor>
#include <QImage>

static constexpr float EPS = 1e-6;
static constexpr float PI = 3.14159265358979323846;

static constexpr float BALL_RADIUS = 20.0f;
static constexpr float COLLISION_THRESHOLD = 1.0f;            // 碰撞检测阈值
static constexpr float BALL_SPEED = 0.5f;                     // 轨道上球的移动速度
static constexpr float SHOOTING_BALL_SPEED = BALL_SPEED * 20; // 正在飞行的球的飞行速度
static constexpr int MIN_MATCH_COUNT = 3;                     // 最小匹配消除的数目
static constexpr int MATERIAL_TOTAL = 7;
static constexpr int MOVE_RADIUS_LIMIT = 100; // 移动范围限制
static constexpr int CORENER = 50;            // 路径控制点数量
static constexpr int PATH_SMOOTH = 150;       // 路径平滑程度

static constexpr int FRAME_RATE = 60;                   // 帧率
static constexpr float DELTA_TIME = 100.0 / FRAME_RATE; // 时间间隔
static constexpr float SCREEN_MIN_X = 600;              // 屏幕最小宽度
static constexpr float SCREEN_MIN_Y = 600;              // 屏幕最小高度
static constexpr float SCREEN_RESOLUTION_MAX_X = 300;   // X轴最大分辨率
static constexpr float SCREEN_RESOLUTION_MAX_Y = 300;   // Y轴最大分辨率
static float SCREEN_RESOLUTION_X = SCREEN_RESOLUTION_MAX_X;
static float SCREEN_RESOLUTION_Y = SCREEN_RESOLUTION_MAX_Y;

static float CAMERA_SENSITIVITY = 400.0;  // 鼠标灵敏度
static float VIEW_DISTANCE =1000;         // 视距
static float FOGGYNESS = 0.1;             // 雾化程度
static float GLOW = 0.5;                  // 泛光
static float GLOW_DELTA = 0;              // 泛光变化
static constexpr float LIGHT_DECAY = 0.5; // 0~1

static QImage GROUND = QImage(":/images/2.png"); // 地面贴图
static constexpr float GROUND_LIGHT = 0;         // 地面光照
static constexpr float GROUND_SMOOTH = 0.2;      // 地面光滑度
static constexpr float GROUND_GLASS = 0;         // 地面着色度

static bool SET_GROUND = true;        // 是否设置地面
static bool LIGHTING = true;
static bool DRAWPATH = true;

static constexpr int RAYREFLECTION_MAX_COUNT = 100; // 最大反射次数
static constexpr int SAMPLE_COUNT = 1;              // 光线采样次数

static QColor ENVIRONMENT_COLOR_NIGHT = QColor(0, 0, 0);     // 夜晚环境颜色
static QColor ENVIRONMENT_COLOR_DAY = QColor(98, 177, 236);  // 白天环境颜色
static QColor ENVIRONMENT_COLOR_DUSK = QColor(214, 139, 43); // 黄昏环境颜色

static const QVector<QColor> COLORS = {
    QColor(0, 0, 0),       // 黑0
    QColor(255, 0, 0),     // 红1
    QColor(0, 255, 0),     // 绿2
    QColor(0, 0, 255),     // 蓝3
    QColor(255, 255, 255), // 白4,4
    QColor(255, 180, 0),   // 橙5
    QColor(255, 0, 255),   // 紫6
};
static const QVector<Material> MATERIAL = {
    Material(0, 1, 0, COLORS[0]),
    Material(20, 1, 0.5, COLORS[1]),
    Material(20, 1, 0.2, COLORS[2]),
    Material(0, 0.6, 0, COLORS[3]),
    Material(-0.01, 0.3, 0, COLORS[4]),
    Material(2, 1, 1, COLORS[5]),
    Material(0, 0.2, 0.2, COLORS[6]),
};