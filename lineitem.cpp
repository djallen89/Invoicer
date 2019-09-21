#include "lineitem.h"
#include <QGridLayout>
#include <QDate>
#include <QString>
#include <QSizePolicy>
#include <QDoubleValidator>
#include <QStringBuilder>

LineItem::LineItem(int idx, QWidget *parent) :
    QWidget(parent)
{
    index = idx;
    selected = new QCheckBox(QString::number(idx + 1));

    dateLabel = new QLabel("Date");

    auto today = QDate::currentDate();
    date = new QDateEdit();
    date->setDate(today);
    date->setCalendarPopup(true);

    timeSelect = new QRadioButton("Time", this);
    timeSelect->setChecked(true);
    productSelect = new QRadioButton("Product", this);

    quantityLabel = new QLabel("Hours");
    quantity = new QLineEdit();
    quantity->setValidator(new QDoubleValidator(MINHOURS, MAXHOURS, 2, this));

    unitCostLabel = new QLabel("Rate");
    unitCost = new QLineEdit();
    unitCost->setValidator(new QDoubleValidator(MINCOST, MAXCOST, 2, this));

    subtotalLabel = new QLabel("Subtotal: ");

    descriptionLabel = new QLabel("Description");
    description = new QTextEdit();

    auto grid = new QGridLayout;

    grid->addWidget(selected,         0, 0,  1,  1);
    grid->addWidget(dateLabel,        0, 1,  1,  1);
    grid->addWidget(date,             0, 2,  1,  1);

    grid->addWidget(timeSelect,       0, 3,  1,  1);
    grid->addWidget(productSelect,    1, 3,  1,  1);

    grid->addWidget(quantityLabel,    0, 4,  1,  1);
    grid->addWidget(quantity,         0, 5,  1,  1);

    grid->addWidget(unitCostLabel,    0, 6,  1,  1);
    grid->addWidget(unitCost,         0, 7,  1,  1);

    grid->addWidget(subtotalLabel,    0, 8,  1,  1);

    grid->addWidget(descriptionLabel, 2, 0,  1,  1);

    grid->addWidget(description,      3, 0, -1, -1);

    setLayout(grid);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(600, 200);

    connect(selected, &QCheckBox::toggled, this, &LineItem::setSelected);
    connect(quantity, &QLineEdit::editingFinished, this, &LineItem::calculateSubtotal);
    connect(unitCost, &QLineEdit::editingFinished, this, &LineItem::calculateSubtotal);
    connect(timeSelect, &QRadioButton::toggled, this, &LineItem::changeToHours);
    connect(productSelect, &QRadioButton::toggled, this, &LineItem::changeToProduct);
}

QSize LineItem::sizeHint() const {
    return QSize(300, 300);
}

LineItem::~LineItem()
{
    delete dateLabel;
    delete descriptionLabel;
    delete date;
    delete quantityLabel;
    delete quantity;
    delete unitCostLabel;
    delete unitCost;
    delete description;
    delete selected;
    delete timeSelect;
    delete subtotalLabel;
    delete productSelect;
}

/* Signals */


/* Slots */
void LineItem::setSelected() {
    emit selectedChanged(selected->isChecked());
}

void LineItem::changeToHours()
{
    quantityLabel->setText("Hours");
    unitCostLabel->setText("Rate");
    quantity->setValidator(new QDoubleValidator(MINHOURS, MAXHOURS, 2, this));
}

void LineItem::changeToProduct()
{
    quantityLabel->setText("Quantity");
    unitCostLabel->setText("Unit Price");
    quantity->setValidator(new QDoubleValidator(MINQ, MAXQ, 0, this));
}

void LineItem::calculateSubtotal()
{
    auto q = quantify();
    auto r = rate();
    auto subtotal = QString::number(q * r, 'f', 2);
    subtotalLabel->setText("Subtotal: " + subtotal);
    emit subtotalChanged();
}

/* Public Methods */

bool LineItem::read(const QJsonObject &json)
{
    if (!json.contains("index") ||
        !json.contains("selected") || 
        !json.contains("date") || 
        !json.contains("time_selected") || 
        !json.contains("product_selected") || 
        !json.contains("quantity") || 
        !json.contains("unit_cost") ||
        !json.contains("description")
        )
    {
        qWarning("Invalid line item");
        return false;
    }

    if (!json["index"].isDouble() ||
        !json["selected"].isBool() ||
        !json["date"].isString() ||
        !json["time_selected"].isBool() ||
        !json["product_selected"].isBool() ||
        !json["quantity"].isDouble() ||
        !json["unit_cost"].isDouble() ||
        !json["description"].isString()
        )
    {
        qWarning("Invalid line item types");
        return false;
    }

    bool time_s = json["time_selected"].toBool();
    bool product_s = json["product_selected"].toBool();

    if (time_s != product_s) {
        timeSelect->setChecked(time_s);
        productSelect->setChecked(product_s);
    } else {
        qWarning("Invalid selection for time or product; must select one and only one."); 
        return false;
    }

    double q = json["quantity"].toDouble();
    double r = json["unit_cost"].toDouble();
    double st = q * r;
    
    index = static_cast<int>(json["index"].toDouble());
    selected->setChecked(json["selected"].toBool());
    date->setDate(QDate::fromString(json["date"].toString()));
    quantity->setText(QString::number(q));
    unitCost->setText(QString::number(r));
    subtotalLabel->setText("Subtotal: " + QString::number(st, 'f', 2));
    description->setText(json["description"].toString());

    return true;
}

void LineItem::write(QJsonObject &json) const
{
    json["index"] = index;
    json["selected"] = is_selected();
    json["date"] = date->date().toString();
    json["time_selected"] = timeSelect->isChecked();
    json["product_selected"] = productSelect->isChecked();
    json["quantity"] = quantify();
    json["unit_cost"] = rate();
    json["description"] = description->toPlainText();
}

bool LineItem::is_selected() const
{
    return selected->isChecked();
}

double LineItem::quantify() const {
    return quantity->text().toDouble();
}

double LineItem::rate() const {
    return unitCost->text().toDouble();
}

double LineItem::subtotal() const {
    return subtotalLabel->text().toDouble();
}

void LineItem::updateIndex(int idx) {
    index = idx;
    selected->setText(QString::number(idx + 1));
}

QString LineItem::buildLatex() const
{
    //auto table_header = "Date & Hours/Product & Quantity & Rate/Cost & Description""\\""\\""\\hline\n";
    auto amp = QString(" & ");
    auto desc_text = sanitizeDescription();
    //auto desc_text = description->toPlainText().replace(QString("\n"), QString("\n\n"));
    auto h_or_p = timeSelect->isChecked() ? QString("& H &") : QString("& P &");
    return date->date().toString("  MM/dd") % amp % h_or_p % amp %  quantity->text()
        % amp % unitCost->text() % amp % "\n  "
        % desc_text % QString("\\""\\""\\hline\n\n");
}

QString LineItem::sanitizeDescription() const
{
    auto desc_text = description->toPlainText();
    desc_text = desc_text.replace(QString("\\"), QString(""));
    desc_text = desc_text.replace(QRegExp("([&#\\$%_{}\\^])"), "\\1");
    desc_text = desc_text.replace(QString("\n"), QString("\n\n"));
    return desc_text;
}
