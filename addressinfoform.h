#ifndef ADDRESSINFOFORM_H
#define ADDRESSINFOFORM_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>

class AddressInfoForm : public QFrame
{
    Q_OBJECT

public:
    explicit AddressInfoForm(QWidget *parent = nullptr);
    ~AddressInfoForm();
    QFrame *container;

private:
    /*
    QLabel *nameLabel;
    QLabel *streetAddressLabel;
    QLabel *cityLabel;
    QLabel *stateLabel;
    QLabel *zipLabel;
    QLabel *phoneLabel;
    QLabel *emailLabel;
    */
    QLineEdit *name;
    QLineEdit *streetAddress;
    QLineEdit *city;
    QLineEdit *state;
    QLineEdit *zip;
    QLineEdit *phone;
    QLineEdit *email;
};

#endif // ADDRESSINFOFORM_H
