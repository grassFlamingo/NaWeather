#ifndef WEATHERMSSAGEFRAME_H
#define WEATHERMSSAGEFRAME_H

#include <QFrame>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>
#include <QCursor>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "weatherresult.h"
#include "weathericonhelper.h"
#include "settingparameter.h"

namespace Ui {
class WeatherMssageFrame;
}

class WeatherMssageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WeatherMssageFrame(QWidget *parent = 0);
    ~WeatherMssageFrame();

    void setData(WeatherModel* today,
                 WeatherModel* tomorrow,
                 WeatherModel* dayAfterTomorrow);

    void showWidget();

    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void windowOnCloseSlot();

private:
    WeatherModel* Today;
    WeatherModel* Tomorrow;
    WeatherModel* DayAfterTomorrow;
    Ui::WeatherMssageFrame *ui;
    QTimer timer;
};

#endif // WEATHERMSSAGEFRAME_H
