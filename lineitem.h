#ifndef LINEITEM_H
#define LINEITEM_H

#include <QWidget>
#include <QLabel>
#include <QDateEdit>
#include <QLineEdit>
#include <QTextEdit>
#include <QSize>
#include <QCheckBox>
#include <QRadioButton>
#include <QJsonObject>
#include <cstdint>
#include <climits>

const double MAXHOURS = 24.0;
const double MINHOURS = 0.02; // roughly one minute

const double MINQ = 1;
const double MAXQ = static_cast<double>(INT_MAX);

const double MINCOST = static_cast<double>(INT_MIN);
const double MAXCOST = static_cast<double>(INT_MAX);

class LineItem : public QWidget
{
    Q_OBJECT

public:
    explicit LineItem(int idx, QWidget *parent = nullptr);
    ~LineItem();
    QSize sizeHint() const;
    bool is_selected() const;
    double quantify() const;
    double rate() const;
    double subtotal() const;
    QString buildLatex() const;
    QString sanitizeDescription() const;

signals:
    void selectedChanged(bool s);
    void subtotalChanged();
                           
public slots:
    bool read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void setSelected();
    void changeToHours();
    void changeToProduct();
    void calculateSubtotal();
    void updateIndex(int idx);

private:
    int index;
    QCheckBox *selected;
    QLabel *dateLabel;
    QDateEdit *date;
    QRadioButton *timeSelect;
    QRadioButton *productSelect;
    QLabel *quantityLabel;
    QLineEdit *quantity;
    QLabel *unitCostLabel;
    QLineEdit *unitCost;
    QLabel *subtotalLabel;
    QLabel *descriptionLabel;
    QTextEdit *description;
};

#endif // LINEITEM_H
