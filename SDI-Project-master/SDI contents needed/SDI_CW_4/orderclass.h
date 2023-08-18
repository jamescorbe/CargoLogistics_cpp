#pragma once //only includes this file once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include <ctime>
#include <qdebug.h>
#include <chrono>
#include <boost/lexical_cast.hpp>
#include <vector>


class Lorry {
    private:
        std::string regNum;
        int typeIndex;
    public:
        Lorry(int typeIndex, std::string regNumInp);
        std::string getregNum(){return regNum;}
        int gettypeIndex(){return typeIndex;}
};



class Order {
    private:
       float DimensionsandWeight[4];
        float cost;
        bool fragile, frozen;
        std::map<std::string, std::string> sourceAddress = {
            {"num&street", ""},
            {"city", ""},
            {"county", ""},
            {"postCode", ""}
        };
        std::map<std::string, std::string> destinationAddress = {
            {"num&street", ""},
            {"city", ""},
            {"county", ""},
            {"postCode", ""}
        };
    public:
        float calcShippingCost(float orderDimensionsandWeight[], bool frozen, bool fragile,float transportcompRate);
        void setSourceandDestination(std::string SourceAddress[],std::string DestinationAddress[]);
        float getWidth() const{return DimensionsandWeight[0];}
        float getLength() const{return DimensionsandWeight[1];}
        float getHeight() const{return DimensionsandWeight[2];}
        float getWeight() const{return DimensionsandWeight[3];}
        float getCost()  const{return cost;};
        bool getFragile() const{return fragile;}
        bool getFrozen() const{return frozen;}
        const std::string& getSourceAddressLine1() const {return sourceAddress.at("num&street"); }
        const std::string& getSourceAddressLine2() const {return sourceAddress.at("city"); }
        const std::string& getSourceAddressLine3() const {return sourceAddress.at("county"); }
        const std::string& getSourceAddressLine4() const {return sourceAddress.at("postCode"); }
        const std::string& getDestinationAddressLine1() const {return destinationAddress.at("num&street"); }
        const std::string& getDestinationAddressLine2() const {return destinationAddress.at("city"); }
        const std::string& getDestinationAddressLine3() const {return destinationAddress.at("county"); }
        const std::string& getDestinationAddressLine4() const {return destinationAddress.at("postCode"); }

};
