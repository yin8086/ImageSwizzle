TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    swizzleUtility.cpp

INCLUDEPATH += dist
LIBS += -L../ImageSwizzle/dist/mingw -lFreeImagePlus

HEADERS += \
    swizzleUtility.h
