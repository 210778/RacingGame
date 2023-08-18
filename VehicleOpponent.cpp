#include "VehicleOpponent.h"

//�R���X�g���N�^
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
{
}

//�R���X�g���N�^2
VehicleOpponent::VehicleOpponent(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehicleOpponent")
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
}

//�f�X�g���N�^
VehicleOpponent::~VehicleOpponent()
{
}

//����
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