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
                    QString* state_init, QString* zip_init,
                    QString* phone_init, QString* email_init);
        ~ContactInfo();
        QString* name;
        QString* street;
        QString* city;
        QString* state;
        QString* zip;
        QString* phone;
        QString* email;
        QString* country;

    };
}
#endif // CONTACTINFO_H
