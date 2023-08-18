#pragma once //only includes this file once

#include <iostream>
#include <fstream>
#include <string>
#include <QMainWindow>
#include <vector>
#include "boost/lexical_cast.hpp"
#include <QMessageBox>
#include <thread>
#include <future>

#include <controllerclass.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
     void setUsertype(std::string usertype){userType = usertype;};
    ~MainWindow();

private slots:

    void on_loginButton_clicked();

    void on_backToHomeButton_clicked();

    void on_createAccountButton_clicked();

    void on_createDriver_clicked();

    void on_createTC_clicked();

    void on_createCO_clicked();

    void clearLoginInputs();
    void clearOrderInputs();
    void clearRegistrationInputs();
    void hideButtons();

    void on_quit_clicked();

    void on_calculateCost_clicked();

    void on_placeOrderBTN_clicked();

    void on_createOrderBTN_clicked();

    void on_logoutBTN_clicked();

    void on_openOrdersBTN_clicked();

    void on_jobOffersBTN_clicked();

    void on_TCComboBox_activated(const QString &transportcompRate);

    void on_TCorders_activated(const QString &selectedOrder);

    void on_AssignDriver_clicked();

    void on_BackBTN_clicked();

    void on_OrderSelection_activated(const QString &driverOrderSelected);

    void on_OrderBack_clicked();



    void on_acceptOrder_clicked();

    void on_rejectOrder_clicked();

    void on_backBTN_clicked();

    void on_backBTN_2_clicked();

    void on_currentJobBTN_clicked();

    void on_assignedOrders_activated(const QString &assignedOrderSelection);

    void on_loadingBTN_clicked();

    void on_onRouteBTN_clicked();

    void on_deliveredBTN_clicked();

    void on_pendingDeliveriesBTN_clicked();

    void on_pendingOrders_activated(const QString &cargoownerOrderSelected);

    void on_backBTN_3_clicked();

    void on_orderHistoryBTN_clicked();

    void on_backBTN_4_clicked();

    void setOrderInfoDisplay(std::vector<std::string> orderv);

    void on_orderhistory_activated(const QString &selectedOrder);

    void on_backBTN_5_clicked();

    void on_accountInfo_clicked();

    void on_changeunBTN_clicked();

    void on_changepwBTN_clicked();

    void on_changeemailBTN_clicked();

    void on_changecontactnumBTN_clicked();

    void on_changecompPriceBTN_clicked();

private:
    Ui::MainWindow *ui;
    Controller controller;
    std::string selectedDriverOrderID;
    float cost;
    int lorryIndex;
    std::string driverAssignedOrderID;
    QString orderinfoOut;
    float transportcompRate;
    std::string transportCompSelected;
    std::vector<std::string> SelectedTCorder;
    std::vector<std::vector<std::string>> transportcompanyOrders;
    std::vector<std::vector<std::string>> driverOrders;
    std::vector<std::vector<std::string>> cargoOwnerOrders;
    std::vector<std::vector<std::string>> orderHistory;
    std::string userType; //used for temporary storage of which user type is being created in the account creation screen
};
