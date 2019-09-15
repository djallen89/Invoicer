#ifndef ADDRESSINFOFORM_H
#define ADDRESSINFOFORM_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QString>

class AddressInfoForm : public QGroupBox
{
    Q_OBJECT

public:
    explicit AddressInfoForm(QString entityName, QWidget *parent = nullptr);
    ~AddressInfoForm();
    QFrame *container;

private:
    QLineEdit *name;
    QLineEdit *streetAddress;
    QLineEdit *city;
    QLineEdit *state;
    QLineEdit *zip;
    QLineEdit *phone;
    QLineEdit *email;
};

#endif // ADDRESSINFOFORM_H
