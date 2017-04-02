#ifndef SETTINGPARAMETER_H
#define SETTINGPARAMETER_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDebug>
#include <QVariant>
#include <QMap>

enum DataOperation{
    Insert,
    Update,
    NotChange
};

class DataStateRec : public QObject{
    Q_OBJECT
public:
    DataStateRec(QVariant data, DataOperation operation = NotChange, QObject *parent = 0);
    DataStateRec(QObject *parent = 0);
    DataStateRec(const DataStateRec &other);

    void operator = (DataStateRec other);

    QVariant data;
    DataOperation operation;
};

class SettingParameter : public QObject
{
    Q_OBJECT
public:
    explicit SettingParameter(QObject *parent = 0);

    static QVariant getData(QString parameterName);
    static void setData(QString parameterName, QVariant data);
    static bool saveToDataBase();

signals:

public slots:

private:
    static void loadDataFromDataBase();
    static QMap<QString, DataStateRec> *dataMap;

};

#endif // SETTINGPARAMETER_H
