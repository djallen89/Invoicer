#include "invoicer.h"
#include "lineitem.h"
#include "ui_invoicer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

Invoicer::Invoicer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Invoicer),
    yourInfo(new AddressInfoForm),
    clientInfo(new AddressInfoForm),
    lineItems(QVector<LineItem*>()),
    lineItemsContainer(new QWidget)
{
    // set up base UI elements
    ui->setupUi(this);

    // set up form information  
    auto formLayout = new QHBoxLayout;
    formLayout->addWidget(yourInfo);
    formLayout->addWidget(clientInfo);
    this->ui->formInfoTab->setLayout(formLayout);

    // set up line items
    auto linesLayout = new QVBoxLayout;
    this->lineItemsContainer->setLayout(linesLayout);
    this->ui->lineItemsScrollArea->setWidget(lineItemsContainer);
    this->addLineItem();

    // connect actions
    connect(ui->actionQuit, &QAction::triggered, this, &Invoicer::quit);

    // connect buttons
    connect(ui->addLineItemButton, &QPushButton::clicked, this, &Invoicer::addLineItem);
}

Invoicer::~Invoicer()
{
    delete yourInfo;
    delete clientInfo;
    foreach (auto lineItem, lineItems) {
        delete lineItem;
    }
    lineItems.clear();
    delete lineItemsContainer;
    delete ui;
}

void Invoicer::quit()
{
    QCoreApplication::quit();
}

void Invoicer::addLineItem()
{
    lineItems.push_back(new LineItem());
    this->lineItemsContainer->layout()->addWidget(lineItems.last()->container);
}
