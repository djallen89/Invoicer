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
    connect(ui->actionQuit, &QAction::triggered, this, &Invoicer::quit);

    // connect buttons
    connect(ui->addLineItemButton, &QPushButton::clicked, this, &Invoicer::pushLineItem);
    connect(ui->removeLineItemButton, &QPushButton::clicked, this, &Invoicer::popLineItem);
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

void Invoicer::quit()
{
    QCoreApplication::quit();
}

void Invoicer::pushLineItem()
{
    int length = lineItems.length();
    lineItems.push_back(new LineItem(length));
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
    auto selected = QVector<LineItem*>();
    int u_idx = 0;
    foreach(auto lineItem, lineItems) {
        if (lineItem->is_selected()) {
            selected.push_back(lineItem);
        } else {
            lineItem->updateIndex(u_idx);
            u_idx += 1;
            unselected.push_back(lineItem);
        }
    }
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
