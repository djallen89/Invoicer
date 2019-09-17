#ifndef CONTACTINFO_H
#define CONTACTINFO_H

#include <QString>

namespace Model {
    class ContactInfo
    {
    public:
        ContactInfo();
        ContactInfo(QString* name_init,
                    QString* street_init, QString* city_init,
                    QString* state_init, unsigned int zip_init,
                    unsigned long int phone_init, QString* email_init);
        ~ContactInfo();
        QString* name;
        QString* street;
        QString* city;
        QString* state;
        unsigned int zip;
        unsigned long int phone;
        QString* email;
        QString* country;

        QString renderZip() {
            return QString(zip);
        }
    };
}
#endif // CONTACTINFO_H
