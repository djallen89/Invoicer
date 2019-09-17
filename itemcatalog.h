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
        QVector<double> hours;
        QVector<double> values;
    };
}

#endif // ITEMCATALOG_H
