#include "lineitem.h"
#include <QGridLayout>
#include <QDate>
#include <QString>
#include <QSizePolicy>

LineItem::LineItem(QWidget *parent) :
    QWidget(parent)
{
    container = new QWidget();

    dateLabel = new QLabel("Date");
    hoursLabel = new QLabel("Hours");
    descriptionLabel = new QLabel("Description");

    auto today = QDate::currentDate();
    date = new QDateEdit();
    date->setDate(today);
    date->setCalendarPopup(true);

    hours = new QLineEdit();
    
    description = new QTextEdit();

    selected = new QCheckBox("Selected");
    
    auto grid = new QGridLayout;
    grid->addWidget(dateLabel, 0, 0, 1, 1);
    grid->addWidget(date, 0, 1, 1, 1);
    grid->addWidget(hoursLabel, 1, 0, 1, 1);
    grid->addWidget(hours, 1, 1, 1, 1);
    grid->addWidget(selected, 0, 3, 3, 2);
    grid->addWidget(descriptionLabel, 2, 0, 1, 1);
    grid->addWidget(description, 3, 0, -1, -1);
    grid->setColumnMinimumWidth(2, 200);
    container->setLayout(grid);
    container->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    container->setMinimumSize(600, 200);
}

QSize LineItem::sizeHint() const {
    return QSize(300, 300);
}

LineItem::~LineItem()
{
    delete dateLabel;
    delete hoursLabel;
    delete descriptionLabel;
    delete date;
    delete hours;
    delete description;
    delete selected;
    delete container;
}
