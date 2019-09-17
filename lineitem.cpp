#include "lineitem.h"
#include <QGridLayout>
#include <QDate>
#include <QString>
#include <QSizePolicy>
#include <QDoubleValidator>

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
/*
*/

/* Slots */
void LineItem::setSelected() {
    emit selectedChanged(selected->isChecked());
}

void LineItem::changeToHours()
{
    quantityLabel->setText("Hours");
    quantity->setValidator(new QDoubleValidator(MINHOURS, MAXHOURS, 2, this));
}

void LineItem::changeToProduct()
{
    quantityLabel->setText("Quantity");
    quantity->setValidator(new QDoubleValidator(MINQ, MAXQ, 0, this));
}

void LineItem::calculateSubtotal()
{
    auto q = quantify();
    auto r = rate();
    auto subtotal = QString::number(q * r, 'f', 2);
    subtotalLabel->setText("Subtotal: " + subtotal);
}

/* Public Methods */
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

void LineItem::updateIndex(int idx) {
    index = idx;
    selected->setText(QString::number(idx + 1));
}
