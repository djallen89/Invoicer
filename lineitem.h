#ifndef LINEITEM_H
#define LINEITEM_H

#include <QWidget>
#include <QLabel>
#include <QDateEdit>
#include <QLineEdit>
#include <QTextEdit>
#include <QSize>
#include <QCheckBox>

class LineItem : public QWidget
{
    Q_OBJECT

public:
    explicit LineItem(QWidget *parent = nullptr);
    ~LineItem();
    QWidget *container;
    QSize sizeHint() const;

private slots:

private:
    QLabel *dateLabel;
    QLabel *hoursLabel;
    QLabel *descriptionLabel;
    QDateEdit *date;
    QLineEdit *hours;
    QTextEdit *description;
    QCheckBox *selected;
};

#endif // LINEITEM_H
