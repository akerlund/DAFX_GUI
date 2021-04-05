QT += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    biquad.cpp \
    cmdline.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    qplot.cpp \
    serial.cpp \
    vbytearray.cpp \
    mixchannel.cpp \
    main.cpp \
    mixer.cpp \
    filter.cpp \
    console.cpp

HEADERS += \
    biquad.h \
    cmdline.h \
    qcustomplot.h \
    qplot.h \
    serial.h \
    vbytearray.h \
    qhost_defines.h \
    mixchannel.h \
    mainwindow.h \
    mixer.h \
    filter.h \
    console.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
