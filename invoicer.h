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
    bool read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void buildPDF();
               
private slots:
    void setSaveFile();
    void save() const;
    void saveAs();
    void open();
    void setSelectedCount(bool s);
    void pushLineItem();
    void popLineItem();
    void removeSelectedItems();
    
private:
    void setFileName();
    Ui::Invoicer *ui;
    QString *fileName;
    int selectedCount;
    AddressInfoForm *yourInfo;
    AddressInfoForm *clientInfo;
    QVector<LineItem*> lineItems;
    QWidget *lineItemsContainer;
};

#endif // INVOICER_H
