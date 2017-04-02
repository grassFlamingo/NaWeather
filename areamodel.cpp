#include "areamodel.h"

AreaModel::AreaModel(QObject *parent) :
    QObject(parent)
{

}

AreaModel::AreaModel(int id, int zoneId, QString area, int areaCode, QObject *parent )
    : QObject(parent){
    this->ID = id;
    this->ZoneID = zoneId;
    this->Area = area;
    this->AreaCode = areaCode;
}
