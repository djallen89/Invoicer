#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <QVector>
#include <QDate>
#include <QString>
#include "lineitem.h"

namespace Model {
    class ItemCatalog
    {
    public:
        ItemCatalog();
        ~ItemCatalog();
        void populate_catalog(QVector<LineItem> lineitems);

    private:
        void recalulate_total();
        QVector<QDate> dates;
        QVector<bool> isTime;
        QVector<double> quantity;
        QVector<double> unitCost;
        QVector<double> subtotal;
        QVector<QString*> description;
    };
}

#endif // ITEMCATALOG_H
