#include "addressinfoform.h"
#include <QFormLayout>

AddressInfoForm::AddressInfoForm(QWidget *parent) :
    QFrame(parent)
{
    container = new QFrame();
    /*
    nameLabel = new QLabel("Name");
    streetAddressLabel = new QLabel("Street Address");
    cityLabel = new QLabel("City");
    stateLabel = new QLabel("State");
    zipLabel = new QLabel("Zip Code");
    phoneLabel = new QLabel("Phone Number");
    emailLabel = new QLabel("Email Address");
    */
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
    /*
    delete nameLabel;
    delete streetAddressLabel;
    delete cityLabel;
    delete stateLabel;
    delete zipLabel;
    delete phoneLabel;
    delete emailLabel;
    */
}
