#ifndef INVOICER_H
#define INVOICER_H

#include <QMainWindow>
#include <QVector>
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
               
private slots:
    void quit();
    void addLineItem();
    
private:
    Ui::Invoicer *ui;
    AddressInfoForm *yourInfo;
    AddressInfoForm *clientInfo;
    QVector<LineItem*> lineItems;
    QWidget *lineItemsContainer;
};

#endif // INVOICER_H
