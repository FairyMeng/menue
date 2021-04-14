#-------------------------------------------------
#
# Project created by QtCreator 2017-08-03T09:55:24
#
#-------------------------------------------------

QT       += core gui charts
QT       += widgets
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION,4): QT += widgets printsupport



TARGET = menue
TEMPLATE = app


SOURCES += main.cpp\
    message_cancel.cpp \
    sequencefrequency.cpp \
    radarperform.cpp \
    datafit.cpp \
    piontlinemeasuring.cpp \
    shuzhijisuan1.cpp \
    prijingce.cpp \
    maijianjujingce.cpp \
    maichongcunchu.cpp \
    guolvshezhi2.cpp \
    guolvshezhi.cpp \
    shujuchuli.cpp \
    caogaoben.cpp \
    imcontrast.cpp \
    matchsearch.cpp \
    xvliepingpufenxi.cpp \
    mainwindow.cpp \
    impulse_clustering.cpp \
    chart.cpp \
    scene.cpp \
    view.cpp \
    databaseconnector.cpp \
    databaseoperator.cpp \
    pulsegraphoperator.cpp \
    pulsefilemanager.cpp \
    pulsegraphdisplayer.cpp \
    pulsemapper.cpp \
    signalworker.cpp \
    slotworker.cpp \
    basewindow.cpp \
    mytitlebar.cpp \
    internalpulseview.cpp \
    internalpulsechart.cpp \
    pulsewindow.cpp \
    internalpulsewindow.cpp \
    mywindow.cpp \
    impulse_second_form.cpp



HEADERS  += \
    message_cancel.h \
    sequencefrequency.h \
    radarperform.h \
    datafit.h \
    piontlinemeasuring.h \
    shuzhijisuan1.h \
    shujuchuli.h \
    prijingce.h \
    maijianjujingce.h \
    maichongcunchu.h \
    guolvshezhi2.h \
    guolvshezhi.h \
    caogaoben.h \
    imcontrast.h \
    matchsearch.h \
    xvliepingpufenxi.h \
    mainwindow.h \
    impulse_clustering.h \
    chart.h \
    scene.h \
    view.h \
    databaseconnector.h \
    databaseoperator.h \
    structdefine.h \
    pulsegraphoperator.h \
    pulsefilemanager.h \
    pulsegraphdisplayer.h \
    pulsemapper.h \
    signalworker.h \
    slotworker.h \
    mytitlebar.h \
    basewindow.h \
    internalpulseview.h \
    internalpulsechart.h \
    pulsewindow.h \
    internalpulsewindow.h \
    ui_mainwindow.h \
    mywindow.h \
    impulse_second_form.h


FORMS    += \
    message_cancel.ui \
    sequencefrequency.ui \
    radarperform.ui \
    datafit.ui \
    piontlinemeasuring.ui \
    shuzhijisuan1.ui \
    shujuchuli.ui \
    prijingce.ui \
    maijianjujingce.ui \
    maichongcunchu.ui \
    guolvshezhi2.ui \
    guolvshezhi.ui \
    caogaoben.ui \
    imcontrast.ui \
    matchsearch.ui \
    xvliepingpufenxi.ui \
    impulse_clustering.ui \
    pulsewindow.ui \
    internalpulsewindow.ui \
    mainwindow.ui \
    mywindow.ui \
    impulse_second_form.ui


RESOURCES += \
    res.qrc \
    qss.qrc \
    titlebar.qrc
