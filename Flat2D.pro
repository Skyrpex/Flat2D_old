QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flat2D
TEMPLATE = app


SOURCES += main.cpp \
    Bone.cpp \
    View.cpp \
    Attachment.cpp \
    Application.cpp \
    MainWindow.cpp \
    commands/RotateCommand.cpp \
    commands/ScaleCommand.cpp \
    commands/TranslateCommand.cpp \
    commands/CreateCommand.cpp

HEADERS  += \
    Bone.hpp \
    View.hpp \
    Attachment.hpp \
    Application.hpp \
    MainWindow.hpp \
    commands/RotateCommand.hpp \
    commands/ScaleCommand.hpp \
    commands/TranslateCommand.hpp \
    commands/CreateCommand.hpp

RESOURCES += \
    images.qrc

FORMS += \
    MainWindow.ui
