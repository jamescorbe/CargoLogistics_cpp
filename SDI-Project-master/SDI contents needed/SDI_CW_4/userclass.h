#pragma once //only includes this file once

#include "orderclass.h"
#ifndef USERCLASS_H
#define USERCLASS_H

class User {
    private:
        std::string username, password, email, contactNumber; //initialising user variables
        std::map<std::string, std::string> address = { //necessary data for all users
            {"num&street", ""},
            {"city", ""},
            {"county", ""},
            {"postCode", ""}
        };
        std::vector<std::string> messages;
    public:

        virtual ~User(){} //virtual destructor, makes class polymorphic
        virtual void setAddress(std::string numberAndStreet, std::string townOrCity, std::string county,std::string postCode);

        virtual const std::string& getUsername() const { return username; } //accessors
        const std::string& getPassword() const { return password; }
        const std::string& getEmail() const { return email; }
        const std::string& getContactNumber() const { return contactNumber; }
        const std::map<std::string, std::string>& getAddress() const {return address; }
        const std::string getAddressFormat() const {return "address format incomplete"; } //address needs to return map (4 lines)
        const std::string& getAddressLine1() const {return address.at("num&street"); }
        const std::string& getAddressLine2() const {return address.at("city"); }
        const std::string& getAddressLine3() const {return address.at("county"); }
        const std::string& getAddressLine4() const {return address.at("postCode"); }

        void setUsername(const std::string uNameInp) { username = uNameInp; } //mutators
        void setPassword(const std::string pWordInp) { password = pWordInp; }
        void setEmail (const std::string emailInp) { email = emailInp; }
        void setContactNumber (const std::string cNumInp) { contactNumber = cNumInp; }
};

class CargoOwner: public User {
    public:
        CargoOwner(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp);
};

class TransportComp: public User {
    public:
        TransportComp(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp);

};

class Driver: public User {
    private:
        std::string NInumber, drivLicID, CPCnum;
        Lorry lorry;

        void acceptOrder(int index);
        void declineOrder(int index, TransportComp company);

    public:
        Driver(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp, std::string NInumberInp, std::string drivLicIDInp, std::string CPCnumInp, //constructor. account related details
               std::string lorryRegNumInp, int lorryIndex);

        void manageOrder(int index);

        const std::string& getCPCnumber() const { return CPCnum; }
        const std::string& getNInumber() const { return NInumber; }
        Lorry getlorry() const { return lorry;}
        const std::string& getDrivLicID() const { return drivLicID; }


};
#endif // USERCLASS_H
