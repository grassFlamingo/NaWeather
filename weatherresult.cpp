#include "weatherresult.h"

WeatherModel::WeatherModel(QObject *parent)
    : QObject(parent){

}

WeatherModel::~WeatherModel(){

}

WeatherModel::WeatherModel(
        QString temperature,
        QString situation,
        QString wind,
        QString picFirst,
        QString picSecond){
    this->Temperature = temperature;
    this->Situation = situation;
    this->Wind = wind;
    this->PicFirst = picFirst;
    this->PicSecond = picSecond;
}

QString WeatherModel::getShowPicHtml(){
    return QString("<a href = '#' >"
                   "<img src='%1' width='35' height='35' />"
                   "<img src='%2' width='35' height='35' />"
                   "</a>")
            .arg(WeatherIconHelper::getPicture(this->PicFirst))
            .arg(WeatherIconHelper::getPicture(this->PicSecond));
}

WeatherDetailsModel::WeatherDetailsModel(QString detail,QObject *parent)
    : QObject(parent){
    map = new QMap<QString,QString*>();
    map->insert("气温", &Temperature);
    map->insert("风向/风力", &Wind);
    map->insert("湿度", &Humidity);
    map->insert("紫外线强度", &UVRadiationIntensity);
    map->insert("空气质量", &AirQuality);
    setData(detail);
}

WeatherDetailsModel::~WeatherDetailsModel(){
    delete map;
}

void WeatherDetailsModel::setData(QString detail){
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
    if(NULL == detail || detail.isEmpty()){
        return;
    }
    QRegExp rx("：");
    int pos = rx.indexIn(detail);
    QRegExp re("([\u4e00-\u9fa5/]+)(：)([^。；：;]+)");
    QString* stringPointer;
    while ((pos = re.indexIn(detail, pos)) != -1) {
        stringPointer = map->value(re.cap(1));
        if(NULL != stringPointer){
            *stringPointer = re.cap(3);
        }
        pos += re.matchedLength();
    }
}

WeatherAdvicesModel::WeatherAdvicesModel(QString detail, QObject *parent)
    : QObject(parent){
    map = new QMap<QString,QString*>();
    map->insert("紫外线指数", &UVRadition);
    map->insert("感冒指数", &Cold);
    map->insert("穿衣指数", &Wear);
    map->insert("运动指数", &Sport);
    map->insert("洗车指数", &WashCar);
    map->insert("空气污染指数", &AirPolution);
    setData(detail);
}

WeatherAdvicesModel::~WeatherAdvicesModel(){
    delete map;
}

void WeatherAdvicesModel::setData(QString details){
    /* 紫外线指数：最弱，辐射弱，涂擦SPF8-12防晒护肤品。
     * 感冒指数：较易发，天凉温差大，适当增减衣服。
     * 穿衣指数：较冷，建议着厚外套加毛衣等服装。
     * 洗车指数：较适宜，无雨且风力较小，易保持清洁度。
     * 运动指数：较不宜，天气寒冷，推荐您进行室内运动。
     * 空气污染指数：良，气象条件有利于空气污染物扩散。
     */
    if(NULL == details || details.isEmpty()){
        return;
    }
    QRegExp re("([\u4e00-\u9fa5]+)(：)([^。\\s]+)");
    int pos = 0;
    QString * stringPointer;
    while ((pos = re.indexIn(details, pos)) != -1) {
        stringPointer = map->value(re.cap(1));
        if(NULL != stringPointer){
            *stringPointer = re.cap(3);
        }
        pos += re.matchedLength();
    }
}

QString WeatherAdvicesModel::getLabelText(QString &origen){
    return origen.replace("，", " ");
}

QString WeatherAdvicesModel::getLabelToolTip(QString &origen){
    return origen.replace("，", "<br/>");
}


WeatherResult::WeatherResult(int areaCode, QObject *parent)
    : QObject(parent){

    AreaCode = areaCode;
    Today = new WeatherModel();
    Tomorrow = new WeatherModel();
    DayAfterTomorrow = new WeatherModel();
    TodayDetails = new WeatherDetailsModel();
    TodayAdvices = new WeatherAdvicesModel();
    NetWork = new QNetworkAccessManager(this);
    connect(NetWork, SIGNAL(finished(QNetworkReply*)), this, SLOT(netWorkAccessedFinishedSlot(QNetworkReply*)) );
}

WeatherResult::~WeatherResult(){
    if(this->Today){
        Today->deleteLater();
    }
    if(this->Tomorrow){
        Tomorrow->deleteLater();
    }
    if(this->DayAfterTomorrow){
        DayAfterTomorrow->deleteLater();
    }
    if(this->TodayDetails){
        TodayDetails->deleteLater();
    }
    if(this->TodayAdvices){
        TodayAdvices->deleteLater();
    }
    NetWork->deleteLater();
}

void WeatherResult::netWorkAccessedFinishedSlot(QNetworkReply *reply){
    if(reply->error() != QNetworkReply::NoError){
        reply->close();
        readFromDataBase();
        emit dataErrorSignal();
        return;
    }
    QByteArray xml = reply->readAll();
    reply->close();
    QDomDocument doc;
    //bool QDomDocument::setContent(const QByteArray &data, bool namespaceProcessing, QString *errorMsg = Q_NULLPTR, int *errorLine = Q_NULLPTR, int *errorColumn = Q_NULLPTR)
    doc.setContent(xml,false);
    QDomElement docElement = doc.documentElement();
    QDomNodeList childNodes = docElement.childNodes();
    /* 省份，城市，城市代码，城市图片名称 0-4
     * <string>直辖市</string>
     * <string>上海</string>
     * <string>58367</string>
     * <string>58367.jpg</string>
     * <string>2017-3-6 14:30:08</string>
     */
    Province = childNodes.at(0).toElement().text();
    City = childNodes.at(1).toElement().text();
    AreaCode = childNodes.at(2).toElement().text().toInt();

    if(AreaCode == 0 ){
        readFromDataBase();
        emit dataErrorSignal();
        return;
    }
    AreaPicture = childNodes.at(3).toElement().text();
    LastUpdateTime = QDateTime::fromString(
                childNodes.at(4).toElement().text(),
                "yyyy-M-d h:m:ss");
    /* 当天的 气温，概况，风向和风力，天气趋势开始图片名称 5-11
     * <string>4℃/12℃</string>
     * <string>3月6日 多云</string>
     * <string>东北风3-4级转微风</string>
     * <string>1.gif</string>
     * <string>1.gif</string>
     */
    Today->Temperature = childNodes.at(5).toElement().text();
    Today->Situation = childNodes.at(6).toElement().text();
    Today->Wind = childNodes.at(7).toElement().text();
    Today->PicFirst = childNodes.at(8).toElement().text();
    Today->PicSecond = childNodes.at(9).toElement().text();

    /*<string>今日天气实况：气温：10℃；
     * 风向/风力：北风 1级；
     * 湿度：30%；
     * 紫外线强度：最弱。
     * 空气质量：良。
     * </string>
     * <string>
     * 紫外线指数：最弱，辐射弱，涂擦SPF8-12防晒护肤品。
     * 感冒指数：较易发，天凉温差大，适当增减衣服。
     * 穿衣指数：较冷，建议着厚外套加毛衣等服装。
     * 洗车指数：较适宜，无雨且风力较小，易保持清洁度。
     * 运动指数：较不宜，天气寒冷，推荐您进行室内运动。
     * 空气污染指数：良，气象条件有利于空气污染物扩散。
     * </string>
     */
    TodayDetails->setData(childNodes.at(10).toElement().text());
    TodayAdvices->setData(childNodes.at(11).toElement().text());
    /* 第二天的 气温，概况，风向和风力，图标一，图标二。 12-16
     * <string>5℃/14℃</string>
     * <string>3月7日 多云</string>
     * <string>西风微风</string>
     * <string>1.gif</string>
     * <string>1.gif</string>
     */
    Tomorrow->Temperature = childNodes.at(12).toElement().text();
    Tomorrow->Situation = childNodes.at(13).toElement().text();
    Tomorrow->Wind = childNodes.at(14).toElement().text();
    Tomorrow->PicFirst = childNodes.at(15).toElement().text();
    Tomorrow->PicSecond = childNodes.at(16).toElement().text();

    /*第三天的 气温，概况，风向和风力，图标一，图标二。 17-21
     * <string>6℃/15℃</string>
     * <string>3月8日 晴</string>
     * <string>西北风微风</string>
     * <string>0.gif</string>
     * <string>0.gif</string>
     */
    DayAfterTomorrow->Temperature = childNodes.at(17).toElement().text();
    DayAfterTomorrow->Situation = childNodes.at(18).toElement().text();
    DayAfterTomorrow->Wind = childNodes.at(19).toElement().text();
    DayAfterTomorrow->PicFirst = childNodes.at(20).toElement().text();
    DayAfterTomorrow->PicSecond = childNodes.at(21).toElement().text();
    /* 被查询的城市或地区的介绍 22
     * <string>
     * 上海简称：沪，位置：上海地处长江三角洲前缘，东濒东海，南临杭州湾，西接江苏，浙江两省，北界长江入海，正当我国南北岸线的中部，北纬31°14′，东经121°29′。面积：总面积7823.5平方公里。人口：人口1000多万。上海丰富的人文资源、迷人的城市风貌、繁华的商业街市和欢乐的节庆活动形成了独特的都市景观。游览上海，不仅能体验到大都市中西合壁、商儒交融、八方来风的氛围，而且能感受到这个城市人流熙攘、车水马龙、灯火璀璨的活力。上海在中国现代史上占有着十分重要的地位，她是中国共产党的诞生地。许多震动中外的历史事件在这里发生，留下了众多的革命遗迹，处处为您讲述着一个个使人永不忘怀的可歌可泣的故事，成为包含民俗的人文景观和纪念地。在上海，每到秋祭，纷至沓来的人们在这里祭祀先烈、缅怀革命历史,已成为了一种风俗。大上海在中国近代历史中，曾是风起云涌可歌可泣的地方。在这里荟萃多少风云人物，散落在上海各处的不同住宅建筑，由于其主人的非同寻常，蕴含了耐人寻味的历史意义。这里曾留下许多革命先烈的足迹。瞻仰孙中山、宋庆龄、鲁迅等故居，会使您产生抚今追昔的深沉遐思，这里还有无数个达官贵人的住宅，探访一下李鸿章、蒋介石等人的公馆，可以联想起主人那段显赫的发迹史。
     * </string>
     */
    Discripe = childNodes.at(22).toElement().text();
    Discripe = Discripe.replace("。","。<br/>");
    emit dataCompleteSignal();
    saveToDataBase();
}

void WeatherResult::saveToDataBase(){
    QSqlDatabase::database().open();
    QSqlQuery query;
    query.prepare("INSERT INTO WeatherInfo "
                  "( Province, "
                  "City, "
                  "AreaCode, "
                  "LastUpdateTime, "
                  "TodayTemperature, "
                  "TodaySituation, "
                  "TodayWind, "
                  "TodayPicFirst, "
                  "TodayPicSecond, "
                  "TomorrowTemperature, "
                  "TomorrowSituation, "
                  "TomorrowWind, "
                  "TomorrowPicFirst, "
                  "TomorrowPicSecond, "
                  "DayAfterTomorrowTemperature, "
                  "DayAfterTomorrowSituation, "
                  "DayAfterTomorrowWind, "
                  "DayAfterTomorrowPicFirst, "
                  "DayAfterTomorrowPicSecond, "
                  "DetailsTemperature, "
                  "DetailsWind, "
                  "DetailsHumidity, "
                  "DetailsUVRadiationIntensity, "
                  "DetailsAirQuality, "
                  "AdvicesUVRadition, "
                  "AdvicesCold, "
                  "AdvicesWear, "
                  "AdvicesWashCar, "
                  "AdvicesSport, "
                  "AdvicesAirPolution ) "
                  "VALUES ( :Province, "
                  ":City, "
                  ":AreaCode, "
                  ":LastUpdateTime, "
                  ":TodayTemperature, "
                  ":TodaySituation, "
                  ":TodayWind, "
                  ":TodayPicFirst, "
                  ":TodayPicSecond, "
                  ":TomorrowTemperature, "
                  ":TomorrowSituation, "
                  ":TomorrowWind, "
                  ":TomorrowPicFirst, "
                  ":TomorrowPicSecond, "
                  ":DayAfterTomorrowTemperature, "
                  ":DayAfterTomorrowSituation, "
                  ":DayAfterTomorrowWind, "
                  ":DayAfterTomorrowPicFirst, "
                  ":DayAfterTomorrowPicSecond, "
                  ":DetailsTemperature, "
                  ":DetailsWind, "
                  ":DetailsHumidity, "
                  ":DetailsUVRadiationIntensity, "
                  ":DetailsAirQuality, "
                  ":AdvicesUVRadition, "
                  ":AdvicesCold, "
                  ":AdvicesWear, "
                  ":AdvicesWashCar, "
                  ":AdvicesSport, "
                  ":AdvicesAirPolution )");
    query.bindValue(":Province", Province);
    query.bindValue(":City",City);;
    query.bindValue(":AreaCode",AreaCode);
    query.bindValue(":LastUpdateTime", LastUpdateTime);
    query.bindValue(":TodayTemperature", Today->Temperature);
    query.bindValue(":TodaySituation", Today->Situation);
    query.bindValue(":TodayWind", Today->Wind);
    query.bindValue(":TodayPicFirst", Today->PicFirst);
    query.bindValue(":TodayPicSecond", Today->PicSecond);
    query.bindValue(":TomorrowTemperature",Tomorrow->Temperature);
    query.bindValue(":TomorrowSituation", Tomorrow->Situation);
    query.bindValue(":TomorrowWind", Tomorrow->Wind);
    query.bindValue(":TomorrowPicFirst", Tomorrow->PicFirst);
    query.bindValue(":TomorrowPicSecond", Tomorrow->PicSecond);
    query.bindValue(":DayAfterTomorrowTemperature", DayAfterTomorrow->Temperature);
    query.bindValue(":DayAfterTomorrowSituation", DayAfterTomorrow->Situation);
    query.bindValue(":DayAfterTomorrowWind", DayAfterTomorrow->Wind);
    query.bindValue(":DayAfterTomorrowPicFirst", DayAfterTomorrow->PicFirst);
    query.bindValue(":DayAfterTomorrowPicSecond", DayAfterTomorrow->PicSecond);
    query.bindValue(":DetailsTemperature", TodayDetails->Temperature);
    query.bindValue(":DetailsWind", TodayDetails->Wind);
    query.bindValue(":DetailsHumidity", TodayDetails->Humidity);
    query.bindValue(":DetailsUVRadiationIntensity", TodayDetails->UVRadiationIntensity);
    query.bindValue(":DetailsAirQuality", TodayDetails->AirQuality);
    query.bindValue(":AdvicesUVRadition", TodayAdvices->UVRadition);
    query.bindValue(":AdvicesCold", TodayAdvices->Cold);
    query.bindValue(":AdvicesWear", TodayAdvices->Wear);
    query.bindValue(":AdvicesWashCar", TodayAdvices->WashCar);
    query.bindValue(":AdvicesSport", TodayAdvices->Sport);
    query.bindValue(":AdvicesAirPolution", TodayAdvices->AirPolution);
    query.exec();

    query.prepare("UPDATE Area SET "
                  "Discripe = ? WHERE AreaCode = ? ");
    query.bindValue(0, Discripe);
    query.bindValue(1,AreaCode);
    query.exec();
    QSqlDatabase::database().close();
}

void WeatherResult::readFromDataBase(){
    QSqlDatabase::database().open();
    QSqlQuery query;
    query.exec("SELECT * FROM WeatherInfo JOIN Area "
               "ON Area.AreaCode = WeatherInfo.AreaCode "
               "ORDER BY ID DESC LIMIT 1");
    if(query.next()){
        Province = query.value("Province").toString();
        City = query.value("City").toString();
        AreaCode = query.value("AreaCode").toInt();
        LastUpdateTime = query.value("LastUpdateTime").toDateTime();
        Today->Temperature = query.value("TodayTemperature").toString();
        Today->Situation = query.value("TodaySituation").toString();
        Today->Wind = query.value("TodayWind").toString();
        Today->PicFirst = query.value("TodayPicFirst").toString();
        Today->PicSecond = query.value("TodayPicSecond").toString();
        Tomorrow->Temperature = query.value("TomorrowTemperature").toString();
        Tomorrow->Situation = query.value("TomorrowSituation").toString();
        Tomorrow->Wind = query.value("TomorrowWind").toString();
        Tomorrow->PicFirst = query.value("TomorrowPicFirst").toString();
        Tomorrow->PicSecond = query.value("TomorrowPicSecond").toString();
        DayAfterTomorrow->Temperature = query.value("DayAfterTomorrowTemperature").toString();
        DayAfterTomorrow->Situation = query.value("DayAfterTomorrowSituation").toString();
        DayAfterTomorrow->Wind = query.value("DayAfterTomorrowWind").toString();
        DayAfterTomorrow->PicFirst = query.value("DayAfterTomorrowPicFirst").toString();
        DayAfterTomorrow->PicSecond = query.value("DayAfterTomorrowPicSecond").toString();
        TodayDetails->Temperature = query.value("DetailsTemperature").toString();
        TodayDetails->Wind = query.value("DetailsWind").toString();
        TodayDetails->Humidity = query.value("DetailsHumidity").toString();
        TodayDetails->UVRadiationIntensity = query.value("DetailsUVRadiationIntensity").toString();
        TodayDetails->AirQuality = query.value("DetailsAirQuality").toString();
        TodayAdvices->UVRadition = query.value("AdvicesUVRadition").toString();
        TodayAdvices->Cold = query.value("AdvicesCold").toString();
        TodayAdvices->Wear = query.value("AdvicesWear").toString();
        TodayAdvices->WashCar = query.value("AdvicesWashCar").toString();
        TodayAdvices->Sport = query.value("AdvicesSport").toString();
        TodayAdvices->AirPolution = query.value("AdvicesAirPolution").toString();
        Discripe = query.value("Discripe").toString();
    }

    QSqlDatabase::database().close();
}

void WeatherResult::getWeatherData(){
    if(AreaCode == -1){
        return;
    }
    NetWork->get(QNetworkRequest(QUrl(getWeatherLink())));
//    NetWork->get(QNetworkRequest(QUrl("qrc:/xml/scrpis/weather.xml")));
}

QString WeatherResult::getWeatherLink(){
    return QString("http://www.webxml.com.cn/WebServices/WeatherWebService.asmx/getWeatherbyCityName?theCityName=%1").arg(this->AreaCode);
}

void WeatherResult::setAreaCode(int areaCode){
    AreaCode = areaCode;
}

QString WeatherResult::getCity(){
    return City;
}

QString WeatherResult::getDiscripe(){
    return Discripe;
}

WeatherDetailsModel* WeatherResult::getTodayDetails(){
    return TodayDetails;
}

WeatherAdvicesModel* WeatherResult::getTodayAdvices(){
    return TodayAdvices;
}

WeatherModel* WeatherResult::getToday(){
    return Today;
}

WeatherModel* WeatherResult::getTomorrow(){
    return Tomorrow;
}

WeatherModel* WeatherResult::getDayAfterTomorrow(){
    return DayAfterTomorrow;
}

QDateTime WeatherResult::getLastUpdateTime(){
    return LastUpdateTime;
}
