#include "setingweather.h"
#include "ui_setingweather.h"

ZoneModel::ZoneModel(QObject *parent)
    :QObject(parent){

}

ZoneModel::~ZoneModel(){

}

ZoneModel::ZoneModel(int ID, QString Zone, QObject *parent)
    :QObject(parent){
    this->ID = ID;
    this->Zone = Zone;
}


SetingWeather::SetingWeather(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SetingWeather)
{
    ui->setupUi(this);
    getDataFromDataBase();
    setData();
    foreach( ZoneModel* zoneModel, this->zones ){
        ui->ZoneBox->addItem(zoneModel->Zone,zoneModel->ID);
    }
    connect(ui->ZoneBox, SIGNAL(currentIndexChanged(int)), this, SLOT(zoneComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->AreaBox, SIGNAL(currentIndexChanged(int)), this, SLOT(areaComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->ViewChoseColorButton, SIGNAL(clicked()), this, SLOT(viewChooseButtonClickedSlot()));
    connect( ui->WIndowOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(viewWindowOpacityValueChangedSlot(int)));
    connect(ui->YesButton, SIGNAL(clicked()), this, SLOT(yesButtonClickedSlot()));
}

SetingWeather::~SetingWeather()
{
    delete ui;
    foreach (ZoneModel* zoneModel, this->zones) {
        zoneModel->deleteLater();
    }
    foreach (AreaModel* model, areas) {
        model->deleteLater();
    }
}

void SetingWeather::getDataFromDataBase(){
    QSqlDatabase database = QSqlDatabase::database();
    database.open();

    QSqlQuery query;

    query.exec("SELECT ID,Zone FROM Zone");
    while(query.next()){
        zones.append(
                    new ZoneModel(
                        query.value(0).toInt(),
                        query.value(1).toString()
                        )
                    );
    }
    query.clear();


    query.exec("SELECT ID, ZoneID, Area, AreaCode FROM Area");
    //int id, QString zoneId, QString area, int AreaCode
    while(query.next()){
        areas.append(
                    new AreaModel(
                        query.value(0).toInt(),
                        query.value(1).toInt(),
                        query.value(2).toString(),
                        query.value(3).toInt()
                        )
                    );
    }
    query.clear();
    database.close();
}

void SetingWeather::zoneComboBoxCurrentIndexChangedSlot(int index){
    int zoneID = ui->ZoneBox->itemData(index).toInt();
    ui->AreaBox->clear();
    foreach (AreaModel* model, areas) {
        if(model->ZoneID == zoneID){
            ui->AreaBox->addItem(model->Area, model->AreaCode);
        }
    }
}

void SetingWeather::areaComboBoxCurrentIndexChangedSlot(int index){
    AreaCode = ui->AreaBox->itemData(index).toInt();
}

void SetingWeather::setData(){
    AreaCode = SettingParameter::getData("AreaCode").toInt();
    Color = QColor(SettingParameter::getData("Color").toString());
    Opacity = SettingParameter::getData("Opacity").toFloat();
    Transparent = SettingParameter::getData("Transparent").toBool();
    City = SettingParameter::getData("City").toString();
    ZoneName = SettingParameter::getData("ZoneName").toString();
    ui->transparentBox->setChecked(Transparent);
    ui->ZoneBox->setCurrentText(ZoneName);
    ui->AreaBox->setCurrentText(City);
    ui->AreaSelectedCityLabel->setText(City);
    ui->WIndowSliderValueLabel->setNum(int(Opacity*100));
    ui->WIndowOpacitySlider->setValue(int(Opacity*100));
    ui->NowColorLabel->setStyleSheet(QString("background: %1 ;").arg(Color.name()));
}

void SetingWeather::viewChooseButtonClickedSlot(){
    QColorDialog diag;
    diag.setCurrentColor(Color);
    diag.exec();
    Color = diag.selectedColor();
    ui->SelectedColorLabel->setStyleSheet(QString("background: %1 ;").arg(Color.name()));
}

void SetingWeather::viewWindowOpacityValueChangedSlot(int value){
    QToolTip::showText(QCursor::pos(),QString::number(value));
    Opacity = value * 0.01;
}

void SetingWeather::yesButtonClickedSlot(){
    Transparent = ui->transparentBox->isChecked();
    emit areaDataSeted(AreaCode, Color, Opacity, Transparent);
    City = ui->AreaBox->currentText();
    ZoneName = ui->ZoneBox->currentText();
    SettingParameter::setData("Opacity", Opacity);
    SettingParameter::setData("Color", Color.name());
    SettingParameter::setData("AreaCode", AreaCode);
    SettingParameter::setData("Transparent", Transparent);
    SettingParameter::setData("City",City);
    SettingParameter::setData("ZoneName",ZoneName);
    SettingParameter::saveToDataBase();
    this->close();
}

