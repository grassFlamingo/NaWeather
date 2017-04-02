#include "showresult.h"
#include "ui_showresult.h"

showResult::showResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showResult)
{
    ui->setupUi(this);
}

showResult::~showResult()
{
    delete ui;
}
