#include "docmodel.h"

namespace Model {
    DocModel::DocModel()
    {
        this->yourInfo =   new ContactInfo();
        this->clientInfo = new ContactInfo();
        invoiceNumber = new QString();
        catalog = new ItemCatalog();
        currency = new QString();
        total = 0;
    }

    DocModel::~DocModel()
    {
        delete yourInfo;
        delete clientInfo;
        delete invoiceNumber;
        delete catalog;
        delete currency;
    }

    void DocModel::updateInvoiceNumber(QString *newNumber)
    {
        if (invoiceNumber) {
            delete invoiceNumber;
            invoiceNumber = newNumber;
        }
    }

    void DocModel::updateCurrency(QString *newCurrency)
    {
        if (currency) {
            delete currency;
            currency = newCurrency;
        }
    }
}
