#include "addressinfoform.h"
#include <QFormLayout>
#include <QSizePolicy>
#include <QStringBuilder>

AddressInfoForm::AddressInfoForm(QString entityName, QWidget *parent) :
    QGroupBox(entityName, parent)
{
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
}

QString AddressInfoForm::getName() const
{
    return name->text();
}

QString AddressInfoForm::getAddress() const
{
    return streetAddress->text();
}

QString AddressInfoForm::getCity() const
{
    return city->text();
}

QString AddressInfoForm::getState() const
{
    return state->text();
}

QString AddressInfoForm::getZip() const
{
    return zip->text();
}

QString AddressInfoForm::getPhone() const
{
    return phone->text();
}

QString AddressInfoForm::getEmail() const
{
    return email->text();
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
    json["name"] = getName();
    json["street_address"] = getAddress();
    json["city"] = getCity();
    json["state"] = getState();
    json["zip"] = getZip();
    json["phone"] = getPhone();
    json["email"] = getEmail();
}

QString AddressInfoForm::buildLatex() const
{
    /*
\begin{minipage}{0.5\textwidth}
  \LARGE
  Dominick Allen \\
  \normalsize
  784 N. Milton Grove Rd \\
  Elizabethtown, Pa 17022 \\
  \textbf{Phone} \\
  (814) 753-1345 \\
  \textbf{Email} \\
  \href{mailto:dominick.allen1989@gmail.com}{dominick.allen1989@gmail.com}
\end{minipage}
    */

    const QString begin = QStringLiteral("\\begin{minipage}{0.5") % QStringLiteral("\\textwidth}");
    const QString end = QStringLiteral("\\end{minipage}");
    const QString newline = QString("\n");
    const QString texNewline = QString("\\""\\");
    const QString totalNewline = newline % texNewline;
    const QString hyperref = QStringLiteral("\\href{mailto:");
    const QString emailAddress = getEmail();

    return begin % newline
        % getName() % totalNewline
        % getAddress() % totalNewline
        % QStringLiteral("Phone") % totalNewline
        % getPhone() % totalNewline
        % QStringLiteral("Email") % totalNewline
        % hyperref % emailAddress % QStringLiteral("}{") % emailAddress % QStringLiteral("}") % totalNewline
        % end % newline;
}

