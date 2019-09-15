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
                    QString* state_init, int zip_init,
                    int phone_init, QString* email_init);
        ~ContactInfo();
        
    private:
        QString* name;
        QString* street;
        QString* city;
        QString* state;
        int zip;
        int phone;
        QString* email;
        QString* country;
    };
}
#endif // CONTACTINFO_H
