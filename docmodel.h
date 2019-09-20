#ifndef DOCMODEL_H
#define DOCMODEL_H

#include <QString>
#include <QVector>
#include "addressinfoform.h"
#include "lineitem.h"
#include "contactinfo.h"
#include "itemcatalog.h"
#include "hpdf.h"
#include <iostream>

namespace Model {
    class DocModel
    {
    public:
        DocModel();
        ~DocModel();
        void createPDF();
        // void populateCatalog(); args tbd
        // void updateTotal(); tbd

    public slots:
        /*
        void updateInvoiceNumber(QString *newNumber);
        void updateCurrencyName(QString *newCurrency);
        void updateCurrencySymbol(QString *newCurrency);
        void updateYourInfo(AddressInfoForm* yourInfoForm);
        void updateClientInfo(AddressInfoForm* clientInfoForm);
        */
        
    private:
        static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *data) {
            //QString warningMsg = "Status Error " + QString::number(error_no) + "Detail: " + QString::number(detail_no);
            std::cout << "Status Error " << error_no << "Detail: " << detail_no;
            throw std::exception();
        }
        
        QString *invoiceNumber;
        QString *currency;
        QString *currencySymbol;
        ContactInfo *yourInfo;
        ContactInfo *clientInfo;
        ItemCatalog *catalog;
        int total;
    };
}
#endif // DOCMODEL_H
