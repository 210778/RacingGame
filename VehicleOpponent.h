#pragma once
#include "Vehicle.h"

//���������Ǘ�����N���X
class VehicleOpponent : public Vehicle
{
    float frontHitLength_;  //�O�̏Փ˔��蒷��
public:
    //�R���X�g���N�^
    VehicleOpponent(GameObject* parent);

    //�R���X�g���N�^2
    VehicleOpponent(GameObject* pParent, std::string vehicleName, std::string wheelName);

    //�f�X�g���N�^
    ~VehicleOpponent();

    //����
    void InputOperate() override;
};