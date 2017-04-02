#ifndef WEATHERRESULT_H
#define WEATHERRESULT_H

#include <Qstring>
#include <QStringList>
#include <QUrl>
#include <QMap>
#include <QDateTime>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QObject>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>
#include <QRegExp>
#include <QPicture>

#include "weathericonhelper.h"


class WeatherModel : public QObject{
public:
    explicit WeatherModel(QObject *parent = 0);
    ~WeatherModel();
    WeatherModel(QString temperature,
                 QString situation,
                 QString wind,
                 QString picFirst,
                 QString picSecond);
    QString getShowPicHtml();

    /*
     * <string>4℃/12℃</string>
     * <string>3月6日 多云</string>
     * <string>东北风3-4级转微风</string>
     * <string>1.gif</string>
     * <string>1.gif</string>
     */
    QString Temperature;
    QString Situation;
    QString Wind;
    QString PicFirst;
    QString PicSecond;
};

class WeatherDetailsModel : public QObject{
public:
    WeatherDetailsModel(QString detail = "",QObject *parent = 0);
    ~WeatherDetailsModel();

    void setData(QString detail);
    /*
     * <string>
     * 今日天气实况：
     * 气温：10℃；
     * 风向/风力：北风 1级；
     * 湿度：30%；
     * 紫外线强度：最弱。
     * 空气质量：良。
     * </string>
     */
    QString Temperature;
    QString Wind;
    QString Humidity;
    QString UVRadiationIntensity;
    QString AirQuality;
    QMap<QString,QString*> *map;
};


class WeatherAdvicesModel : public QObject{
public:
    WeatherAdvicesModel(QString detail = "", QObject *parent = 0);
    ~WeatherAdvicesModel();

    void setData(QString details);
    QString getLabelText(QString &origen);
    QString getLabelToolTip(QString &origen);

    QString UVRadition;
    QString Cold;
    QString Wear;
    QString WashCar;
    QString Sport;
    QString AirPolution;
    QMap<QString,QString*> *map;
};

class WeatherResult : public QObject
{
    Q_OBJECT

public:
    explicit WeatherResult(int areaCode = -1, QObject *parent = 0);
    ~WeatherResult();
    QString getWeatherLink();
    void getWeatherData();
    QString getDiscripe();
    QString getCity();
    QDateTime getLastUpdateTime();
    WeatherDetailsModel* getTodayDetails();
    WeatherAdvicesModel* getTodayAdvices();
    WeatherModel* getToday();
    WeatherModel* getTomorrow();
    WeatherModel* getDayAfterTomorrow();

    void setAreaCode(int areaCode);
    void saveToDataBase();
    void readFromDataBase();

private slots:
    void netWorkAccessedFinishedSlot(QNetworkReply* reply);

signals:
    void dataCompleteSignal();
    void dataErrorSignal();

private:
    QString Province;
    QString City;
    int AreaCode;
    QString AreaPicture;
    QDateTime LastUpdateTime;
    WeatherDetailsModel* TodayDetails;
    WeatherAdvicesModel* TodayAdvices;
    WeatherModel *Today;
    WeatherModel *Tomorrow;
    WeatherModel *DayAfterTomorrow;
    QString Discripe;

    QNetworkAccessManager *NetWork;
};
/*<string>直辖市</string>
<string>上海</string>
<string>58367</string>
<string>58367.jpg</string>
<string>2017-3-6 14:30:08</string>
<string>4℃/12℃</string>
<string>3月6日 多云</string>
<string>东北风3-4级转微风</string>
<string>1.gif</string>
<string>1.gif</string>
<string>
今日天气实况：
气温：10℃；
风向/风力：北风 1级；
湿度：30%；
紫外线强度：最弱。
空气质量：良。
</string>
<string>
紫外线指数：最弱，辐射弱，涂擦SPF8-12防晒护肤品。
感冒指数：较易发，天凉温差大，适当增减衣服。
穿衣指数：较冷，建议着厚外套加毛衣等服装。
洗车指数：较适宜，无雨且风力较小，易保持清洁度。
运动指数：较不宜，天气寒冷，推荐您进行室内运动。
空气污染指数：良，气象条件有利于空气污染物扩散。
</string>
<string>5℃/14℃</string>
<string>3月7日 多云</string>
<string>西风微风</string>
<string>1.gif</string>
<string>1.gif</string>
<string>6℃/15℃</string>
<string>3月8日 晴</string>
<string>西北风微风</string>
<string>0.gif</string>
<string>0.gif</string>
<string>
上海简称：沪，
位置：上海地处长江三角洲前缘，东濒东海，南临杭州湾，西接江苏，浙江两省，北界长江入海，正当我国南北岸线的中部，北纬31°14′，东经121°29′。
面积：总面积7823.5平方公里。
人口：人口1000多万。
上海丰富的人文资源、迷人的城市风貌、繁华的商业街市和欢乐的节庆活动形成了独特的都市景观。
游览上海，不仅能体验到大都市中西合壁、商儒交融、八方来风的氛围，而且能感受到这个城市人流熙攘、车水马龙、灯火璀璨的活力。
上海在中国现代史上占有着十分重要的地位，她是中国共产党的诞生地。
许多震动中外的历史事件在这里发生，留下了众多的革命遗迹，处处为您讲述着一个个使人永不忘怀的可歌可泣的故事，成为包含民俗的人文景观和纪念地。
在上海，每到秋祭，纷至沓来的人们在这里祭祀先烈、缅怀革命历史,已成为了一种风俗。大上海在中国近代历史中，曾是风起云涌可歌可泣的地方。
在这里荟萃多少风云人物，散落在上海各处的不同住宅建筑，由于其主人的非同寻常，蕴含了耐人寻味的历史意义。
这里曾留下许多革命先烈的足迹。瞻仰孙中山、宋庆龄、鲁迅等故居，会使您产生抚今追昔的深沉遐思，这里还有无数个达官贵人的住宅，探访一下李鸿章、蒋介石等人的公馆，可以联想起主人那段显赫的发迹史。
</string>
*/

/*
 * getWeatherbyCityName
 * 根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数
 * 调用方法如下：
 * 输入参数：    theCityName =
 * 城市中文名称(国外城市可用英文)或城市代码(不输入默认为上海市)，
 * 如：上海 或 58367，
 * 如有城市名称重复请使用城市代码查询
 * (可通过 getSupportCity 或 getSupportDataSet 获得)；
 * 返回数据： 一个一维数组 String(22)，
 * 共有23个元素。
 * String(0) 到 String(4)：
 * 省份，城市，城市代码，城市图片名称，最后更新时间。
 *
 * String(5) 到 String(11)：
 *
 * 当天的 气温，概况，风向和风力，天气趋势开始图片名称
 * (以下称：图标一)，天气趋势结束图片名称(以下称：图标二)，
 * 现在的天气实况，天气和生活指数。
 *
 * String(12) 到 String(16)：
 * 第二天的 气温，概况，风向和风力，图标一，图标二。
 *
 * String(17) 到 String(21)：
 * 第三天的 气温，概况，风向和风力，图标一，图标二。
 * String(22) 被查询的城市或地区的介绍
 */

#endif // WEATHERRESULT_H
