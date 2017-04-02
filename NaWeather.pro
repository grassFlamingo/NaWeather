#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T23:15:50
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NaWeather
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    areamodel.cpp \
    networkerror.cpp \
    setingweather.cpp \
    settingparameter.cpp \
    showresult.cpp \
    weathericonhelper.cpp \
    weathermssageframe.cpp \
    weatherresult.cpp

HEADERS  += mainwindow.h \
    areamodel.h \
    networkerror.h \
    setingweather.h \
    settingparameter.h \
    showresult.h \
    weathericonhelper.h \
    weathermssageframe.h \
    weatherresult.h

FORMS    += mainwindow.ui \
    networkerror.ui \
    setingweather.ui \
    showresult.ui \
    weathermssageframe.ui

RESOURCES += \
    sources.qrc

DISTFILES += \
    scrpis/Air.png \
    scrpis/AirQuality.png \
    scrpis/close.png \
    scrpis/Cold.png \
    scrpis/Humidity.png \
    scrpis/menu.png \
    scrpis/move.png \
    scrpis/Sport.png \
    scrpis/Temperature.png \
    scrpis/UVRadiationIntensity.png \
    scrpis/WashCar.png \
    scrpis/Wear.png \
    scrpis/Wind.png \
    weatherIcon/0.png \
    weatherIcon/1.png \
    weatherIcon/10.png \
    weatherIcon/11.png \
    weatherIcon/12.png \
    weatherIcon/13.png \
    weatherIcon/14.png \
    weatherIcon/15.png \
    weatherIcon/16.png \
    weatherIcon/17.png \
    weatherIcon/18.png \
    weatherIcon/19.png \
    weatherIcon/2.png \
    weatherIcon/20.png \
    weatherIcon/21.png \
    weatherIcon/22.png \
    weatherIcon/23.png \
    weatherIcon/24.png \
    weatherIcon/25.png \
    weatherIcon/26.png \
    weatherIcon/27.png \
    weatherIcon/28.png \
    weatherIcon/29.png \
    weatherIcon/3.png \
    weatherIcon/31.png \
    weatherIcon/4.png \
    weatherIcon/5.png \
    weatherIcon/6.png \
    weatherIcon/7.png \
    weatherIcon/8.png \
    weatherIcon/9.png \
    weatherIcon/nothing.png \
    Humidity.png \
    icon.ico \
    Weather.db

RC_ICONS += icon.ico
