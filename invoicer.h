#ifndef INVOICER_H
#define INVOICER_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QFile>
#include "addressinfoform.h"
#include "lineitem.h"

namespace Ui {
    class Invoicer;
}

class Invoicer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Invoicer(QWidget *parent = nullptr);
    ~Invoicer();
    QString getInvoiceNumber() const;
    QString getCurrencyName() const;
    QString getCurrencySymbol() const;
    void setInvoiceNumber(QString n);
    void setCurrencyName(QString cn);
    void setCurrencySymbol(QString cs);
    bool read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QString buildLatex() const;
    void buildPDF();
               
private slots:
    void newInvoice();
    void setSaveFile();
    void save();
    void saveAs();
    void open();
    void about();
    void setModified(bool m);
    void setSelectedCount(bool s);
    void pushLineItem();
    void popLineItem();
    void removeSelectedItems();
    void setTotal();
    
private:
    void setupNewInvoice();
    void connectLineItemSignals(LineItem* lineItem);
    void setFileName();
    double calculateTotal();
    Ui::Invoicer *ui;
    bool modified;
    QString *fileName;
    int selectedCount;
    AddressInfoForm *yourInfo;
    AddressInfoForm *clientInfo;
    QVector<LineItem*> lineItems;
    QWidget *lineItemsContainer;
};

#endif // INVOICER_H
