#include "contactinfo.h"
namespace Model {
    ContactInfo::ContactInfo()
    {
        name = new QString("");
        street = new QString("");
        city = new QString("");
        state = new QString("");
        zip = 0;
        phone = 0;
        email = new QString("");
        country = new QString("");
    }

    ContactInfo::ContactInfo(QString* name_init,
                             QString* street_init, QString* city_init,
                             QString* state_init, unsigned int zip_init,
                             unsigned long int phone_init, QString* email_init)
    {
        name = name_init;
        street = street_init;
        city = city_init;
        state = state_init;
        zip = zip_init;
        phone = phone_init;
        email = email_init;
        country = new QString("");
    }

    ContactInfo::~ContactInfo()
    {
        delete name;
        delete street;
        delete city;
        delete state;
        delete email;
        delete country;
    }
}
 
