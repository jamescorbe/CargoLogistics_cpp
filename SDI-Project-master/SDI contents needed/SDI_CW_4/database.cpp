#include "database.h"


Database::Database(std::string connectString)
{
    connection_string ="postgresql://u0jbs0tjlhbq71dmvne0:UiRXXL18lahU7fHYa2FR@bft8nehsqvfjwboybmay-postgresql.services.clever-cloud.com:5432/bft8nehsqvfjwboybmay";

}

/*!
*Tests the database connection.
*/
void Database::startDBtest()
{
    try
    {

        pqxx::connection c(connection_string);
        if (c.is_open()) {
             qDebug() << "Opened database successfully: " << c.dbname();
        } else {
             qDebug() << "Can't open database";
        }
    }
    catch (const std::exception &e)
    {
          std::cerr << e.what() << std::endl;
    }
}

/*!
* Gets all transport company prices and names, returning a vector with all entries.
*/
std::vector<std::string> Database::getTransportcompanys()
{
    int cols;
    std::vector<std::string> transportCompDetails;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result transportCompRes = workobj.exec("Select username,transportcomp_price FROM currentusers WHERE usertype = 2;");

    if(!transportCompRes.empty())
   {
        cols = transportCompRes.columns();

        if(cols >1)
        {
            for(auto row : transportCompRes)  ///<looping throught the username results
                {
                    for(int i = 0; i < cols; i++)
                    {
                       transportCompDetails.push_back(row[i].c_str());
                    }
                }
        }
    workobj.commit();
    c.disconnect();
    return transportCompDetails;
    }
}

/*!
* writeUser Writes all the user information to the database after a user has been created, this is done by passing all the validated information to a stored procedure on the database
* while calling the 'adddriverinfo' procedure if a drivers details also need to be added. The passwords are encrypted with the postgresql encode function in the adduser statement as so they
* are stored securely.
*/
void Database::writeUser(const std::shared_ptr<User>& user)
{


    try {
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    std::string testcpc;
    std::string testDrivlicnumber;
    std::string testNI;
    int userTypereturn;


    std::string type;

     ///!Setting the local user type to be the same as in the user object.

    if (dynamic_cast<Driver*>(user.get())){
        type = "Driver";
        userTypereturn = 0;
    } else if (dynamic_cast<CargoOwner*>(user.get())){
        type = "CargoOwner";
        userTypereturn = 1;
    } else if (dynamic_cast<TransportComp*>(user.get())){
        type = "TransportCompany";
        userTypereturn = 2;
    }

    pqxx::result adduser = workobj.exec("CALL adduser('" + user->getUsername() + "'," + "encode('" +user->getPassword() + "', 'base64')"  + "," + std::to_string(userTypereturn) + ",'" + user->getEmail() + "'," + user->getContactNumber() + ",'" + user->getAddressLine1() + "','" + user->getAddressLine2() + "','" + user->getAddressLine3() + "','" + user->getAddressLine4() + "');");


    if(type == "Driver")
    {


        pqxx::result addDriverdetails = workobj.exec("CALL adddriverinfo('" + user->getUsername() +"','"+ dynamic_cast<Driver*>(user.get())->getCPCnumber()+"','"+dynamic_cast<Driver*>(user.get())->getlorry().getregNum()+"','"+dynamic_cast<Driver*>(user.get())->getDrivLicID()+"','"+dynamic_cast<Driver*>(user.get())->getNInumber()+ "','"+std::to_string(dynamic_cast<Driver*>(user.get())->getlorry().gettypeIndex())+"');"); // needs to pass username, cpcnumber, lorryregnumber,drivlicid,ninumber,lorrytype,lorrywidth,lorrylength,lorryheight
    }
    workobj.commit();
    c.disconnect();
   } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;

}

}

/*!
* writeOrder Writes an order to the database after it has been created and validated, calling a stored procedure on the database with the required values.
*/
void Database::writeOrder(const std::shared_ptr<User>& user,const Order &order,std::string transportCompSelected)
{
   pqxx::connection c(connection_string);
   pqxx::work workobj(c);
    try {

        pqxx::result createOrder = workobj.exec("CALL addorder('" + user->getUsername()+"','"+"NONE"+"','"+transportCompSelected+"','"+"Created"+"','"+ std::to_string(order.getWidth())+"','"+std::to_string(order.getLength())+"','"+std::to_string(order.getHeight())+"','"+std::to_string(order.getWeight()) +"','"+std::to_string(order.getFragile())+"','"+std::to_string(order.getFrozen())+"','"+std::to_string(order.getCost())+"','"+ order.getSourceAddressLine1()+"','"+order.getSourceAddressLine2()+"','"+order.getSourceAddressLine3()+"','"+order.getSourceAddressLine4()+"','"+order.getDestinationAddressLine1()+"','"+order.getDestinationAddressLine2()+"','"+order.getDestinationAddressLine3()+"','"+order.getDestinationAddressLine4()+"');");

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;

    }


    workobj.commit();
    c.disconnect();


}


/*!
* readUser uses the username and password given at login to search for users in the database, if found the user will be initialized to the corresponding user type.
*  If the usertype is a driver the driver details will also be found from 'lorrys' where they are stored in the database. The passwords on the database are encrypted so a QT base64 conversion
* is used to compare the un encrypted password to the user input.
*/
std::shared_ptr<User> Database::readUser(const std::string& username,const std::string& password){
    QByteArray decrypPasswordBA;
    std::string decrypPassword;

    int userType; //stored in database as int 0= driver, 1 = CargoOwner, 2 = transport comp.
   std::shared_ptr<User> user;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    int usercol =0;
    int drivercol=0;

    std::string userDetails [7];
    std::string DriverDetails [6];
    QString un = QString::fromStdString(username);
    QString pw = QString::fromStdString(password);

    pqxx::result userResults = workobj.exec("SELECT * FROM public.currentusers WHERE username = '"+username +"';");

    //std::cout<<userResults.at(1)<<endl;

    if(!userResults.empty())
   {
        usercol = userResults.columns();

        if(usercol >1)
        {
            for(auto row : userResults) // adding database responce to the userDetails array.
                {
                    for(int i = 0; i < usercol; i++)
                    {
                     if(i == 1)
                     {
                         decrypPasswordBA = QByteArray::fromBase64(row[i].c_str());
                         QString QdecrypPassword  = QString(decrypPasswordBA);
                         decrypPassword = QdecrypPassword.toStdString();
                     }
                    userDetails [i] = row[i].c_str();

                    }
                }

            if (userDetails[0] == username && decrypPassword == password)
            {
                for(int i = 0; i < usercol; i++)
                {
                     QString UD = QString::fromStdString(userDetails [i]);


                }


                QString UD = QString::fromStdString(userDetails[0]);


                if (userDetails[2] == "0") { //if driver
                    pqxx::result DriverResults = workobj.exec("SELECT * FROM lorrys WHERE username ='"+username +"';");
                    drivercol = DriverResults.columns();
                    for(auto driverRow :DriverResults)
                    {
                        for(int i = 0; i < drivercol; i++)
                        {
                        DriverDetails [i] = driverRow[i].c_str();
                        const QString QTDriverDetails = QString::fromStdString(driverRow[i].c_str());

                        }
                    }


                    user.reset((new Driver(userDetails[0], decrypPassword, userDetails[2], userDetails[3], DriverDetails[4],DriverDetails[3], DriverDetails[1],DriverDetails[2],std::stoi(DriverDetails[5])))); //replace values to be the ones loaded from the database

                } else if(userDetails[2] == "1") { // if cargo owner


                    user.reset((new CargoOwner(userDetails[0], decrypPassword, userDetails[2], userDetails[3])));

                } else if (userDetails[2] == "2") {//if transport comp


                    user.reset((new TransportComp(userDetails[0], decrypPassword, userDetails[2], userDetails[3])));
                } else {

                }
            }
            else if(userDetails[0] == username)
            {
                qDebug() << "User found however wrong password given ";
                  user.reset((new TransportComp(userDetails[0], "","","")));

            }
            else
             {
                    user = nullptr; // user not found
             }
        }
    }

    workobj.commit();
   c.disconnect();
    return user;
}

/*!
* readCOOrders reads all  cargoOwners orders and stores all which have not been marked as delivered. It returns a vector of vectors containing all of these string entries.
*/
std::vector<std::vector<std::string>> Database::readCOOrders(const std::string& username){
    std::vector<std::vector<std::string>> orderList;
    std::vector<std::string> ordervec;
    int cols;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result allOrders = workobj.exec("Select * from orders WHERE cargoowner ='" +username+"';");
    std::string orderstring;
    if(!allOrders.empty())
   {

        cols = allOrders.columns();

        if(cols >1)
        {
            for(auto Orderrow : allOrders)
                {

                    for(int i = 0; i < cols; i++)
                    {
                            ordervec.push_back(Orderrow[i].c_str());
                            if(i == 14)
                            {

                                if(ordervec.at(4) != "Delivered")
                                orderList.push_back(ordervec);
                                ordervec.clear();
                            }
                    }
                }
        }

    }
    workobj.commit();
    c.disconnect();
    return orderList;
}

/*!
* readOrderHistory retreives all the orders which have been delivered and returns a vector of string vectors, it iterates though each row and column to add the responses to a vector,
* adding the created vector to orderHistoryList once all the columns have been added.
*/
std::vector<std::vector<std::string>> Database::readOrderHistory(){
    std::vector<std::vector<std::string>> orderHistoryList;
    std::vector<std::string> orderHistoryVec;
    int cols;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result allDelivered = workobj.exec("Select * from orders WHERE orderstatus = 'Delivered';");
    if(!allDelivered.empty())
   {
        cols = allDelivered.columns();

        if(cols >1)
        {
            for(auto Orderrow : allDelivered)
                {

                    for(int i = 0; i < cols; i++)
                    {
                            orderHistoryVec.push_back(Orderrow[i].c_str());
                            if(i == 14)
                            {
                                orderHistoryList.push_back(orderHistoryVec);
                                orderHistoryVec.clear();
                            }
                    }
                }
        }

    }
    workobj.commit();
    c.disconnect();
    return orderHistoryList;
}

/*!
* readTCOrders selects all of the orders where the given transportation company have been assigned, looping through the results and returning a vector of vectors.
*/
std::vector<std::vector<std::string>> Database::readTCOrders(const std::string& username){
    std::vector<std::vector<std::string>> orderList;
    std::vector<std::string> ordervec;
    int cols;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result allOrders = workobj.exec("Select * from orders WHERE transport_comp ='" +username+"';");
    std::string orderstring;
    if(!allOrders.empty())
   {
        cols = allOrders.columns();

        if(cols >1)
        {
            for(auto Orderrow : allOrders)
                {

                    for(int i = 0; i < cols; i++)
                    {
                            ordervec.push_back(Orderrow[i].c_str());
                            if(i == 14)
                            {
                                orderList.push_back(ordervec);
                                ordervec.clear();
                            }
                    }
                }
        }

    }
    workobj.commit();
    c.disconnect();
    return orderList;
}

/*!
* addDriverSelectiontoOrder updates the given order with the driver chosen for it along with using the orderID to identify the location to change the results.
*/
void Database::addDriverSelectiontoOrder(std::string driverName,std::string OrderID)
{
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result addDriverToOrder = workobj.exec("UPDATE orders SET driverchosen ='" + driverName + "' WHERE order_id ='" +OrderID +"';"); ///< adding the selected driver to the driverChosen variable.
    workobj.commit();
    c.disconnect();
}

/*!
* getDriverOrders returns all the driver orders and filters which are returned with the call location, with call location 0 being for assigned orders and call location 2 being for
* all order states except delivered or created.
*/
 std::vector<std::vector<std::string>> Database::getDriverOrders(std::string driverName,int callLocation)
{

     int cols;
     std::vector<std::vector<std::string>> DriverOrderList;
     std::vector<std::string> DriverOrdervec;
     pqxx::connection c(connection_string);
     pqxx::work workobj(c);
     pqxx::result DriverOrders = workobj.exec("SELECT * FROM orders WHERE driverchosen ='" + driverName+"';");

     if(!DriverOrders.empty())
    {
         cols = DriverOrders.columns();
         if(cols >1)
         {
             for(auto row : DriverOrders) ///< looping through results and adding them to a vector,then adding the result to another vector.
                 {
                  for(int i =0; i < cols; i++)
                  {

                     DriverOrdervec.push_back(row[i].c_str());

                     if(i == 14)
                     {

                        if(DriverOrdervec.at(4) == "Created" &&callLocation ==0)
                            {
                                 DriverOrderList.push_back(DriverOrdervec);
                                 DriverOrdervec.clear();
                            }
                        if(DriverOrdervec.size() >0)
                            {
                                 if(callLocation ==1)
                                    {
                                        if(DriverOrdervec.at(4) != "Delivered")
                                        {
                                            DriverOrderList.push_back(DriverOrdervec);
                                            DriverOrdervec.clear();
                                        }
                                    }
                            }
                    }
             }
         }
     }
   }


     workobj.commit();
     c.disconnect();
     return DriverOrderList;

}

  /*!
  * acceptOrder is used to set order status to assigned for the order after it is accepted, while adding the drivers name in the correct column.
  */
 void Database::acceptOrder(std::string driverName,std::string orderID)
{
     pqxx::connection c(connection_string);
     pqxx::work workobj(c);
     pqxx::result updateOrderDriver = workobj.exec("UPDATE orders SET driver ='" + driverName+"'"+" WHERE order_id =" +orderID+";"); ///< getting all drivers and their lorry index.
     pqxx::result updateOrderstate = workobj.exec("UPDATE orders SET orderstatus = 'Assigned' WHERE order_id =" + orderID + ";");
     workobj.commit();
     c.disconnect();


}
 /*!
  * rejectOrder is used to remove the driverchosen as so that the order can be reassigned a new driver.
  */
 void Database::rejectOrder(std::string orderID)
 {
     pqxx::connection c(connection_string);
     pqxx::work workobj(c);
     pqxx::result updateOrderstate = workobj.exec("UPDATE orders SET driverchosen = '' WHERE order_id =" + orderID + ";");
     workobj.commit();
     c.disconnect();
 }

void Database::updateUserDetails(std::string username, std::string updateString, std::string updateID)
{
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result updateOrderstate = workobj.exec("UPDATE currentusers SET'"+updateID + "' ='"+ updateString + "' WHERE username ='" + username + ";");
    workobj.commit();
    c.disconnect();
}

/*!
 * updateOrderStatus is used to update the order with the status chosen by the driver such as delivered or loading.
 */
void Database::updateOrderStatus(std::string orderStatusChosen,std::string orderID)
{
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result updateOrderstate = workobj.exec("UPDATE orders SET orderstatus ='"+ orderStatusChosen+ "' WHERE order_id = " + orderID + ";");
    workobj.commit();
    c.disconnect();
}

/*!
 * readDrivers selects the drivers username and lorryindex from all drivers, finds the drivers lorry details with lorryindex and returns a vector of vectors with their details.
 */
std::vector<std::vector<std::string>>Database::readDrivers(){

    std::vector<std::vector<std::string>> DriverList;
    std::vector<std::string> Drivervec;

    std::vector<std::vector<std::string>> driverandLorryspecList;
    std::vector<std::string> driverandLorryspec; ///< includes the drivers username, their lorry weight, if they can carry frozen goods and their cargo volume.
    int cols;
    pqxx::connection c(connection_string);
    pqxx::work workobj(c);
    pqxx::result driverLorrys = workobj.exec("SELECT username,lorryindex FROM lorrys;"); ///< getting all drivers and their lorry index.

    if(!driverLorrys.empty())
   {
        cols = driverLorrys.columns();
        if(cols >1)
        {
            for(auto row : driverLorrys) ///< looping through results and adding them to a vector,then adding the result to another vector.
                {
                    Drivervec.push_back(row[0].c_str());
                    Drivervec.push_back(row[1].c_str());

                    DriverList.push_back(Drivervec);
                    Drivervec.clear();

                }
        }
    }

    for(std::vector<std::string> driverinfo : DriverList )
    {

    pqxx::result lorryIndexInfo = workobj.exec("SELECT max_weight,carry_frozen,cargo_volume FROM lorry_types WHERE lorryid ='" +driverinfo.at(1) +"';");
    if(!lorryIndexInfo.empty())
    {
        for(auto row : lorryIndexInfo)
            {

                driverandLorryspec.push_back(driverinfo.at(0));
                driverandLorryspec.push_back(row[0].c_str());
                driverandLorryspec.push_back(row[1].c_str());
                driverandLorryspec.push_back(row[2].c_str());
                driverandLorryspec.push_back(driverinfo.at(1));


                driverandLorryspecList.push_back(driverandLorryspec);
                driverandLorryspec.clear();

            }
    }
    }
    workobj.commit();
    c.disconnect();
    return driverandLorryspecList;

}
