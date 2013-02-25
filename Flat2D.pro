QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flat2D
TEMPLATE = app


SOURCES += main.cpp \
    Bone.cpp \
    View.cpp \
    Attachment.cpp \
    ResizeNode.cpp \
    Application.cpp

HEADERS  += \
    Bone.hpp \
    View.hpp \
    Attachment.hpp \
    ResizeNode.hpp \
    Application.hpp

RESOURCES += \
    images.qrc
