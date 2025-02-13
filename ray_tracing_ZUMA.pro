QT += core gui widgets opengl openglwidgets
CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/ui/Camera.cpp \
    src/ui/GameWidget.cpp \
    src/ui/MainWindow.cpp \
    src/ui/Ray.cpp \
    src/core/Game.cpp \
    src/core/Ball.cpp \
    src/core/Material.cpp \
    src/core/Path.cpp \
    src/core/CollisionSystem.cpp \
    src/core/Shooter.cpp

HEADERS += \
    src/ui/Camera.h \
    src/ui/GameWidget.h \
    src/ui/MainWindow.h \
    src/ui/Ray.h \
    src/core/Game.h \
    src/core/Ball.h \
    src/core/Material.h \
    src/core/Parameter.h \
    src/core/Path.h \
    src/core/CollisionSystem.h\
    src/core/Shooter.h \
    src/mySTL/Vector3D.h \
    src/mySTL/MyList.h \
    src/mySTL/Container.h

RESOURCES += \
    src/res/image.qrc

INCLUDEPATH += \
    src \
    src/core \
    src/ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target