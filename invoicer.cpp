#include "invoicer.h"
#include "lineitem.h"
#include "ui_invoicer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

Invoicer::Invoicer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Invoicer),
    yourInfo(new AddressInfoForm(QString("Your Information"))),
    clientInfo(new AddressInfoForm(QString("Client Information"))),
    lineItems(QVector<LineItem*>()),
    lineItemsContainer(new QWidget)
{
    // set up base UI elements
    ui->setupUi(this);

    selectedCount = 0;

    // set up form information  
    auto formLayout = new QHBoxLayout;
    formLayout->addWidget(yourInfo);
    formLayout->addWidget(clientInfo);
    ui->formInfoContainer->setLayout(formLayout);

    // set up line items
    auto linesLayout = new QVBoxLayout;
    lineItemsContainer->setLayout(linesLayout);
    ui->lineItemsScrollArea->setWidget(lineItemsContainer);
    pushLineItem();

    // connect actions
    //connect(ui->actionQuit, &QAction::triggered, this, &Invoicer::quit);
    connect(ui->actionQuit, &QAction::triggered, this, &QCoreApplication::quit);

    // connect buttons
    connect(ui->addLineItemButton, &QPushButton::clicked, this, &Invoicer::pushLineItem);
    connect(ui->removeLineItemButton, &QPushButton::clicked, this, &Invoicer::popLineItem);
    connect(ui->removeSelectedButton, &QPushButton::clicked, this, &Invoicer::removeSelectedItems);
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

/* Slots */

void Invoicer::setSelectedCount(bool s)
{
    if (s) {
        selectedCount += 1;
    } else {
        selectedCount -= 1;
    }
    if (selectedCount > 1) {
        ui->removeSelectedButton->setEnabled(true);
    } else {
        ui->removeSelectedButton->setEnabled(false);
    }

    if (selectedCount < 0) {
        selectedCount = 0;
    }
}

void Invoicer::pushLineItem()
{
    int length = lineItems.length();
    auto lineItem = new LineItem(length);
    connect(lineItem, &LineItem::selectedChanged, this, &Invoicer::setSelectedCount);
    lineItems.push_back(lineItem);
    lineItemsContainer->layout()->addWidget(lineItems.last());
    if (lineItems.length() > 1) {
        ui->removeLineItemButton->setEnabled(true);
    }
}

void Invoicer::popLineItem()
{
    if (lineItems.length() > 1) {
        auto last = lineItems.takeLast();
        delete last;
    }
    if (lineItems.length() == 1) {
        ui->removeLineItemButton->setEnabled(false);
    }
}

void Invoicer::removeSelectedItems()
{
    auto unselected = QVector<LineItem*>();
    auto oldLayout = lineItemsContainer->layout();
    auto linesLayout = new QVBoxLayout;
    int u_idx = 0;
    foreach(auto lineItem, lineItems) {
        if (lineItem->is_selected()) {
            oldLayout->removeWidget(lineItem);
            delete lineItem;
        } else {
            lineItem->updateIndex(u_idx);
            u_idx += 1;
            linesLayout->addWidget(lineItem);
            unselected.push_back(lineItem);
        }
    }
    delete lineItemsContainer->layout();
    lineItemsContainer->setLayout(linesLayout);
    lineItems = unselected;
    if (lineItems.length()  == 1) {
        ui->removeLineItemButton->setEnabled(false);
        ui->removeSelectedButton->setEnabled(false);
    }
}

void Invoicer::buildPDF() {
    //auto pdf_text = new PDFBuilder();
    //pdf_text.simple_output();
}
