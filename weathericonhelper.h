#ifndef WEATHERICONHELPER_H
#define WEATHERICONHELPER_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QImage>
#include <QMap>
#include <QFile>

class WeatherIconHelper
{
public:
    static QString getPictureMeaning(QString picName);
    static QString getPicture(QString picName);
private:
    static QMap<QString,QString> *nameMeanningMap;
};

/*
 *          图片名        说  明
======================================
          0.gif          晴
          1.gif          多云
          2.gif          阴
          3.gif          阵雨
          4.gif          雷阵雨
          5.gif          雷阵雨并伴有冰雹
          6.gif          雨加雪
          7.gif          小雨
          8.gif          中雨
          9.gif          大雨
          10.gif         暴雨
          11.gif         大暴雨
          12.gif         特大暴雨
          13.gif         阵雪
          14.gif         小雪
          15.gif         中雪
          16.gif         大雪
          17.gif         暴雪
          18.gif         雾
          19.gif         冻雨
          20.gif         沙尘暴
          21.gif         小雨-中雨
          22.gif         中雨-大雨
          23.gif         大雨-暴雨
          24.gif         暴雨-大暴雨
          25.gif         大暴雨-特大暴雨
          26.gif         小雪-中雪
          27.gif         中雪-大雪
          28.gif         大雪-暴雪
          29.gif         浮尘
          30.gif         扬沙
          31.gif         强沙尘暴
          nothing.gif    没有数据(可自己更换)
 */

#endif // WEATHERICONHELPER_H
