#include "settingparameter.h"


QMap<QString, DataStateRec>* SettingParameter::dataMap = NULL;

DataStateRec::DataStateRec(QVariant data, DataOperation operation, QObject *parent)
    : QObject(parent)
{
    this->data = data;
    this->operation = operation;
}

DataStateRec::DataStateRec(QObject *parent)
    :QObject(parent)
{

}
DataStateRec::DataStateRec(const DataStateRec &other)
    : QObject(other.parent()){
    this->data = other.data;
    this->operation = other.operation;
}

void DataStateRec::operator = (DataStateRec other){
    this->data = other.data;
    this->operation = other.operation;
}

SettingParameter::SettingParameter(QObject *parent) : QObject(parent)
{
    loadDataFromDataBase();
}

void SettingParameter::loadDataFromDataBase(){
    if(!dataMap){
        dataMap = new QMap<QString, DataStateRec>();
    }else{
        dataMap->clear();
    }
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.exec("SELECT Parameter, Value FROM SettingData");
    while(query.next()){
        dataMap->insert(query.value("Parameter").toString(),
                        DataStateRec(query.value("Value")));
    }
    db.close();
}

QVariant SettingParameter::getData(QString parameterName){
    if(!dataMap){
        loadDataFromDataBase();
    }
    return dataMap->value(parameterName).data;
}

void SettingParameter::setData(QString parameterName, QVariant data){
    if(!dataMap){
        loadDataFromDataBase();
    }
    if(dataMap->value(parameterName).data.isValid()){
        //If there is already an item with the key key,
        //that item's value is replaced with value.
        dataMap->insert(parameterName, DataStateRec(data, Update));
    }
    else{
        dataMap->insert(parameterName, DataStateRec(data, Insert));
    }
}

bool SettingParameter::saveToDataBase(){
    if(!dataMap){
        return false;
    }
    QSqlDatabase::database().open();
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    {
        //先判断该数据库是否支持事务操作
        if(QSqlDatabase::database().transaction()) //启动事务操作
        {
            //下面执行各种数据库操作
            QSqlQuery query;
            QMap<QString, DataStateRec>::iterator i;
            for (i = dataMap->begin(); i != dataMap->end(); i++){
                if(i.value().operation == Insert){
                    query.prepare("INSERT INTO SettingData ( Parameter, Value ) "
                                  " VALUES ( ?, ? ) ");
                    query.bindValue(0, i.key());
                    query.bindValue(1, i.value().data);
                    query.exec();
                }
                else if(i.value().operation == Update ){
                    query.prepare("UPDATE SettingData SET "
                                  "Value = :Value WHERE Parameter = :Parameter");
                    query.bindValue(":Value", i.value().data);
                    query.bindValue(":Parameter", i.key());
                    query.exec();
                }else{
                    continue;
                }
//                qDebug()<<query.lastError() << i.value().data << i.value().operation << i.key();
            }
            if(!QSqlDatabase::database().commit())
            {
                QSqlDatabase::database().rollback();
                QSqlDatabase::database().close();
                return false;
            }
        }
    }
    QSqlDatabase::database().close();
    return true;
}
