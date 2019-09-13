#include "invoicer.h"
#include "lineitem.h"
#include "ui_invoicer.h"
#include "ui_addressinfoform.h"
#include "ui_lineitem.h"

Invoicer::Invoicer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Invoicer),
    yourInfo(new Ui::AddressInfoForm)
{
    ui->setupUi(this);
    auto layout = new QHBoxLayout;//(this->ui->formInfoTab->layout());
    Ui::LineItem* lineItem1 = new Ui::LineItem();
    layout->addWidget(lineItem1->holder);
    this->ui->lineItemsTab->setLayout(layout);
    connect(ui->actionQuit, &QAction::triggered, this, &Invoicer::quit);
}

Invoicer::~Invoicer()
{
    delete ui;
}

void Invoicer::quit()
{
    QCoreApplication::quit();
}
