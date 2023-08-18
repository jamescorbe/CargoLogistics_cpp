#include "controllerclass.h"
Controller::Controller() : db(""){ //pass "" to use default connection string in database constructor
    pUser = nullptr;
}
/*!
*signIn calls the database readUser function in order to validate the user login info. If the user is found in the database the user pointer won't be null and true is returned.
*/
bool Controller::signIn(std::string username, std::string password){ //signs the user in if the password is correct and assigns the controller's user object to the object loaded from the database with the given username

   std::shared_ptr<User> loadUser = db.readUser(username,password);
    if (loadUser != nullptr) {
        if (loadUser -> getPassword() == password) {
            pUser = std::move(loadUser);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
std::vector<std::vector<std::string>>Controller::getTCOrderList()
{
    return db.readTCOrders(pUser ->getUsername());
}
std::vector<std::vector<std::string>>Controller::getOrderHistory()
{
    return db.readOrderHistory();
}
std::string Controller::passUsername()
{
    return pUser->getUsername();
}
std::vector<std::string>Controller::getTransportcomp()
{
    std::vector<std::string> transportCompDetails;
   transportCompDetails = db.getTransportcompanys();
    return transportCompDetails;
}

/*!
* signOut signs out the user as so that another can login or create an account. It does this by resetting the user pointer.
*/
bool Controller::signOut(){ //resets the user object to the blank initial state after writing any changes to the user into the database
    //db.writeUser(user); //not sure if this is necessary if changes are always written to database as they are made
    pUser = nullptr; //resets the user pointer.
    return(true);
}

/*!
* This returns the user type of the user object to the main window where it is used.
*/
std::string Controller::getUserType(){
    std::string type = "NoType";

    if (dynamic_cast<Driver*>(pUser.get())){ //if the user type is a type, set the type string to be that type
        type = "Driver";
    } else if (dynamic_cast<CargoOwner*>(pUser.get())){
        type = "CargoOwner";
    } else if (dynamic_cast<TransportComp*>(pUser.get())){
        type = "TransportCompany";
    }

    return type;
}

/*!
* This takes all the order information from the MainWindow and validates if its in the correct format,
* if it is the calculate shipping cost function is called in the order class and an order is instantiated.
*/
int Controller::validateOrderDimension(std::string orderDimensionsandWeight[], bool frozen, bool fragile, float transportcompRate)///< returns true
{
 using boost::lexical_cast;
 using boost::bad_lexical_cast;
 std::regex onlyFloat("([0-9]*\.[0-9]+|[0-9]+)");
 float FloatorderDimensionsandWeight[4]; ///< regex needs to compare strings so the values are converted to floats to be used after.
 std::string width = orderDimensionsandWeight[0];
 char ch = '.';
  bool result = find(width.begin(),width.end(), ch ) != width.end();
  if(!result)
  {
      orderDimensionsandWeight[0] += ".0";
  }

 for (int i =0; i < 4 ; i++){
     try {
         FloatorderDimensionsandWeight[i] = lexical_cast<float>(orderDimensionsandWeight[i]);

     } catch (bad_lexical_cast &e) {
         return 0;
     }

    }
 if(transportcompRate == 0)
 {
     return 3;
 }
 if(FloatorderDimensionsandWeight[3] > 28000)
 {
     return 2;
 }
if(FloatorderDimensionsandWeight[0] < 2.5 &&  FloatorderDimensionsandWeight[1] < 13.3 &&  FloatorderDimensionsandWeight[2] < 2.6 && FloatorderDimensionsandWeight[3] < 28000)
    {
    if(FloatorderDimensionsandWeight[0] >0.1 &&  FloatorderDimensionsandWeight[1] > 0.1 &&  FloatorderDimensionsandWeight[2] > 0.1 && FloatorderDimensionsandWeight[3] > 0.1)
    {
    return order.calcShippingCost(FloatorderDimensionsandWeight, frozen,fragile,transportcompRate);
    }
    else
    {
        return 1;
    }
    }

else{
       return 0;
    }
}

/*!
* Passes order details to the database to be stored.
*/
void Controller::passOrderDetails(std::string transportCompSelected)
{
 db.writeOrder(pUser,order,transportCompSelected);
}

/*!
* ValidateAccount calls the database readUser function to see if the user is in the database, if they are found the user will be created and tempUser won't be null.
*/
bool Controller::validateAccount(std::string username, std::string password){

   std::shared_ptr<User> tempUser = db.readUser(username,password);
    if (tempUser != nullptr)
    {
    if (tempUser ->getUsername() == username){ //if a user has been found
        if (tempUser->getPassword() == password){
            return true; //username and password are both correct
        } else {
            return false; //user found but password incorrect
        }
    }
    } else {
        return false; //no user found, return false (failed to validate)
    }
}

/*!
* ValidateUsername uses readUser like validateAccount however returns true if only the username is found to match.
*/
bool Controller::validateUsername(std::string username)
{
    std::shared_ptr<User> tempUser = db.readUser(username,"");

         if (tempUser != nullptr)
         {
         if (tempUser ->getUsername() == username)//if a user has been found
         {
             return true;
         }
         }
         else
         {
             return false;
         }
}

/*!
* Validate password ensures the user enters a password within the bounds set.
*/
bool Controller::validatePassword(std::string password){
    if(password.length() > 6 && password.length()< 25)
    {
    return true;
    }
    else{
        return false;
    }
}

/*!
* ValidateEmail checks a given email against a regex pattern and returns the result.
*/
bool Controller::validateEmail(std::string email){
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
   return regex_match(email,pattern);
}

/*!
* validateCnum checks to see if the given contact number is the correct length and returns true if it is.
*/
bool Controller::validateCnum(std::string contactNum){
    if(contactNum.length()  == 11)
    {
    return true;
    }
    else{
        return false;
    }
}

/*!
* updateOrderStatus takes a call location and order id to allow the same database function to be called with different results.
* The function called updates the state of the order and can only be called by the driver if they have accepted the order.
*/
void Controller::updateOrderStatus(int callLocation,std::string orderID)
{
    if(callLocation ==0)
    db.updateOrderStatus("Loading",orderID);
    if(callLocation ==1)
    db.updateOrderStatus("On route",orderID);
    if(callLocation ==2)
    db.updateOrderStatus("Delivered",orderID);
}


void Controller::passUpdatedUserDetails(std::string updateString,int callLocation)
{
    std::string updateID;
db.updateUserDetails(pUser->getUsername() ,updateString,updateID);
}

/*!
* selectDriver receives an order vector from the user and converts the required details into floats with lexical cast, then it gets all drivers in the database and compares them to current values for best weight and volume match.
* Once the best match is found, it is passed to setDriverChosen where the selected driver is set for the orderid given.
*/
void Controller::selectDriver( std::vector<std::string> SelectedTCorder)
{

    bool correctVolume;
    bool correctWeight;
    int driverIndex;
    std::string bestDriverName;
    float closestDriverVolume = 100;
    float closestDriverWeight =90000;

   using boost::lexical_cast;
   float selectedOrderWeight;
   float selectedOrderVolume;

   float selectedDriverWeight =0;
   float selectedDriverVolume =0;

  std::vector<std::vector<std::string>> DriverInfoList = db.readDrivers();
  try {
      selectedOrderVolume = lexical_cast<float>(SelectedTCorder.at(5)) *lexical_cast<float>(SelectedTCorder.at(6)) * lexical_cast<float>(SelectedTCorder.at(7));
      selectedOrderWeight = lexical_cast<float>(SelectedTCorder.at(8));



  } catch (boost::bad_lexical_cast &e) {
      qDebug() <<"Error in assigning driver: lexical cast failed. " <<e.what();
  }

  for(std::vector<std::string> Driver : DriverInfoList)
  {


      try {
          selectedDriverWeight =  lexical_cast<float>(Driver.at(1)) * 1000;
          selectedDriverVolume =  lexical_cast<float>(Driver.at(3));
          driverIndex = lexical_cast<float>(Driver.at(4));
           correctVolume = (selectedDriverVolume - selectedOrderVolume < closestDriverVolume - selectedOrderVolume && selectedDriverVolume >= selectedOrderVolume);
           correctWeight= (selectedDriverWeight - selectedOrderWeight < closestDriverWeight - selectedOrderWeight && selectedDriverWeight >= selectedOrderWeight);
      } catch (boost::bad_lexical_cast &e) {
       qDebug() <<"Error in assigning driver: lexical cast failed. " <<e.what();
      }



      if(SelectedTCorder.at(10) !="t")
      {
      if(correctVolume && correctWeight)
      {
          qDebug("valid driver");
          closestDriverVolume = selectedDriverVolume;
          closestDriverWeight = selectedDriverWeight;
          bestDriverName =Driver.at(0);
      }
      }else
      {
          if(driverIndex == 2 ||driverIndex == 5 || driverIndex == 8)
          {
              closestDriverVolume = selectedDriverVolume;
              closestDriverWeight = selectedDriverWeight;
              bestDriverName =Driver.at(0);
          }
      }
  }
  setDriverChosen(bestDriverName , SelectedTCorder.at(3));
}
/*!
* All the Get order functions return a vector of string vectors with order information for a given username.
*/

std::vector<std::vector<std::string>>Controller::getDriverOrders(std::string driverName, int callLocation)
{
    return db.getDriverOrders(driverName,callLocation);
}

std::vector<std::vector<std::string>>Controller::getcargoOwnerOrders(std::string cargoOwnerName)
{
    return db.readCOOrders(cargoOwnerName);
}

/*!
* validateAddress takes the users address information validates it with regex and returns true of false.
*/
bool Controller::validateAddress(std::string numberandstreet, std::string townorcity, std::string county, std::string postcode){
    std::regex onlyString("^[a-zA-Z]*$");
    std::regex onlyNumber("^[0-9]*$");
    std::string tempword;
   const std::regex validpostcode("^([A-Za-z][A-Ha-hJ-Yj-y]?[0-9][A-Za-z0-9]? ?[0-9][A-Za-z]{2}|[Gg][Ii][Rr] ?0[Aa]{2})$");
    std::string splitnumberandstreet [2];

        std::stringstream ss(numberandstreet);
            while (ss >> tempword)
            {
               if (splitnumberandstreet[0] == "")
                {
                  splitnumberandstreet[0] =tempword;
                }
                else{
                    splitnumberandstreet[1] =tempword;
                }
            }
            QString QSsplitnumberandstreet = QString::fromStdString(splitnumberandstreet [0]);

            if (regex_match(splitnumberandstreet[0],onlyNumber) && regex_match(townorcity,onlyString) && regex_match(county,onlyString)&&regex_match(postcode,validpostcode))
            {

                return true;
            }

            else
            {
                return false;
            }
}

/*!
* validatePostCode is used to validate the postcode separately from the address to be used in the order.
*/
bool Controller::validatePostCode(std::string postCode)
{
    const std::regex validpostcode("^([A-Za-z][A-Ha-hJ-Yj-y]?[0-9][A-Za-z0-9]? ?[0-9][A-Za-z]{2}|[Gg][Ii][Rr] ?0[Aa]{2})$");

     if (regex_match(postCode,validpostcode))
     {
         return true;
     }
     else
     {
         return false;
     }
}

/*!
* acceptorRejectOrder passes the drivers choice after receiving and order and stores it in the database, it uses accept order to validate which option needs to be called.
*/
void Controller::acceptorRejectOrder(bool acceptOrder,std::string driverName,std::string orderID)
{
if (acceptOrder)
{
    db.acceptOrder(driverName,orderID);
}
else{
     db.rejectOrder(orderID);
}
}

/*!
* Passing driver chosen to the database.
*/
void Controller::setDriverChosen(std::string driverName,std::string OrderID)
{
    db.addDriverSelectiontoOrder(driverName,OrderID);
}

/*!
* Setting order source and destination location.
*/
void Controller::passOrderAddresses(std::string sourceLocation[], std::string destinationLocation[])
{
    order.setSourceandDestination(sourceLocation, destinationLocation);
}

/*!
* validatelorryIndex takes all of the checkbox states from the UI, ensures only one is chosen and returns which has been chosen.
*/
int Controller::validatelorryIndex(std::vector<bool>lorryIndexVec)
{
int boxeschecked = 0;
for(int i =0; i < lorryIndexVec.size(); i++)
{
    if(lorryIndexVec.at(i) == 1)
    {
       boxeschecked++;
    }
}
if(boxeschecked == 1)
{
    for(int i =0; i < lorryIndexVec.size(); i++)
    {

        if(lorryIndexVec.at(i) == 1)
        {
            return i + 2;
        }
    }

}
else if (boxeschecked > 1)
{
    return 18;
}
else
{
    return 20;
}
}
bool Controller::ValidateDriverdetails(std::string CPCnum,std::string drivLicID, std::string NInum,std::string regNum,bool lorryIndex)
{

}

bool Controller::validateCPCnum(std::string CPCnum){

    return true;
}

bool Controller::validateDrivLicID(std::string drivLicID){

    return true;
}

bool Controller::validateNInum(std::string NInum){

    return true;
}

bool Controller::validateRegNum(std::string regNum){

    return true;
}

/*!
* createUser is used to create the User object and pass the user details to be saved on the database.
*/
bool Controller::createUser(std::string userType, std::string username, std::string password, std::string email, std::string contactNumber, std::vector<std::string> driverStrings,std::vector<std::string> address,int lorryIndex){
    if (userType == "Driver"){
        if (driverStrings.size() == 4){

            pUser.reset(new Driver(username, password, email, contactNumber, driverStrings[0], driverStrings[1], driverStrings[2], driverStrings[3],lorryIndex));
            pUser->setAddress(address.at(0), address.at(1),address.at(2), address.at(3));
            std::async(&Database::writeUser, &db, pUser);
            return true;

        } else {
            return false;
        }
    } else if (userType == "CargoOwner"){
        pUser.reset(new CargoOwner(username, password, email, contactNumber));
        std::async(&Database::writeUser, &db, pUser);
        pUser->setAddress(address.at(0), address.at(1),address.at(2), address.at(3));
        return true;

    } else if (userType == "TransportCompany"){
        pUser.reset(new TransportComp(username, password, email, contactNumber));
        pUser->setAddress(address.at(0), address.at(1),address.at(2), address.at(3));
        std::async(&Database::writeUser, &db, pUser);
        return true;

    } else{
        return false;
    }
}


