QT += core gui widgets

TARGET = ducky-snake-qt
TEMPLATE = app

# Enable C++11 (or later) support
# Note: Qt 6 requires C++17, but qmake usually handles that automatically.
CONFIG += c++11

SOURCES += \
    main.cpp \
    gamemodel.cpp \
    gameview.cpp

HEADERS += \
    gamemodel.h \
    gameview.h
