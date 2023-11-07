#pragma once
#include "Vehicle.h"

//���������Ǘ�����N���X
class VehicleOpponent : public Vehicle
{
    float max_;
    float min_;

    float frontHitLength_;  //�O�̏Փ˔���̒���
    float sideHitLength_;   //���̏Փ˔���̒���

    float checkBackAngle_;  //���ɖړI�n������Ɣ��f����p�x(1/2)
    bool backFlag_;        //�O���ǂȂ�o�b�N����
    unsigned int backTimeCount_;   //�o�b�N���Ă��鎞�� (�P�ʁF�t���[��)
    unsigned int backTimeMax_;     //�o�b�N����K�v�����鎞�� (�P�ʁF�t���[��)

    bool boostFlag_;

    int backOdds_;  //�O���ǂ̎��Ƀo�b�N����m��
    int randomTurnOdds_;    //�����_���Ƀn���h����؂�m��
    int wallTurnOdds_;

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