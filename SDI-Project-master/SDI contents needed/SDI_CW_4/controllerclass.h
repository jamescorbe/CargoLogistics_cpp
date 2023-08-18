#pragma once //only includes this file once

#include <iostream>
#include <database.h>
#include <userclass.h>
#include <regex>
#include "boost/lexical_cast.hpp"
#include <memory>
#include <vector>
#include <qdebug.h>
#include <orderclass.h>
#include <thread>
#include <future>


class Controller
{
private:
    std::shared_ptr<User> pUser;
    Database db;
    Order order;
public:
    Controller();

    bool signIn(std::string username, std::string password);
    bool signOut();

    std::vector<std::string> getTransportcomp();
    bool validateAccount(std::string username, std::string password);
    bool validateUsername(std::string usernameInput);
    bool validatePassword(std::string password);
    void passUpdatedUserDetails(std::string updateString,int callLocation);
    bool validateEmail(std::string email);
    int validateOrderDimension(std::string orderDimensionsandWeight[], bool frozen, bool fragile,float transportcompRate);
    bool validateCnum(std::string contactNum);
    bool validateOrder(std::string sourceAddress[],std::string destinationAddress[], float orderDimentionsandWeight[],bool frozen, bool fragile);
    bool validateAddress(std::string numberandstreet, std::string townorcity, std::string county, std::string postcode);
    bool ValidateDriverdetails(std::string CPCnum,std::string drivLicID, std::string NInum,std::string regNum,bool lorryIndex);
    int validatelorryIndex(std::vector<bool>lorryIndexVec);
    bool validatePostCode(std::string postCode);
    void selectDriver(std::vector<std::string> SelectedTCorder);
     std::vector<std::vector<std::string>>getTCOrderList();
     std::vector<std::vector<std::string>>getDriverOrders(std::string driverName,int callLocation);
     std::vector<std::vector<std::string>>getOrderHistory();
     std::vector<std::vector<std::string>>getcargoOwnerOrders(std::string cargoOwnerName);

    std::string getUserType();
    bool validateCPCnum(std::string CPCnum);
    bool validateDrivLicID(std::string drivLicID);
    bool validateNInum(std::string NInum);
    bool validateRegNum(std::string regNum);
    void passOrderAddresses(std::string sourceLocation[], std::string destinationLocation[]);
    void passOrderDetails(std::string transportCompSelected);
    void acceptorRejectOrder(bool acceptOrder,std::string driverName,std::string orderID);
    void updateOrderStatus(int callLocation,std::string orderID);
    void setDriverChosen(std::string driverName,std::string orderID);
    std::string passUsername();

    bool createUser(std::string userType, std::string username, std::string password, std::string email, std::string contactNumber, std::vector<std::string> driverStrings,std::vector<std::string> address,int lorryIndex);



    bool CreateLorry(std::string lorryType, std::string lorryRegNum);
    bool deleteThisUser();

    bool createOrder();

    bool writeUserToDB(std::shared_ptr<User>& user); //create user types
    bool writeOrderToDB(std::shared_ptr<User>& user, const Order& order);
    bool writeMsgToDB(const std::string& username, const std::string& msg); //sendMessage
    bool writeLorryToDB(const User& user, const Lorry& lorry); //createDriver

    bool deleteUserFromDB(const std::string username);


    User readUserFromDB(const std::string& username);
    std::list<Order> readOrdersFromDB(const std::string& username);
    std::list<std::string> readMessagesFromDB(const std::string& username);
    Lorry readLorryFromDB(const std::string& username);
};
