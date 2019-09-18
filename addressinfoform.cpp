#include "addressinfoform.h"
#include <QFormLayout>
#include <QSizePolicy>

AddressInfoForm::AddressInfoForm(QString entityName, QWidget *parent) :
    QGroupBox(entityName, parent)
{
    container = new QFrame();
    name = new QLineEdit();
    streetAddress = new QLineEdit();
    city = new QLineEdit();
    state = new QLineEdit();
    zip = new QLineEdit();
    phone = new QLineEdit();
    email = new QLineEdit();

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Name"), name);
    formLayout->addRow(tr("Street Address"), streetAddress);
    formLayout->addRow(tr("City"), city);
    formLayout->addRow(tr("State"), state);
    formLayout->addRow(tr("Zip Code"), zip);
    formLayout->addRow(tr("Phone Number"), phone);
    formLayout->addRow(tr("Email Address"), email);
    setLayout(formLayout);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(300, 200);
    setMaximumSize(10000, 500);
}

AddressInfoForm::~AddressInfoForm()
{
    delete name;
    delete streetAddress;
    delete city;
    delete state;
    delete zip;
    delete phone;
    delete email;
    delete container;
}


void AddressInfoForm::read(const QJsonObject &json)
{
    
}
void AddressInfoForm::write(QJsonObject &json) const
{
    json["name"] = name->text();
    json["street_address"] = streetAddress->text();
    json["city"] = city->text();
    json["state"] = state->text();
    json["zip"] = zip->text();
    json["phone"] = phone->text();
    json["email"] = email->text();

}
