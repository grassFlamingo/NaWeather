#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMenu>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QColor>
#include <QToolTip>
#include <QColorDialog>
#include <QPicture>
#include <QTimer>
#include "setingweather.h"
#include "weathericonhelper.h"
#include "weatherresult.h"
#include "weathermssageframe.h"
#include "settingparameter.h"
#include "networkerror.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void settingButtonClickedSlot();
    void windowMoveSlot();
    void menuClickedSlot();

    void resultDataCompleteSlot();    
    void resultDataErrorSlot();
    void areaDataSetedSlot(int areaCode, QColor color, float opacity, bool transparent);

    void todatWeatherLinkHoveredSlot(const QString &link);

    void timeOutSlot();

    void windowOnTopSlot();

private:
    void setData();

    Ui::MainWindow *ui;
    SetingWeather *setWeather;
    WeatherResult* result;
    WeatherMssageFrame* weatherFrame;
    NetWorkError* WeatherError;
    QTimer* timer;
    bool moveable;
    QPoint dragPos;
    float Opacity;
    QColor Color;
    int AreaCode;
    bool Transparent;
    bool WindowOnTop;
};

#endif // MAINWINDOW_H
