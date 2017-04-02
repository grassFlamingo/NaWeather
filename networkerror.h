#ifndef NETWORKERROR_H
#define NETWORKERROR_H

#include <QDialog>
#include <QShowEvent>
#include "settingparameter.h"

namespace Ui {
class NetWorkError;
}

class NetWorkError : public QDialog
{
    Q_OBJECT

public:
    explicit NetWorkError(QDialog *parent = 0);
    ~NetWorkError();

    int showDiaglog();

signals:
    void tryAgainSignal();
    void exitSignal();

private slots:
    void tryAgainSlot();

private:
    Ui::NetWorkError *ui;
};

#endif // NETWORKERROR_H
