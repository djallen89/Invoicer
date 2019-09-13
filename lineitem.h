#ifndef LINEITEM_H
#define LINEITEM_H

#include <QWidget>
#include <Q

namespace Ui {
class LineItem;
}

class LineItem : public QWidget
{
    Q_OBJECT

public:
    explicit LineItem(QWidget *parent = nullptr);
    ~LineItem();

private:
    
};

#endif // LINEITEM_H
