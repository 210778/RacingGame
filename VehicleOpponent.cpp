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
    Operation.inputNow[Operation.inputName::moveFront] = 1.0f;
}