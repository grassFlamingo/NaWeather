#ifndef AREAMODEL_H
#define AREAMODEL_H

#include <QString>
#include <QObject>

class AreaModel : public QObject
{
public:
    AreaModel(QObject *parent = 0);
    AreaModel(int id, int zoneId, QString area, int areaCode,QObject *parent = 0 );

    int ID;
    int ZoneID;
    QString Area;
    int AreaCode;
};

#endif // AREAMODEL_H
