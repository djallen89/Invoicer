#ifndef DOCMODEL_H
#define DOCMODEL_H

#include <QString>
#include <QVector>
#include "model/contactinfo.h"
#include "itemcatalog.h"

namespace Model {
    class DocModel
    {
    public:
        DocModel();
        ~DocModel();
        void updateYourInfo();
        void updateClientInfo();
        void updateInvoiceNumber();
        void updateCurrency();
        void populateCatalog();
        void updateTotal();

    private:
        ContactInfo *yourInfo;
        ContactInfo *clientInfo;
        QString *invoiceNumber;
        ItemCatalog *catalog;
        QString *currency;
        int total;
    };
}
#endif // DOCMODEL_H
