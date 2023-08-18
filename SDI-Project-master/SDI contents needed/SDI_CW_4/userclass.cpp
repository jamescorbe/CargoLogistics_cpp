#include "userclass.h"

/*!
* Used for setting the user address when they sign in or create an account.
*/
void User::setAddress(std::string numberAndStreet, std::string townOrCity, std::string county, std::string postCode)
{
    address.at("num&street") = numberAndStreet;
    address.at("city") = townOrCity;
    address.at("county") = county;
    address.at("postCode") = postCode;
}

/*!
* Used for setting cargo owner details
*/
CargoOwner::CargoOwner(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp){
    setUsername(uNameInp);
    setPassword(pWordInp);
    setEmail(emailInp);
    setContactNumber(cNumInp);
}

/*!
* Used for setting TransportComp details.
*/
TransportComp::TransportComp(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp) {
    setUsername(uNameInp);
    setPassword(pWordInp);
    setEmail(emailInp);
    setContactNumber(cNumInp);
}

/*!
* Used for setting Driver details.
*/
Driver::Driver(std::string uNameInp, std::string pWordInp, std::string emailInp, std::string cNumInp, std::string NInumberInp, std::string drivLicIDInp, std::string CPCnumInp, //constructor. account related details
               std::string lorryRegNumInp,int lorryIndex)  //lorry related details
            : NInumber(NInumberInp), drivLicID(drivLicIDInp), CPCnum(CPCnumInp), //Driver class variable assignment
              lorry(Lorry(lorryIndex,lorryRegNumInp)){ //Lorry class constructor, returns the lorry object into the variable
    setUsername(uNameInp);
    setPassword(pWordInp);
    setEmail(emailInp);
    setContactNumber(cNumInp);
        }

