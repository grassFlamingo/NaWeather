#ifndef SETINGWEATHER_H
#define SETINGWEATHER_H

#include <QDialog>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QHashData>
#include <QColorDialog>
#include <QToolTip>
#include <QCursor>

#include "areamodel.h"
#include "weatherresult.h"
#include "settingparameter.h"

namespace Ui {
class SetingWeather;
}

class ZoneModel : public QObject{
public:
    ZoneModel(QObject *parent = 0);
    ~ZoneModel();
    ZoneModel(int ID, QString Zone, QObject *parent = 0);

    int ID;
    QString Zone;
};


class SetingWeather : public QDialog
{
    Q_OBJECT

public:
    explicit SetingWeather(QDialog *parent = 0);
    ~SetingWeather();
    void setData();

    //http://www.webxml.com.cn/WebServices/WeatherWS.asmx/getRegionCountry
private:
    Ui::SetingWeather *ui;

    QList<ZoneModel *> zones;
    QList<AreaModel *> areas;
    QColor Color;
    int AreaCode;
    float Opacity;
    QString City;
    QString ZoneName;
    bool Transparent;

    void getDataFromDataBase();
private slots:
    void zoneComboBoxCurrentIndexChangedSlot(int index);
    void areaComboBoxCurrentIndexChangedSlot(int index);

    void viewChooseButtonClickedSlot();
    void viewWindowOpacityValueChangedSlot(int value);

    void yesButtonClickedSlot();


signals:
    void areaDataSeted(int AreaCode, QColor color, float opacity, bool transparent );
};


#endif // SETINGWEATHER_H
