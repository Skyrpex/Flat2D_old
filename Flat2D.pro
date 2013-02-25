QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flat2D
TEMPLATE = app


SOURCES += main.cpp \
    Bone.cpp \
    View.cpp \
    Attachment.cpp \
    Application.cpp \
    MainWindow.cpp

HEADERS  += \
    Bone.hpp \
    View.hpp \
    Attachment.hpp \
    Application.hpp \
    MainWindow.hpp

RESOURCES += \
    images.qrc

FORMS += \
    MainWindow.ui
