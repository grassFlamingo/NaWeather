#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    result = new WeatherResult();
    setWeather = new SetingWeather();
    weatherFrame = new WeatherMssageFrame();
    timer = new QTimer();
    WeatherError = new NetWorkError();

    moveable = false;
    ui->MoveBar->setVisible(false);

    Opacity = SettingParameter::getData("Opacity").toFloat();
    Color = QColor(SettingParameter::getData("Color").toString());
    AreaCode = SettingParameter::getData("AreaCode").toInt();
    Transparent = SettingParameter::getData("Transparent").toInt();
    WindowOnTop = SettingParameter::getData("WindowOnTop").toBool();

    setStyleSheet(Transparent ? "" : QString("background: %1 ;")
                                .arg(Color.name()));

    setWindowOpacity(Opacity);
    if(WindowOnTop){
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    }else{
        setWindowFlags(Qt::FramelessWindowHint);
    }
    setAttribute(Qt::WA_TranslucentBackground);


    connect(ui->menuButton, SIGNAL(clicked()), this, SLOT(menuClickedSlot()));
    connect(ui->TodayPic, SIGNAL(linkHovered(QString)), this, SLOT(todatWeatherLinkHoveredSlot(QString)));

    connect(setWeather, SIGNAL(areaDataSeted(int,QColor,float,bool)), this, SLOT(areaDataSetedSlot(int,QColor,float,bool)));
    connect(result, SIGNAL(dataCompleteSignal()), this, SLOT(resultDataCompleteSlot()));

    connect(timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    connect(result, SIGNAL(dataErrorSignal()), this, SLOT(resultDataErrorSlot()));

    connect(WeatherError, SIGNAL(tryAgainSignal()), this, SLOT(timeOutSlot()));

    timeOutSlot();

    timer->start(9000000);
}

MainWindow::~MainWindow()
{
    delete ui;
    result->deleteLater();
    setWeather->deleteLater();
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    if( event->button() == Qt::LeftButton ){
        dragPos = event->globalPos() - frameGeometry().topLeft();
        if( this->frameGeometry().contains(event->pos()) ){
            moveable = true;
        }
    }
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton && moveable){
        move(event->globalPos() - dragPos);
    }
    event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    moveable = false;
    ui->MoveBar->setVisible(false);
    ui->menuButton->setVisible(true);
    event->accept();
}

void MainWindow::settingButtonClickedSlot(){
    setWeather->setData();
    setWeather->exec();
}

void MainWindow::resultDataCompleteSlot(){
    ui->CityLabel->setText(result->getCity());
    ui->CityLabel->setToolTip(result->getDiscripe());
    WeatherDetailsModel* todayDetails = result->getTodayDetails();
    ui->AirQualityLabel->setText(todayDetails->AirQuality);
    ui->AirQualityLabel->setToolTip(todayDetails->AirQuality);
    ui->HumidityLabel->setText(todayDetails->Humidity);
    ui->HumidityLabel->setToolTip(todayDetails->Humidity);
    ui->TemperatureLabel->setText(todayDetails->Temperature);
    ui->TemperatureLabel->setToolTip(todayDetails->Temperature);
    ui->WindLabel->setText(todayDetails->Wind);
    ui->WindLabel->setToolTip(todayDetails->Wind);
    WeatherAdvicesModel* todayAdvice = result->getTodayAdvices();
    ui->UVRaditionText->setText(todayAdvice->getLabelText(todayAdvice->UVRadition));
    ui->UVRaditionText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->UVRadition));
    ui->ColdText->setText(todayAdvice->getLabelText(todayAdvice->Cold));
    ui->ColdText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->Cold));
    ui->AirPolitionText->setText(todayAdvice->getLabelText(todayAdvice->AirPolution));
    ui->AirPolitionText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->AirPolution));
    ui->SpostText->setText(todayAdvice->getLabelText(todayAdvice->Sport));
    ui->SpostText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->Sport));
    ui->WearText->setText(todayAdvice->getLabelText(todayAdvice->Wear));
    ui->WearText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->Wear));
    ui->WashCarText->setText(todayAdvice->getLabelText(todayAdvice->WashCar));
    ui->WashCarText->setToolTip(todayAdvice->getLabelToolTip(todayAdvice->WashCar));
    WeatherModel* today = result->getToday();
    ui->TodayText->setText(today->Situation);
    ui->TodayText->setToolTip(today->Situation);
    ui->TodayPic->setText(today->getShowPicHtml());
    WeatherModel* tomorrow = result->getTomorrow();
    WeatherModel* dayAfterTomorrow = result->getDayAfterTomorrow();
    weatherFrame->setData(today, tomorrow, dayAfterTomorrow);

//    int timediff = result->getLastUpdateTime().addSecs(9000).msecsTo(QDateTime::currentDateTime());
//    int ss = timediff < 5000 ? 5000 : timediff;
//    qDebug()<< QString::number(ss);
//    timer->start( timediff < 5000 ? 5000 : timediff); //2.5-hours

}

void MainWindow::resultDataErrorSlot(){
    this->close();
    WeatherError->showDiaglog();
    this->show();
    resultDataCompleteSlot();
}


void MainWindow::menuClickedSlot(){
    QMenu menu;
    menu.addAction("Setting", this, SLOT(settingButtonClickedSlot()));
    menu.addAction("Move", this, SLOT(windowMoveSlot()));
    if(WindowOnTop){
        menu.addAction("Not On Top", this, SLOT(windowOnTopSlot()));
    }else{
        menu.addAction("On Top", this, SLOT(windowOnTopSlot()));
    }
    menu.addSeparator();
    menu.addAction("Exit", this, SLOT(close()));
    menu.exec(QCursor::pos());
}

void MainWindow::windowMoveSlot(){
    moveable = true;
    ui->MoveBar->setVisible(true);
    ui->menuButton->setVisible(false);
}

void MainWindow::windowOnTopSlot(){
    WindowOnTop = !WindowOnTop;
    if(WindowOnTop){
        setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint );
    }else{
        setWindowFlags(this->windowFlags() &~ Qt::WindowStaysOnTopHint);
    }
    this->show();
    SettingParameter::setData("WindowOnTop", WindowOnTop);
    SettingParameter::saveToDataBase();

}

void MainWindow::areaDataSetedSlot(int areaCode, QColor color, float opacity, bool transparent){
    this->AreaCode = areaCode;
    this->Color = color;
    this->Opacity = opacity;
    this->Transparent = transparent;

    result->setAreaCode(areaCode);
    result->getWeatherData();

    QString style = transparent ? "" : QString("background: %1 ;").arg(Color.name());
    this->setStyleSheet(style);
    setWindowOpacity(opacity);
}

void MainWindow::todatWeatherLinkHoveredSlot(const QString &link){
    weatherFrame->showWidget();
}

void MainWindow::timeOutSlot(){
    result->setAreaCode(AreaCode);
    result->getWeatherData();
}
