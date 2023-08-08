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

//操作
void VehicleOpponent::InputOperate()
{
    Operation.inputNow[Operation.inputName::moveFront] = 1.0f;
}