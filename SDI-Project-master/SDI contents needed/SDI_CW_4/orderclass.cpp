#include "orderclass.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include <list>
#include <chrono>
#include <ctime>

Lorry::Lorry(int typeIndex, std::string regNumInp) :  regNum(regNumInp),typeIndex(typeIndex){
}

/*!
*  calcShippingCost gets order dimensions, fragile/frozen state and transportCompRate to calculate the order cost. The volumetric weight of the order is calculated
* and whichever is heavier the weight or volumetric weight is used to calculate the order cost. Additional variables such as fragile and frozen add cost to the order and
*the transport company fee is added at the end.
*/
float Order::calcShippingCost(float orderDimensionsandWeight[], bool frozen, bool fragile,float transportCompRate){
    float orderVolumetricWeight = ((orderDimensionsandWeight[0] * orderDimensionsandWeight[1]) * orderDimensionsandWeight[2]) * 250; ///< volumetric weight
    float baseordercost;
    float distance = 300; ///< 300 miles set as a place holder distance
    const float costperKGpermile = 0.001;
    if (orderVolumetricWeight > orderDimensionsandWeight[3])
    {

        baseordercost = (costperKGpermile * orderVolumetricWeight) * distance;
    }
    else
    {
         baseordercost = (costperKGpermile * orderDimensionsandWeight[3]) * distance;
    }

    if(frozen)
    {
        baseordercost  *= 1.05; // 5% extra fee for frozen.
    }
    if (fragile)
    {
        baseordercost *= 1.07; // 7% extra fee for fragile.
    }
    ;
      this->DimensionsandWeight[0] = orderDimensionsandWeight[0]; ///< width
      this->DimensionsandWeight[1] = orderDimensionsandWeight[1]; ///< length
      this->DimensionsandWeight[2] = orderDimensionsandWeight[2]; ///< height
      this->DimensionsandWeight[3] = orderDimensionsandWeight[3]; ///< weight
      this->frozen = frozen;
      this->fragile = fragile;
      this->cost = baseordercost;


    return baseordercost * transportCompRate;

    //after this is returned it can be stored in order where it call be called and the values passed to database if the user places the order.
}

/*!
* sets order source and destination.
*/
void Order::setSourceandDestination(std::string SourceAddress[],std::string DestinationAddress[])
{
    this->sourceAddress["num&street"] = SourceAddress[0];
    this->sourceAddress["city"] = SourceAddress[1];
    this->sourceAddress["county"] = SourceAddress[2];
    this->sourceAddress["postCode"] = SourceAddress[3];

    this->destinationAddress["num&street"] = DestinationAddress[0];
    this->destinationAddress["city"] = DestinationAddress[1];
    this->destinationAddress["county"] = DestinationAddress[2];
    this->destinationAddress["postCode"] = DestinationAddress[3];
}
