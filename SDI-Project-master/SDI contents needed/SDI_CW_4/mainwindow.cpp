#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userclass.h"
#include "controllerclass.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), controller() { //main window constructor
    ui ->setupUi(this);
   // ui ->loginstack->addWidget();
    ui->accountInfo->hide();
    this ->transportcompRate =0;
    this-> cost = 0;
    std::vector<std::string> transportcomp;
     if(transportcomp.empty())
     {
        transportcomp = controller.getTransportcomp();
     }
    QWidget* hidedriverwidget = new QWidget();
    QHBoxLayout * Drivercredentials = new QHBoxLayout(hidedriverwidget);
    Drivercredentials ->addWidget(new QLabel("driverDetails"));
    hidedriverwidget->setVisible(false);
    ui->username->setPlaceholderText("Enter a Username");
    ui->password->setPlaceholderText("Enter a Password");
    ui->email->setPlaceholderText("Enter your email");
    ui->contactnumber->setPlaceholderText("Enter your phone number");
    ui->numberandStreet->setPlaceholderText("Enter your building number and street");
    ui->townorcity->setPlaceholderText("Enter Town/City");
    ui->county->setPlaceholderText("Enter County");
    ui->postcode->setPlaceholderText("Enter Postcode");

    userType = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * on_createAccountButton_clicked retrieves all the user inputs for account creation, passes them to be validated in the controller and returns output errors to the user if
 * the inputs are incorrect. Once all inputs are correct, the controller class is called with the user inputs so that a user can be created in the system and on the database.
 */
void MainWindow::on_createAccountButton_clicked()
{
    bool validInputs = true;
    std::string outputErrors = "";
    std::vector<std::string> address;
    std::string usernameInput = ui->username->text().toStdString();
    std::string passwordInput = ui->password->text().toStdString();
    std::string emailInput = ui->email->text().toStdString();
    std::string contactNumberInput = ui->contactnumber->text().toStdString();
    std::string numberAndStreet = ui->numberandStreet->text().toStdString();
    std::string townOrCity = ui->townorcity->text().toStdString();
    std::string county  = ui->county->text().toStdString();
    std::string postCode  = ui->postcode->text().toStdString();
    std::string cpcNumber  = ui->cpcnumber->text().toStdString();
    std::string drivinglicenceID  = ui->drivinglicenceid->text().toStdString();
    std::string NInumber  = ui->ninumber->text().toStdString();
    std::string lorryRegNum = ui->regnum->text().toStdString();
    address.push_back(numberAndStreet);
    address.push_back(townOrCity);
    address.push_back(county);
    address.push_back(postCode);
    try{

    //Goes through all inputs and checks if the value is not "" and for some values, checks if the value passes validation.
    //If a value is empty or doesn't pass validation a message will be appended to outputErrors to be displayed to the user.

    if (usernameInput == ""){
        validInputs = false;
        outputErrors += "Please enter a username.\n";
    } else if (controller.validateUsername(usernameInput)){ //if username is already taken
        validInputs = false;
        outputErrors += "Username already taken.\n";
    }

    if (passwordInput == ""){
        validInputs = false;
        outputErrors += "Please enter a password.\n";
    } else if (!controller.validatePassword(passwordInput)){ //check password against standards
        validInputs = false;
        outputErrors += "Password needs to be between 6 and 25 charicters.\n";
    }

    if (emailInput == ""){
        validInputs = false;
        outputErrors += "Please enter your email.\n";
    } else if (!controller.validateEmail(emailInput)){ //check if the email is in email format
        validInputs = false;
        outputErrors += "Incorrect email format.\n";
    }

    if (contactNumberInput == ""){
        validInputs = false;
        outputErrors += "Please enter your contact number.\n";
    } else if (!controller.validateCnum(contactNumberInput)){ //check if the contact number is in correct format
        validInputs = false;
        outputErrors += "Incorrect contact number format.\n";
    }

    if (numberAndStreet == ""){
        validInputs = false;
        outputErrors += "Please enter the first line of your address.\n";
    }

    if (townOrCity == ""){
        validInputs = false;
        outputErrors += "Please enter the second line of your address.\n";
    }

    if (county == ""){
        validInputs = false;
        outputErrors += "Please enter the third line of your address.\n";
    }

    if (numberAndStreet != "" && townOrCity != "" && county != "" && postCode != "" && controller.validatePostCode(postCode)) {
        if (!controller.validateAddress(numberAndStreet, townOrCity, county, postCode)){
            validInputs = false;
            outputErrors += "Incorrect address.";
        }
    }

    if (postCode == ""){
        validInputs = false;
        outputErrors += "Please enter your post code.\n";
    } else if (!controller.validatePostCode(postCode)){ //check if the post code is in correct format
        validInputs = false;
        outputErrors += "Incorrect post code format.\n";
    }

    if (userType == "Driver"){ //if user is a driver, checks additional fields

       std::vector<bool>lorryIndexVec;
       lorryIndexVec.push_back (ui->twoAxelSmall->isChecked());
       lorryIndexVec.push_back (ui->twoAxel->isChecked());
       lorryIndexVec.push_back (ui->ThreeAxel->isChecked());
       lorryIndexVec.push_back (ui->fourAxel->isChecked());
       lorryIndexVec.push_back (ui->fourAxel->isChecked());
       lorryIndexVec.push_back (ui->twoAxelFrozen->isChecked());
       lorryIndexVec.push_back (ui->threeAxelFrozen->isChecked());
       lorryIndexVec.push_back (ui->sixAxelFrozen->isChecked());

       this->lorryIndex = controller.validatelorryIndex(lorryIndexVec);
       if(lorryIndex == 20)
       {
          validInputs = false;
          outputErrors += "Select 1 lorry Type.\n";
       }
       if(lorryIndex == 18)
       {
            validInputs = false;
            outputErrors +="only select 1 lorrytype.\n";
       }

        if (lorryRegNum == ""){
            validInputs = false;
            outputErrors += "Please enter your lorry's registration number.\n";
        } else if (!controller.validateRegNum(lorryRegNum)){ //check if the post code is in correct format
            validInputs = false;
            outputErrors += "Incorrect registration number format.\n";
        }

    }

    }catch(const std::exception& ex)
    {
     std::cerr << ex.what() << std::endl;
    }

    if (validInputs){ //if the inputs are valid
        std::vector<std::string> driverStrings;

        if (userType == "Driver"){ //if creating a driver, populate the two relevant vectors with input information
            driverStrings.push_back(NInumber);
            driverStrings.push_back(drivinglicenceID);
            driverStrings.push_back(cpcNumber);
            driverStrings.push_back(lorryRegNum);
        }

        if (controller.createUser(userType, usernameInput, passwordInput, emailInput, contactNumberInput, driverStrings,address,lorryIndex )){ //if user is succesfully created

            hideButtons();
            ui->centralStack->setCurrentIndex(2);

            /*!
            * loads user home page
            */

        } else { //if user fails to create
            outputErrors += "Error on account creation.";
        }

    } else { //if the inputs are not valid
        ui->outputErrorsText->setPlainText(QString::fromUtf8(outputErrors.data(), outputErrors.size())); //displays outputErrors to the user
    }
}

/*!
 * on_loginButton_clicked takes the users login inputs, calls the controller to access the database and receives true or false depending on if the entered username and password match that which is found on the database.
 */
void MainWindow::on_loginButton_clicked()
{

    if (controller.validateAccount(ui->usernameLogin->text().toStdString(),ui->passwordLogin->text().toStdString())){ //if the username and password in the login fields successfully validate via the database
        controller.signIn(ui->usernameLogin->text().toStdString(),ui->passwordLogin->text().toStdString());
        userType = controller.getUserType();
        hideButtons();
         ui->centralStack->setCurrentIndex(2);

         /*!
         * loads user home page
         */
        clearLoginInputs(); //clear the login inputs
    } else { //validation failed
        ui->loginErrorsOut->setText("Username or password incorrect.");
    }

}

/*!
* on_createDriver_clicked Takes you to the registration page with the driver details available, setting the user type to 'Driver'
*/
void MainWindow::on_createDriver_clicked() //
{
    ui->centralStack->setCurrentIndex(1);
    ui->driverDetails->setHidden(false);
    clearLoginInputs();
    userType = "Driver";
}

/*!
* on_createTC_clicked Takes you to the registration page with the driver details blocked, setting the user type to 'TransportCompany'
*/
void MainWindow::on_createTC_clicked()
{
    ui->centralStack->setCurrentIndex(1);
    ui->driverDetails->setHidden(true);
    clearLoginInputs();
    userType = "TransportCompany";
}

/*!
* on_createTC_clicked Takes you to the registration page with the driver details blocked, setting the user type to 'CargoOwner'
*/
void MainWindow::on_createCO_clicked()
{
    ui->centralStack->setCurrentIndex(1);
     ui->driverDetails->setHidden(true);
    clearLoginInputs();
    userType = "CargoOwner";
}

/*!
* All back buttons return the user to the user home
*/
void MainWindow::on_backToHomeButton_clicked(){
    ui->centralStack->setCurrentIndex(0);
    clearRegistrationInputs();
    userType = "";
}

/*!
* Clears the users login details for if they want to sign out and back in.
*/
void MainWindow::clearLoginInputs(){
    ui->usernameLogin->clear();
    ui->passwordLogin->clear();
    ui->loginErrorsOut->clear();
}

/*!
* Clears the users Registration details for if they want to sign out and create a new account.
*/
void MainWindow::clearRegistrationInputs(){
    ui->username->clear();
    ui->password->clear();
    ui->contactnumber->clear();
    ui->county->clear();
    ui->email->clear();
    ui->numberandStreet->clear();
    ui->postcode->clear();
    ui->townorcity->clear();
    ui->cpcnumber->clear();
    ui->drivinglicenceid->clear();
    ui->ninumber->clear();
    ui->regnum->clear();
    ui->outputErrorsText->clear();
}

/*!
* Clears the users order inputs.
*/
void MainWindow::clearOrderInputs()
{
    ui->buildingandstreetSL->clear();
    ui->townorcitySL->clear();
    ui->countySL->clear();
    ui->postcodeSL->clear();
    ui->buildingandstreetDL->clear();
    ui->townorcityDL->clear();
    ui->countyDL->clear();
    ui->postcodeDL->clear();
    ui->width->clear();
    ui->length->clear();
    ui->height->clear();
    ui->weight->clear();
}


/*!
* used to close the program when quit is pressed.
*/
void MainWindow::on_quit_clicked()
{
    this->close();
}

/*!
* used to hide all buttons in the user home which they shouldn’t have access to.
*/
void MainWindow::hideButtons()
{
    if(userType == "Driver")
    {
        ui->jobOffersBTN->setVisible(true);
        ui->currentJobBTN->setVisible(true);
        ui->openOrdersBTN->setVisible(false);
        ui->placeOrderBTN->setVisible(false);
        ui->pendingDeliveriesBTN->setVisible(false);
    }
    else if (userType == "CargoOwner")
    {
        ui->placeOrderBTN->setVisible(true);
        ui->pendingDeliveriesBTN->setVisible(true);
        ui->openOrdersBTN->setVisible(false);
        ui->jobOffersBTN->setVisible(false);
        ui->currentJobBTN->setVisible(false);
    }
    else if (userType == "TransportCompany")
    {
        ui->openOrdersBTN->setVisible(true);
        ui->jobOffersBTN->setVisible(false);
        ui->currentJobBTN->setVisible(false);
        ui->placeOrderBTN->setVisible(false);
        ui->pendingDeliveriesBTN->setVisible(false);
    }
}

/*!
* on_calculateCost_clicked takes the users order dimensions, validates them through calling the controller validation class and returns a price if the inputs were valid.
*/
void MainWindow::on_calculateCost_clicked()///< get order size and weight and pass to controller.
{
    std::string dimensionsandweight[4];
    dimensionsandweight[0] = ui->width->text().toStdString();
    dimensionsandweight[1] = ui->length->text().toStdString();
    dimensionsandweight[2] = ui->height->text().toStdString();
    dimensionsandweight[3] = ui->weight->text().toStdString();
    bool frozen =  ui->frozen->isChecked();
    bool fragile =  ui->fragile->isChecked();

   float localcost = controller.validateOrderDimension(dimensionsandweight,frozen, fragile,transportcompRate);
   QString Qweight = QString::fromStdString(dimensionsandweight[3]);

    QString QStringcost = QString::fromStdString(std::to_string(localcost));
    if(localcost == 0)
    {
         ui->costOutput->setText("Invalid order Dimentions or weight");
    }
    if(localcost > 1 && cost < 400)
    {
        ui->costOutput->setText("The Order cost is too small, minimum order cost is 400 cost = £" + QStringcost) ;
    }
    if(localcost == 1)
    {
        ui->costOutput->setText("Minimum order Dimentions or weights not met");
    }
    if (localcost == 2)
    {
        ui->costOutput->setText("order weight is larger the maximum.");
    }
    if (localcost == 3)
    {
         ui->costOutput->setText("Please select a transport company");
    }
    else if(localcost > 400)
    {
        this->cost = localcost;

         ui->costOutput->setText("£" +QStringcost);

    }
}

/*!
* on_placeOrderBTN_clicked changes the view to the order after populating the transport company combo box with all the transport companies and their prices.
*/
void MainWindow::on_placeOrderBTN_clicked()
{
    using boost::lexical_cast;
    using boost::bad_lexical_cast;
    std::string currentUsername;
    std::string CurrentTCprice;
    std::vector<std::string> transportCompDetails = controller.getTransportcomp();
    int TCDlength = transportCompDetails.size();

    for(int i = 0; i < TCDlength; i++)
    {

        QString currentUsername = QString::fromStdString(transportCompDetails.at(i));
        QString CurrentTCprice = QString::fromStdString(transportCompDetails.at(i+1));
        if(CurrentTCprice == "")
        {
            CurrentTCprice = "StandardRate 1.2";
        }
        ui->TCComboBox->addItem(currentUsername + " Price = " +CurrentTCprice +" x");
        i++;

    }
    ui->centralStack->setCurrentIndex(3);
}

/*!
* on_createOrderBTN_clicked passes the order details to the controller classes responsible for setting order details, then order is created once these details have been set and the view is updated.
*/
void MainWindow::on_createOrderBTN_clicked()
{
    std::string sourceLocation[4];
    std::string destinationLocation[4];
    if(this->cost  == 0)
       {
         ui->costOutput->setText("Cost Needs to be calulated before creating order!");
    }
    else
    {
        std::string sourceNumberandStreet = ui->buildingandstreetSL->text().toStdString();
        std::string sourceTownorCity = ui->townorcitySL->text().toStdString();
        std::string sourceCounty  = ui->countySL->text().toStdString();
        std::string sourcePostCode  = ui->postcodeSL->text().toStdString();
       if(controller.validateAddress(sourceNumberandStreet, sourceTownorCity, sourceCounty, sourcePostCode))
       {
           sourceLocation[0] = sourceNumberandStreet;
           sourceLocation[1] = sourceTownorCity;
           sourceLocation[2] = sourceCounty;
           sourceLocation[3] = sourcePostCode;

           std::string destinationNumberandStreet = ui->buildingandstreetDL->text().toStdString();
           std::string destinationTownorCity = ui->townorcityDL->text().toStdString();
           std::string destinationCounty  = ui->countyDL->text().toStdString();
           std::string destinationPostCode  = ui->postcodeDL->text().toStdString();
           if(controller.validateAddress(destinationNumberandStreet, destinationTownorCity, destinationCounty, destinationPostCode))
           {
               destinationLocation[0] = destinationNumberandStreet;
               destinationLocation[1] = destinationTownorCity;
               destinationLocation[2] = destinationCounty;
               destinationLocation[3] = destinationPostCode;

               if( sourceLocation[3] == destinationLocation[3])
               {
                    ui->placeorderOutput->setText("source and destination location can't be the same!");
               }
               else
               {
                   ui->placeorderOutput->setText("Order Created!");

                   controller.passOrderAddresses(sourceLocation, destinationLocation);
                   controller.passOrderDetails(transportCompSelected);
                    ui->centralStack->setCurrentIndex(2);
                    ui->TCComboBox->clear();
                    clearOrderInputs();
              }

           }

           else
           {
                ui->placeorderOutput->setText("destination Location format invalid!");
           }
       }
       else
       {
            ui->placeorderOutput->setText("Source Location format invalid!");
       }


    }

}
/*!
*  logs the user out and returns them to the program start page.
*/
void MainWindow::on_logoutBTN_clicked()
{
     ui->centralStack->setCurrentIndex(0);
     controller.signOut();
}

/*!
*  this is used to help populate the  order info boxed for the program. it works by taking an order input and adding the details of that order to an output string stored in mainwindow
* called orderinfoOut.
*/
void MainWindow::setOrderInfoDisplay(std::vector<std::string> orderv)
{
    orderinfoOut.clear();
    orderinfoOut +="Cargo owner = " +QString::fromStdString(orderv.at(0)) + "\n";
    orderinfoOut +="Driver = " +QString::fromStdString(orderv.at(1)) + "\n";
    orderinfoOut +="Transport Company = " +QString::fromStdString(orderv.at(2)) + "\n"; //to confirm to the user this order is theirs.
    orderinfoOut +="OrderID = " +QString::fromStdString(orderv.at(3)) + "\n";
    orderinfoOut +="Order Status = " +QString::fromStdString(orderv.at(4)) + "\n";
    orderinfoOut +="Width = " +QString::fromStdString(orderv.at(5)) + "\n";
    orderinfoOut +="Length = " +QString::fromStdString(orderv.at(6)) + "\n";
    orderinfoOut +="Height = " +QString::fromStdString(orderv.at(7)) + "\n";
    orderinfoOut +="Weight = " +QString::fromStdString(orderv.at(8)) + "\n";
    orderinfoOut +="Fragile (t/f) = " +QString::fromStdString(orderv.at(9)) + "\n";
    orderinfoOut +="Frozen (t/f) = " +QString::fromStdString(orderv.at(10)) + "\n";
    orderinfoOut +="Total Cost = " +QString::fromStdString(orderv.at(11)) + "\n";
    orderinfoOut +="Source location = " +QString::fromStdString(orderv.at(12)) + "\n";
    orderinfoOut +="Destination location = " +QString::fromStdString(orderv.at(13)) + "\n";
}

/*!
*  on_openOrdersBTN_clicked populates the orders assigned to the transportation company in the TCorders combo box.
*/
void MainWindow::on_openOrdersBTN_clicked()
{

    ui->centralStack->setCurrentIndex(5);
    std::vector<std::vector<std::string>> orderList = controller.getTCOrderList();
    for(std::vector<std::string> orderv : orderList)
    {

        this->transportcompanyOrders.push_back(orderv);

           if(orderv.at(4) == "Created"&& orderv.at(1) =="NONE")
           {
           ui->TCorders->addItem("OrderID= "+QString::fromStdString(orderv.at(3)) + " Cargo Owner ="+QString::fromStdString(orderv.at(0)) + " order cost ="+ QString::fromStdString(orderv.at(11)));
           }


    }
}

/*!
*  on_jobOffersBTN_clicked populates the orders assigned to the driver in the OrderSelection combo box.
*/
void MainWindow::on_jobOffersBTN_clicked()
{
    driverOrders.clear();
    ui->centralStack->setCurrentIndex(4);

    std::vector<std::vector<std::string>> driverOrderList = controller.getDriverOrders(controller.passUsername(),0);
    for(std::vector<std::string> driverOrders : driverOrderList)
    {
        this->driverOrders.push_back(driverOrders);
           if(driverOrders.at(4) == "Created"&& driverOrders.at(1) =="NONE")
           {
           ui->OrderSelection->addItem("OrderID= "+QString::fromStdString(driverOrders.at(3)) + " Cargo Owner ="+QString::fromStdString(driverOrders.at(0)) + " order cost ="+ QString::fromStdString(driverOrders.at(11)));
           }
    }
    if(driverOrderList.empty())
    {
          ui->OrderSelection->addItem("You have no orders alocated");
    }
}

/*!
*  on_TCComboBox_activated Adds the transport company name and price, used when selecting a transport company for the order.
*/
void MainWindow::on_TCComboBox_activated(const QString &pickedtransportcompRate)
{
    if(transportCompSelected =="")
    {
         ui->TCComboBox->removeItem(0);
    }
 using boost::lexical_cast;
 using boost::bad_lexical_cast;
 std::stringstream unsplitTransportcompRate(pickedtransportcompRate.toStdString());
 std::vector<std::string> splitTCRate;
 while (unsplitTransportcompRate.good()) {
     std::string tempstring;
     getline(unsplitTransportcompRate,tempstring, ' ');
     splitTCRate.push_back(tempstring);
 }
this-> transportCompSelected = splitTCRate.at(0);
 try {
    this->transportcompRate= lexical_cast<float>(splitTCRate.at(3));
 } catch (bad_lexical_cast &e) {
    this->transportcompRate = 1.2;
 }

}

/*!
*  on_TCorders_activated Adds all the assigned transport company details for the order selected from the combo box.
*/
void MainWindow::on_TCorders_activated(const QString &selectedOrder)
{
    std::stringstream selectedOrderSS(selectedOrder.toStdString());
    std::vector<std::string> splitselectedOrder;
    while (selectedOrderSS.good()) {
        std::string tempstring;
        getline(selectedOrderSS,tempstring, ' ');
        splitselectedOrder.push_back(tempstring);
    }
    for(std::vector<std::string> orderv : transportcompanyOrders)
    {
        if(orderv.at(3) == splitselectedOrder.at(1))
        {
            setOrderInfoDisplay( orderv);

          ui->TCOrderDetails->setPlainText(orderinfoOut);

             this->SelectedTCorder = orderv;
        }

    }
}

/*!
*  Calls the controller class assign driver function and sets the view back to the user home.
*/
void MainWindow::on_AssignDriver_clicked()
{

 controller.selectDriver(SelectedTCorder);
 ui->centralStack->setCurrentIndex(2);
 ui->TCOrderDetails->clear();
 ui->TCorders->clear();
}



void MainWindow::on_BackBTN_clicked()
{
     ui->centralStack->setCurrentIndex(2);
     ui->TCorders->clear();
}

/*!
*  on_OrderSelection_activated is used to display the driver assigned order information.
*/
void MainWindow::on_OrderSelection_activated(const QString &driverOrderSelected)
{
    std::stringstream driverselectedOrderss(driverOrderSelected.toStdString());
    std::vector<std::string> splitdriverselectedOrder;
    while (driverselectedOrderss.good()) {
        std::string tempstring;
        getline(driverselectedOrderss,tempstring, ' ');
        splitdriverselectedOrder.push_back(tempstring);
    }
for (std::vector<std::string> order :driverOrders)
{
    this->selectedDriverOrderID = splitdriverselectedOrder.at(1);
    if(order.at(3) == splitdriverselectedOrder.at(1))
    {
        setOrderInfoDisplay(order);
         ui->OrderDetails->setPlainText(orderinfoOut);
    }
}
}

void MainWindow::on_OrderBack_clicked()
{
    ui->centralStack->setCurrentIndex(2);
    ui->TCComboBox->clear();
}

/*!
*  on_OrderSelection_activated passes the accept order state to the acceptorRejectOrder function and sets the view to home.
*/
void MainWindow::on_acceptOrder_clicked()
{
    controller.acceptorRejectOrder(true, controller.passUsername(),selectedDriverOrderID);
    ui->centralStack->setCurrentIndex(2);
    driverOrders.clear();
    ui->OrderDetails->clear();
    ui->OrderSelection->clear();

}

/*!
*  on_OrderSelection_activated passes the reject order state to the acceptorRejectOrder function and sets the view to home.
*/
void MainWindow::on_rejectOrder_clicked()
{
    controller.acceptorRejectOrder(false, controller.passUsername(),selectedDriverOrderID);
    ui->centralStack->setCurrentIndex(2);
    driverOrders.clear();
    ui->OrderDetails->clear();
    ui->OrderSelection->clear();
}

void MainWindow::on_backBTN_clicked()
{
    ui->centralStack->setCurrentIndex(2);
    ui->OrderSelection->clear();
}

void MainWindow::on_backBTN_2_clicked()
{
    ui->centralStack->setCurrentIndex(2);
}


/*!
*  on_currentJobBTN_clicked is used to populate the driver accepted order combo box.
*/
void MainWindow::on_currentJobBTN_clicked()
{
    ui->assignedOrders->clear();
    ui->centralStack->setCurrentIndex(6);
    driverOrders.clear();
    ui->assignedOrders->addItem("Select an order");
    std::vector<std::vector<std::string>> driverOrderList = controller.getDriverOrders(controller.passUsername(),1);
    for(std::vector<std::string> Order : driverOrderList)
    {

        if(Order.at(1) != "")
           {
            this->driverOrders.push_back(Order);
           ui->assignedOrders->addItem("OrderID= "+QString::fromStdString(Order.at(3)) + " Cargo Owner ="+QString::fromStdString(Order.at(0)) + " order cost ="+ QString::fromStdString(Order.at(11)));
           }
    }
    if(driverOrderList.empty())
    {
          ui->assignedOrders->addItem("You have no active orders");
    }
}

/*!
*  on_assignedOrders_activated is used to show details of accepted order selected.
*/
void MainWindow::on_assignedOrders_activated(const QString &assignedOrderSelection)
{
     ui->centralStack->setCurrentIndex(6);
     std::vector<std::vector<std::string>> driverOrderList = driverOrders;
     std::stringstream assignedOrderSS(assignedOrderSelection.toStdString());
     std::vector<std::string> splitselectedOrder;
     while (assignedOrderSS.good()) {
         std::string tempstring;
         getline(assignedOrderSS,tempstring, ' ');
         splitselectedOrder.push_back(tempstring);
     }
     this -> driverAssignedOrderID = splitselectedOrder.at(1);
     for(std::vector<std::string> order : driverOrderList)
     {
         if( order.at(3) == splitselectedOrder.at(1))
         {
             setOrderInfoDisplay(order);
             ui->assignedOrderDetails->setPlainText(orderinfoOut);
         }
     }
}

/*!
*  on_loadingBTN_clicked passes order state of loading to  controller.updateOrderStatus.
*/
void MainWindow::on_loadingBTN_clicked()
{
    controller.updateOrderStatus(0,driverAssignedOrderID);
    ui->centralStack->setCurrentIndex(2);
    driverOrders.clear();
    ui->assignedOrderDetails->clear();
}


/*!
*  on_onRouteBTN_clicked passes order state of on route to  controller.updateOrderStatus.
*/
void MainWindow::on_onRouteBTN_clicked()
{
    controller.updateOrderStatus(1,driverAssignedOrderID);
    ui->centralStack->setCurrentIndex(2);
    driverOrders.clear();
    ui->assignedOrderDetails->clear();
}

/*!
*  on_deliveredBTN_clicked passes order state of delivered to  controller.updateOrderStatus.
*/
void MainWindow::on_deliveredBTN_clicked()
{
    controller.updateOrderStatus(2,driverAssignedOrderID);
    ui->centralStack->setCurrentIndex(2);
    driverOrders.clear();
    ui->assignedOrderDetails->clear();
}

/*!
*  on_pendingDeliveriesBTN_clicked populates the pending deliveries combo box with cargo owner deliveries which are not delivered yet.
*/
void MainWindow::on_pendingDeliveriesBTN_clicked()
{
    ui->centralStack->setCurrentIndex(7);

    std::vector<std::vector<std::string>> cargoOwnerOrderList = controller.getcargoOwnerOrders(controller.passUsername());
    for(std::vector<std::string> Order : cargoOwnerOrderList)
    {
        this->cargoOwnerOrders.push_back(Order);
        ui->pendingOrders->addItem("OrderID= "+QString::fromStdString(Order.at(3)) + " Cargo Owner ="+QString::fromStdString(Order.at(0)) + " order cost ="+ QString::fromStdString(Order.at(11)));
    }
    if(cargoOwnerOrderList.empty())
    {
          ui->pendingOrders->addItem("You have no active orders");
    }
}

/*!
*  on_pendingOrders_activated displays the details for the order selected for non-complete cargo owner deliveries.
*/
void MainWindow::on_pendingOrders_activated(const QString &cargoownerOrderSelected)
{

     std::vector<std::vector<std::string>> cargoOwnerOrderList = cargoOwnerOrders;
     std::stringstream assignedOrderSS(cargoownerOrderSelected.toStdString());
     std::vector<std::string> splitselectedOrder;
     while (assignedOrderSS.good()) {
         std::string tempstring;
         getline(assignedOrderSS,tempstring, ' ');
         splitselectedOrder.push_back(tempstring);
     }
     this -> driverAssignedOrderID = splitselectedOrder.at(1);
     for(std::vector<std::string> order : cargoOwnerOrderList)
     {
         if( order.at(3) == splitselectedOrder.at(1))
         {
             setOrderInfoDisplay(order);
              ui->pendingOrderDetails->setPlainText(orderinfoOut);
         }
     }
}

void MainWindow::on_backBTN_3_clicked()
{
 ui->centralStack->setCurrentIndex(2);
 ui->pendingOrders->clear();
 cargoOwnerOrders.clear();
}

/*!
*  on_orderHistoryBTN_clicked gets all completed orders and displays those which belong to the current user signed in (in the orderhistory combo box).
*/
void MainWindow::on_orderHistoryBTN_clicked()
{
    std::string username = controller.passUsername();
    orderHistory.clear();
    ui->centralStack->setCurrentIndex(8);
    this->orderHistory = controller.getOrderHistory();
     ui->OrderSelection->addItem("Select an Order");

    for(std::vector<std::string> order : orderHistory)
    {
            if(userType == "CargoOwner" && order.at(0) == username)
            {
                ui->orderhistory->addItem("OrderID= "+QString::fromStdString(order.at(3)) + " Cargo Owner ="+QString::fromStdString(order.at(0)) + " order cost ="+ QString::fromStdString(order.at(11)));
            }

           if(userType == "Driver" && order.at(1) == username)
           {
                ui->orderhistory->addItem("OrderID= "+QString::fromStdString(order.at(3)) + " Cargo Owner ="+QString::fromStdString(order.at(0)) + " order cost ="+ QString::fromStdString(order.at(11)));
           }

           if(userType == "TransportCompany" && order.at(2) == username)
           {
                ui->orderhistory->addItem("OrderID= "+QString::fromStdString(order.at(3)) + " Cargo Owner ="+QString::fromStdString(order.at(0)) + " order cost ="+ QString::fromStdString(order.at(11)));
           }
    }
    if(orderHistory.empty())
    {
          ui->OrderSelection->addItem("You have no order history");
    }
}

void MainWindow::on_backBTN_4_clicked()
{
    ui->centralStack->setCurrentIndex(2);
    ui->orderhistory->clear();
}

/*!
*  on_orderhistory_activated adds order history details for order selected.
*/
void MainWindow::on_orderhistory_activated(const QString &selectedOrder)
{


     std::stringstream assignedOrderSS(selectedOrder.toStdString());
     std::vector<std::string> splitselectedOrder;
     while (assignedOrderSS.good()) {
         std::string tempstring;
         getline(assignedOrderSS,tempstring, ' ');
         splitselectedOrder.push_back(tempstring);
     }
     for(std::vector<std::string> order : orderHistory)
     {
         if( order.at(3) == splitselectedOrder.at(1))
         {
             setOrderInfoDisplay(order);
              ui->orderhistoryDetails->setPlainText(orderinfoOut);
         }
     }
}



void MainWindow::on_backBTN_5_clicked()
{
     ui->centralStack->setCurrentIndex(2);
}

void MainWindow::on_accountInfo_clicked()
{
    ui->centralStack->setCurrentIndex(9);

}

void MainWindow::on_changeunBTN_clicked()
{

    std::string usernameinp  = ui->usernameUpdate->text().toStdString();
    controller.signIn(usernameinp,"");
    if(controller.passUsername() == usernameinp)
    {

        ui->usernameLabel ->setText("Username not valid");

    }
    else
    {
        controller.passUpdatedUserDetails(usernameinp,0);
         ui->usernameLabel ->setText("Username Changed!");
         ui->centralStack->setCurrentIndex(2);

    }
}

void MainWindow::on_changepwBTN_clicked()
{
    ui->passwordUpdate->text().toStdString();
}

void MainWindow::on_changeemailBTN_clicked()
{
    ui->emailUpdate->text().toStdString();
}

void MainWindow::on_changecontactnumBTN_clicked()
{
    ui->contactNumUpdate->text().toStdString();
}

void MainWindow::on_changecompPriceBTN_clicked()
{
    ui->emailUpdate->text().toStdString();
}
