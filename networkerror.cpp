#include "networkerror.h"
#include "ui_networkerror.h"

NetWorkError::NetWorkError(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::NetWorkError)
{
    ui->setupUi(this);
    connect(ui->TryButton, SIGNAL(clicked()), this, SLOT(tryAgainSlot()));
}

NetWorkError::~NetWorkError()
{
    delete ui;
}

int NetWorkError::showDiaglog(){
    setWindowOpacity(SettingParameter::getData("Opacity").toFloat());
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    //setAttribute(Qt::WA_TranslucentBackground);

    setStyleSheet(QString("background: %1 ;")
                                      .arg(SettingParameter::getData("Color").toString()));
    return this->exec();
}

void NetWorkError::tryAgainSlot(){
    this->close();
    emit tryAgainSignal();
}
