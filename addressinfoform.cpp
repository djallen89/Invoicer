#include "addressinfoform.h"
#include "ui_addressinfoform.h"

AddressInfoForm::AddressInfoForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddressInfoForm)
{
    ui->setupUi(this);
}

AddressInfoForm::~AddressInfoForm()
{
    delete ui;
}
