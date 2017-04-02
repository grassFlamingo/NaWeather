#ifndef SHOWRESULT_H
#define SHOWRESULT_H

#include <QWidget>

namespace Ui {
class showResult;
}

class showResult : public QWidget
{
    Q_OBJECT

public:
    explicit showResult(QWidget *parent = 0);
    ~showResult();

private:
    Ui::showResult *ui;
};

#endif // SHOWRESULT_H
