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


bool AddressInfoForm::read(const QJsonObject &json)
{
    if (!json.contains("name") ||
        !json.contains("street_address") ||
        !json.contains("city") ||
        !json.contains("state") ||
        !json.contains("zip") ||
        !json.contains("phone") ||
        !json.contains("email")
        )
    {
        qWarning("Invalid Address Form");
        return false;
    }

    auto new_name = json["name"];
    auto new_streetAddress = json["street_address"];
    auto new_city = json["city"];
    auto new_state = json["state"];
    auto new_zip = json["zip"];
    auto new_phone = json["phone"];
    auto new_email = json["email"];

    if (!new_name.isString() ||
        !new_streetAddress.isString() ||
        !new_city.isString() ||
        !new_state.isString() ||
        !new_zip.isString() ||
        !new_phone.isString() ||
        !new_email.isString()
        )
    {
        qWarning("Invalid Address Form Types");
        return false;
    }

    name->setText(new_name.toString());
    streetAddress->setText(new_streetAddress.toString());
    city->setText(new_city.toString());
    state->setText(new_state.toString());
    zip->setText(new_zip.toString());
    phone->setText(new_phone.toString());
    email->setText(new_email.toString());

    return true;
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
