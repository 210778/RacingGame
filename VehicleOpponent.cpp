#include "VehicleOpponent.h"

//コンストラクタ
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
{
}

//コンストラクタ2
VehicleOpponent::VehicleOpponent(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehicleOpponent")
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
}

//デストラクタ
VehicleOpponent::~VehicleOpponent()
{
}
