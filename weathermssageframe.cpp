#include "weathermssageframe.h"
#include "ui_weathermssageframe.h"

WeatherMssageFrame::WeatherMssageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WeatherMssageFrame)
{
    ui->setupUi(this);
    Today = NULL;
    Tomorrow = NULL;
    DayAfterTomorrow = NULL;

    setWindowOpacity(1.0);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(windowOnCloseSlot()));
}

WeatherMssageFrame::~WeatherMssageFrame()
{
    delete ui;
}

void WeatherMssageFrame::setData(WeatherModel *today, WeatherModel *tomorrow, WeatherModel *dayAfterTomorrow){
    this->Today = today;
    this->Tomorrow = tomorrow;
    this->DayAfterTomorrow = dayAfterTomorrow;

    ui->TodayImg->setText(Today->getShowPicHtml());
    ui->TodayText->setText(Today->Situation + Today->Temperature);
    ui->TomorrowImg->setText(Tomorrow->getShowPicHtml());
    ui->TomorrowText->setText(Tomorrow->Situation + Tomorrow->Temperature);
    ui->DayAfterTomorrowImg->setText(DayAfterTomorrow->getShowPicHtml());
    ui->DayAfterTomorrowText->setText(DayAfterTomorrow->Situation + DayAfterTomorrow->Temperature);
}

void WeatherMssageFrame::showWidget(){
    this->move(QCursor::pos());
    setStyleSheet(QString("background: %1 ;").arg(SettingParameter::getData("Color").toString()));
    setWindowOpacity(SettingParameter::getData("Opacity").toFloat() + 0.3);
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    QPoint nowPoint = QCursor::pos();
    animation->setDuration(500);
    animation->setStartValue(QRect(nowPoint.x(),nowPoint.y(),300,0));
    animation->setEndValue(QRect(nowPoint.x(),nowPoint.y(),300,150));
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    timer.start(5000);
    this->show();
}

void WeatherMssageFrame::mouseMoveEvent(QMouseEvent *event){
    if( !frameGeometry().contains(event->pos())){
        timer.start(500);
    }
}

void WeatherMssageFrame::windowOnCloseSlot(){
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(500);
    QPoint nowPoint = this->pos();
    animation->setStartValue(QRect(nowPoint.x(),nowPoint.y(),300,150));
    animation->setEndValue(QRect(nowPoint.x(),nowPoint.y(),300,0));
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
}
