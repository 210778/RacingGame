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

    if (rayCastHit_[RayCastHit::front].dist < Size.frontToRear_ * 5)
    {
        if(rayCastHit_[RayCastHit::frontRight].dist < Size.frontToRear_ * 5)
        {
            Operation.inputNow[Operation.inputName::moveRear] = 1.0f;
            Operation.inputNow[Operation.inputName::turnLeft] = 1.0f;
        }
        if (rayCastHit_[RayCastHit::frontLeft].dist < Size.frontToRear_ * 5)
        {
            Operation.inputNow[Operation.inputName::moveRear] = 1.0f;
            Operation.inputNow[Operation.inputName::turnRight] = 1.0f;
        }
    }
    else
    {
        Operation.inputNow[Operation.inputName::moveFront] = 1.0f;
    }
}