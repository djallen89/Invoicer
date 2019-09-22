#ifndef ADDRESSINFOFORM_H
#define ADDRESSINFOFORM_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QJsonObject>

QString sanitize(QString text);

class AddressInfoForm : public QGroupBox
{
    Q_OBJECT

public:
    explicit AddressInfoForm(QString entityName, QWidget *parent = nullptr);
    ~AddressInfoForm();
    QString getName() const;
    QString getAddress() const;
    QString getCity() const;
    QString getState() const;
    QString getZip() const;
    QString getPhone() const;
    QString getEmail() const;
    bool read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QString buildLatex() const;

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
