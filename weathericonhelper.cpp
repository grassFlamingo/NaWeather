#include "weathericonhelper.h"

QMap<QString,QString>* WeatherIconHelper::nameMeanningMap = NULL;


QString WeatherIconHelper::getPictureMeaning(QString picName){
    if(NULL == nameMeanningMap){
        nameMeanningMap = new QMap<QString,QString>();
        QSqlDatabase db = QSqlDatabase::database();
        db.open();
        QSqlQuery query;
        query.exec("SELECT PictureName, ChineseMeanning "
                   "FROM WeatherMeanning");
        while(query.next()){
            nameMeanningMap->insert(query.value(0).toString(), query.value(1).toString());
        }
        db.close();
    }

    return nameMeanningMap->value(picName);
}

QString WeatherIconHelper::getPicture(QString picName){
    return ":/icon/weatherIcon/"+picName.replace(".gif",".png");;
}


