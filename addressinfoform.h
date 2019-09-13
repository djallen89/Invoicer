#ifndef ADDRESSINFOFORM_H
#define ADDRESSINFOFORM_H

#include <QFrame>

namespace Ui {
class AddressInfoForm;
}

class AddressInfoForm : public QFrame
{
    Q_OBJECT

public:
    explicit AddressInfoForm(QWidget *parent = nullptr);
    ~AddressInfoForm();

private:
    Ui::AddressInfoForm *ui;
};

#endif // ADDRESSINFOFORM_H
