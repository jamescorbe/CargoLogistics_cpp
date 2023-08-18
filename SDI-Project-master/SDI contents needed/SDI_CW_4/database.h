#pragma once //only includes this file once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <functional>
#include <cassert>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <pqxx/pqxx>
#include <QDebug>
#include <QCryptographicHash>
#include <QFlags>
#include <vector>
#include <thread>
#include <future>
#include "postgresql/libpq-fe.h"
#include "userclass.h"
#include "orderclass.h"

class Database{
private:
    std::string connection_string;

public:
    Database();
    Database(std::string connectString); //constructor
    void startDBtest();
    void encryptAllPWS();
    std::vector<std::string> getTransportcompanys();
    void writeUser(const std::shared_ptr<User>& user);
    void writeOrder(const std::shared_ptr<User>& user,const Order &order,std::string transportCompSelected);
    void addDriverSelectiontoOrder(std::string driverName,std::string OrderID);
    void acceptOrder(std::string driverName,std::string orderID);
    void rejectOrder(std::string orderID);
    void updateOrderStatus(std::string orderStatusChosen,std::string orderID);
    std::vector<std::vector<std::string>> readOrderHistory();
    void updateUserDetails(std::string username, std::string updateString, std::string updateID);
    std::string  encryptorDecryptPassword(std::string Password);
    bool writeMsg(const std::string& username, const std::string& msg);

    bool deleteUser(const std::string username);

   std::shared_ptr<User> readUser(const std::string& username, const std::string& password);
   std::vector<std::vector<std::string>> readCOOrders(const std::string& username);
   std::vector<std::vector<std::string>> readTCOrders(const std::string& username);
   std::vector<std::vector<std::string>> readDrivers();
   std::vector<std::vector<std::string>> getDriverOrders(std::string driverName,int callLocation);
};
