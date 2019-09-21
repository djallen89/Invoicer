#include "invoicer.h"
#include "lineitem.h"
#include "ui_invoicer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStringLiteral>
#include <QStringBuilder>
#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <math.h>

Invoicer::Invoicer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Invoicer) //,
    // yourInfo(new AddressInfoForm(QString("Your Information"))),
    // clientInfo(new AddressInfoForm(QString("Client Information"))),
    // lineItems(QVector<LineItem*>()),
    // lineItemsContainer(new QWidget)
{
    modified = true;
    fileName = new QString();
    
    // set up base UI elements
    ui->setupUi(this);

    selectedCount = 0;

    // set up form information
    yourInfo = new AddressInfoForm(QString("Your Information"));
    clientInfo = new AddressInfoForm(QString("Client Information"));
    auto formLayout = new QHBoxLayout;
    formLayout->addWidget(yourInfo);
    formLayout->addWidget(clientInfo);
    ui->formInfoContainer->setLayout(formLayout);

    // set up line items
    lineItems = QVector<LineItem*>();
    lineItemsContainer = new QWidget;
    auto linesLayout = new QVBoxLayout;
    lineItemsContainer->setLayout(linesLayout);
    ui->lineItemsScrollArea->setWidget(lineItemsContainer);
    pushLineItem();

    // connect actions
    //connect(ui->actionQuit, &QAction::triggered, this, &Invoicer::quit);
    connect(ui->actionNew_Invoice, & QAction::triggered, this, &Invoicer::newInvoice);
    connect(ui->actionSave, &QAction::triggered, this, &Invoicer::setSaveFile);
    connect(ui->actionSave, &QAction::triggered, this, &Invoicer::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Invoicer::saveAs);
    connect(ui->actionOpen, &QAction::triggered, this, &Invoicer::open);
    connect(ui->actionQuit, &QAction::triggered, this, &QCoreApplication::quit);
    connect(ui->actionAbout, &QAction::triggered, this, &Invoicer::about);

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

void Invoicer::newInvoice()
{
    if (modified) {
        QMessageBox newConfirm;
        newConfirm.setText("The current invoice will be discarded.");
        newConfirm.setInformativeText("Do you want to save your changes?");
        newConfirm.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        newConfirm.setDefaultButton(QMessageBox::Save);
        int confirm = newConfirm.exec();

        switch (confirm) {
        case QMessageBox::Save:
            setFileName();
            save();
            break;
        case QMessageBox::Discard:
            // foo
            break;
        case QMessageBox::Cancel:
            return;
        default:
            qWarning("An impossible situation has occurred.");
            return;
        }
    }

    setupNewInvoice();
}

void Invoicer::setSaveFile()
{
    if (fileName->isNull()) {
        setFileName();
    }
}

void Invoicer::save()
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
    modified = false;
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
        "JSON (*.json");

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

void Invoicer::about()
{
    auto title = QString("About Invoicer");
    QString aboutText(
        "<h1 style=\"text-align: center\">Invoicer</h1>"
        "<p>A simple tool to construct invoices.</p>"
        "<p>Copyright (c) 2019, Dominick Allen</p>"
        "<p>All rights reserved.</p>"
        "<p>Redistribution and use in source and binary forms, with or without"
        "modification, are permitted provided that the following conditions are met:</p>"
        "<ol>"
        "<li> Redistributions of source code must retain the above copyright notice, this"
        "list of conditions and the following disclaimer.</li>"
        "<li> Redistributions in binary form must reproduce the above copyright notice,"
        "this list of conditions and the following disclaimer in the documentation"
        "and/or other materials provided with the distribution.</li>"
        "</ol>"
        "<p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR"
        "ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</p>"
        "<p>The views and conclusions contained in the software and documentation are those"
        "of the authors and should not be interpreted as representing official policies,"
        "either expressed or implied, of the Invoicer project.</p>");

    QMessageBox::about(this, title, aboutText);
}

void Invoicer::setModified(bool m)
{
    modified = m;
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
    //connect(lineItem, &LineItem::selectedChanged, this, &Invoicer::setSelectedCount);
    connectLineItemSignals(lineItem);
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

void Invoicer::setTotal()
{
    double sum = calculateTotal();
    ui->totalLabel->setText(QString::number(sum, 'f', 2));
}

/* Public Methods */

QString Invoicer::getInvoiceNumber() const
{
    return ui->invoiceNumberLineEdit->text();
}

QString Invoicer::getCurrencyName() const
{
    return ui->currencyNameLineEdit->text();
}

QString Invoicer::getCurrencySymbol() const
{
    return ui->currencySymbolLineEdit->text();
}

void Invoicer::setInvoiceNumber(QString n)
{
    ui->invoiceNumberLineEdit->setText(n);
}

void Invoicer::setCurrencyName(QString cn)
{
    ui->currencyNameLineEdit->setText(cn);
}

void Invoicer::setCurrencySymbol(QString cs)
{
    ui->currencySymbolLineEdit->setText(cs);
}

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

    //ui->invoiceNumberLineEdit->setText(
    setInvoiceNumber(invoice_number.toString());
    //ui->currencyNameLineEdit->setText(currency_name.toString());
    setCurrencyName(currency_name.toString());
    //ui->currencySymbolLineEdit->setText(
    setCurrencySymbol(currency_symbol.toString());
    selectedCount = static_cast<int>(round(selected_count.toDouble()));

    auto formLayout = ui->formInfoContainer->layout();
    formLayout->removeWidget(yourInfo);
    formLayout->removeWidget(clientInfo);
    delete yourInfo;
    delete clientInfo;
    yourInfo = newYourInfo;
    clientInfo = newClientInfo;
    formLayout->addWidget(yourInfo);
    formLayout->addWidget(clientInfo);

    auto layout = lineItemsContainer->layout();
    foreach(auto oldLineItem, lineItems) {
        layout->removeWidget(oldLineItem);
        delete oldLineItem;
    }
    lineItems = newLineItems;
    foreach(auto lineItem, lineItems) {
        // connect(lineItem, &LineItem::selectedChanged, this, &Invoicer::setSelectedCount);
        connectLineItemSignals(lineItem);
    }
    delete lineItemsContainer->layout();
    lineItemsContainer->setLayout(newLineItemsLayout);

    setTotal();

    return true;
}

void Invoicer::write(QJsonObject &json) const
{
    json["invoice_number"] = getInvoiceNumber();
    json["currency_name"] = getCurrencyName();
    json["currency_symbol"] = getCurrencySymbol();
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

void Invoicer::buildPDF()
{
    //auto pdf_text = new PDFBuilder();
    //pdf_text.simple_output();
}

QString Invoicer::buildLatex() const
{
    QString text_doc = QString("");
    auto header = QString("\\documentclass[12pt, letterpaper]{article}\n"
                          "\\usepackage[margin=1.0in]{geometry}\n"
                          "\\usepackage[pdftex,\n"
                          "pdfauthor={YOUR_NAME},\n"
                          "pdftitle={Invoice INVOICE_NUMBER},\n"
                          "pdfproducer={Latex with hyperref},\n"
                          "pdfcreator={pdflatex}]{hyperref}\n"
                          "\\usepackage{parskip, longtable, tabu}\n"
                          "\\begin{document}\n");

    auto your_info_minipage = yourInfo->buildLatex();
    auto client_info_minipage = clientInfo->buildLatex();

    auto invoice_number_text = QString("Invoice Number: \\#") + getInvoiceNumber() + QString("\n");

    auto table_header = QString("  Date & Hours/Product & Quantity & Rate/Cost & Description""\\""\\""\\hline\n");
    
    auto table_begin = QString("  \\begin{longtabu} to 1.0\\textwidth {|l|c|r|r|X[l]|}\\hline\n") 
        % table_header
        % QString("  \\endfirsthead\n"
                  "  \\hline \n\n")
        % table_header
        % QString("  \\endhead\n\n");

    auto table_end = QString("\\end{longtabu}\n");

    auto total = QString("\\begin{flushright} \\textbf{Total: ")
        + ui->totalLabel->text()
        + QString("}\\end{flushright}\n");
    auto footer = QString("\\end{document}");
    
    text_doc = header % your_info_minipage % client_info_minipage % invoice_number_text;
    text_doc = text_doc % table_header % table_begin;
    foreach(auto lineItem, lineItems) {
        text_doc = text_doc % lineItem->buildLatex();
        //subtotal += 
    }
    text_doc = text_doc
        % table_end
        % total
        % footer;

    return text_doc;
}

/* Private methods */

void Invoicer::setupNewInvoice()
{
    auto infoContainerLayout = ui->formInfoContainer->layout();
    infoContainerLayout->removeWidget(yourInfo);
    infoContainerLayout->removeWidget(clientInfo);
    delete yourInfo;
    delete clientInfo;
    yourInfo = new AddressInfoForm(QString("Your Information"));
    clientInfo = new AddressInfoForm(QString("Client Information"));
    infoContainerLayout->addWidget(yourInfo);
    infoContainerLayout->addWidget(clientInfo);

    auto lineLayout = lineItemsContainer->layout();
    foreach (auto lineItem, lineItems) {
        lineLayout->removeWidget(lineItem);
        delete lineItem;
    }
    lineItems.clear();
    pushLineItem();

    ui->totalLabel->setText("Total: " + QString::number(calculateTotal(), 'f', 2));
}

void Invoicer::connectLineItemSignals(LineItem* lineItem)
{
    connect(lineItem, &LineItem::selectedChanged, this, &Invoicer::setSelectedCount);
    connect(lineItem, &LineItem::subtotalChanged, this, &Invoicer::setTotal);
}

void Invoicer::setFileName()
{
    *fileName = QFileDialog::getSaveFileName(
        this,
        "Save Invoice",
        QDir::homePath() % "/" % getInvoiceNumber() % ".json",
        "JSON (*.json)");
}

double Invoicer::calculateTotal()
{
    double sum = 0.0;
    foreach(auto lineItem, lineItems)
    {
        sum += lineItem->quantify() * lineItem->rate();
    }
    return sum;
}
