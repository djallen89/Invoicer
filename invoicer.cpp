#include "invoicer.h"
#include "lineitem.h"
#include "ui_invoicer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringLiteral>
#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <math.h>

Invoicer::Invoicer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Invoicer),
    yourInfo(new AddressInfoForm(QString("Your Information"))),
    clientInfo(new AddressInfoForm(QString("Client Information"))),
    lineItems(QVector<LineItem*>()),
    lineItemsContainer(new QWidget)
{
    fileName = new QString();
    
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
    connect(ui->actionSave, &QAction::triggered, this, &Invoicer::setSaveFile);
    connect(ui->actionSave, &QAction::triggered, this, &Invoicer::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Invoicer::saveAs);
    connect(ui->actionOpen, &QAction::triggered, this, &Invoicer::open);

    // connect buttons
    connect(ui->addLineItemButton, &QPushButton::clicked, this, &Invoicer::pushLineItem);
    connect(ui->removeLineItemButton, &QPushButton::clicked, this, &Invoicer::popLineItem);
    connect(ui->removeSelectedButton, &QPushButton::clicked, this, &Invoicer::removeSelectedItems);
}

Invoicer::~Invoicer()
{
    delete fileName;
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

void Invoicer::setSaveFile()
{
    if (fileName->isNull()) {
        setFileName();
    }
}

void Invoicer::save() const
{
    if (fileName->isNull()) {
        return;
    }
    
    QFile saveFile(*fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject invoiceObject;
    write(invoiceObject);
    QJsonDocument saveDoc(invoiceObject);
    saveFile.write(saveDoc.toJson());
    return;
}

void Invoicer::saveAs()
{
    setFileName();
    save();
}

void Invoicer::open()
{
    QString newFile = QFileDialog::getOpenFileName(
        this,
        "Open Invoice",
        QDir::homePath(),
        "JSON file (*.json");

    if (newFile.isNull()) {
        return;
    }

    QFile loadFile(newFile);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file.");
        return;
    }

    QByteArray invoiceObject = loadFile.readAll();

    QJsonDocument loadDoc = QJsonDocument::fromJson(invoiceObject);
    if (read(loadDoc.object())) {
        *fileName = newFile;
    }
}

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
    delete oldLayout;
    lineItemsContainer->setLayout(linesLayout);
    lineItems = unselected;
    if (lineItems.length()  == 1) {
        ui->removeLineItemButton->setEnabled(false);
        ui->removeSelectedButton->setEnabled(false);
    }
}

/* Public Member Functions */

bool Invoicer::read(const QJsonObject &json)
{
    /* Check validity of document */
    
    if (!json.contains("invoice_number") ||
        !json.contains("currency_name") ||
        !json.contains("currency_symbol") ||
        !json.contains("selected_count") ||
        !json.contains("your_info") ||
        !json.contains("client_info") ||
        !json.contains("line_items")
        )
    {
        qWarning("Invalid save file");
        return false;
    }

    auto invoice_number = json["invoice_number"];
    auto currency_name = json["currency_name"];
    auto currency_symbol = json["currency_symbol"];
    auto selected_count = json["selected_count"];
    auto your_info = json["your_info"];
    auto client_info = json["client_info"];
    auto line_items = json["line_items"];

    if (!invoice_number.isString() ||
        !currency_name.isString() ||
        !currency_symbol.isString() ||
        !selected_count.isDouble() ||
        !your_info.isObject() ||
        !client_info.isObject() || 
        !line_items.isArray()
        )
    {
        qWarning("Invalid types found in save file.");
        return false;
    }

    /* end check of validity of document */

    /* create new members 
     * check during their read function for validity */
    auto newYourInfo = new AddressInfoForm(QString("Your Information"));
    auto newClientInfo = new AddressInfoForm(QString("Client Information"));

    if (!newYourInfo->read(your_info.toObject())) {
        qWarning("Invalid address information.");
        return false;
    }
    
    if (!newClientInfo->read(client_info.toObject())) {
        qWarning("Invalid client address information.");
        return false;
    }

    auto newLineItems = QVector<LineItem*>();
    auto new_line_items_arr = line_items.toArray();
    auto newLineItemsLayout = new QVBoxLayout();
    for (int i = 0; i < new_line_items_arr.size(); ++i) {
        auto lineItem = new LineItem(i);
        auto lineItemObject = new_line_items_arr[i].toObject();
        if (!lineItem->read(lineItemObject)) {
            // qWarning("Invalid Line Item.");
            return false;
        }
        newLineItemsLayout->addWidget(lineItem);
        newLineItems.push_back(lineItem);        
    }

    /* Everything checks out */

    ui->invoiceNumberLineEdit->setText(invoice_number.toString());
    ui->currencyNameLineEdit->setText(currency_name.toString());
    ui->currencySymbolLineEdit->setText(currency_symbol.toString());
    selectedCount = static_cast<int>(round(selected_count.toDouble()));
    
    delete yourInfo;
    yourInfo = newYourInfo;
    delete clientInfo;
    clientInfo = newClientInfo;

    auto layout = lineItemsContainer->layout();
    foreach(auto oldLineItem, lineItems) {
        layout->removeWidget(oldLineItem);
        delete oldLineItem;
    }
    lineItems = newLineItems;
    foreach(auto lineItem, lineItems) {
        connect(lineItem, &LineItem::selectedChanged, this, &Invoicer::setSelectedCount);
    }
    delete lineItemsContainer->layout();
    lineItemsContainer->setLayout(newLineItemsLayout);

    return true;
}

void Invoicer::write(QJsonObject &json) const
{
    json["invoice_number"] = ui->invoiceNumberLineEdit->text();
    json["currency_name"] = ui->currencyNameLineEdit->text();
    json["currency_symbol"] = ui->currencySymbolLineEdit->text();
    json["selected_count"] = selectedCount;

    QJsonObject yourInfoObject;
    yourInfo->write(yourInfoObject);
    json["your_info"] = yourInfoObject;

    QJsonObject clientInfoObject;
    clientInfo->write(clientInfoObject);
    json["client_info"] = clientInfoObject;

    QJsonArray lineItemsArray;
    foreach(auto lineItem, lineItems) {
	QJsonObject lineItemObject;
	lineItem->write(lineItemObject);
	lineItemsArray.append(lineItemObject);
    }
    json["line_items"] = lineItemsArray;
}

void Invoicer::buildPDF() {
    //auto pdf_text = new PDFBuilder();
    //pdf_text.simple_output();
}

/* Private methods */

void Invoicer::setFileName()
{
    *fileName = QFileDialog::getSaveFileName(
        this,
        "Save Invoice",
        QDir::homePath(),
        "JSON file (*.json)");
}
