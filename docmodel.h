#ifndef DOCMODEL_H
#define DOCMODEL_H

#include <QString>
#include <QVector>
#include "contactinfo.h"
#include "itemcatalog.h"

namespace Model {
    class DocModel
    {
    public:
        DocModel();
        ~DocModel();
        void updateInvoiceNumber(QString *newNumber);
        void updateCurrency(QString *newCurrency);
        // void populateCatalog(); args tbd
        // void updateTotal(); tbd

        ContactInfo *yourInfo;
        ContactInfo *clientInfo;

    private:
        ItemCatalog *catalog;
        QString *invoiceNumber;
        QString *currency;
        int total;
    };
}
#endif // DOCMODEL_H
