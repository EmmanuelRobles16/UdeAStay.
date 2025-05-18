TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QT += core

SOURCES += \
        alojamiento.cpp \
        anfitrion.cpp \
        archivos.cpp \
        fecha.cpp \
        huesped.cpp \
        main.cpp \
        plataforma.cpp \
        reservacion.cpp

HEADERS += \
    alojamiento.h \
    anfitrion.h \
    archivos.h \
    fecha.h \
    huesped.h \
    plataforma.h \
    reservacion.h
