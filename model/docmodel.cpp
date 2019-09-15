#include "docmodel.h"

namespace Model {
    DocModel::DocModel()
    {
        this->yourInfo = new ConctactInfo();
        this->clientInfo = new ConctactInfo();
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

    void DocModel::updateYourInfo(QString* name,
                                  QString* street,
                                  Qstring* city,
                                  QString* state,
                                  int zip,
                                  QString* phone,
                                  QString* email)
    {
        
    }
}
