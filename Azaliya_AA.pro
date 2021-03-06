QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -LD:\Qt\6.2.1\mingw81_64\lib\libQt5OpenGL.a -lopengl32
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Engine/Camera.cpp \
    Engine/CreateCommand.cpp \
    Engine/Global.cpp \
    Engine/Group.cpp \
    Engine/Logger.cpp \
    Engine/MainWindow.cpp \
    Game/ChessDesk.cpp \
    Game/ChessFigures.cpp \
    Models/Material.cpp \
    Models/MaterialLibrary.cpp \
    Models/ObjectEngine3D.cpp \
    Models/SimpleObject3D.cpp \
    Engine/SkyBox.cpp \
    main.cpp \
    Engine/widget.cpp \

HEADERS += \
    Engine/Camera.h \
    Engine/CreateCommand.h \
    Engine/Global.h \
    Engine/Group.h \
    Engine/Logger.h \
    Engine/MainWindow.h \
    Game/ChessDesk.h \
    Game/ChessFigures.h \
    Models/Material.h \
    Models/MaterialLibrary.h \
    Models/ObjectEngine3D.h \
    Models/SimpleObject3D.h \
    Engine/SkyBox.h \
    Engine/Transformational.h \
    Engine/Types.h \
    Engine/widget.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Chess.qrc \
    Shaders.qrc \
    Textures.qrc

FORMS += \
    Engine/MainWindow.ui
