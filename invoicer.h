#ifndef INVOICER_H
#define INVOICER_H

#include <QMainWindow>
#include "addressinfoform.h"

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
    
private:
    QWidget *container;
    Ui::Invoicer *ui;
    Ui::AddressInfoForm *yourInfo;
    //Ui::AddressInfoForm *clientInfo;
};

#endif // INVOICER_H
