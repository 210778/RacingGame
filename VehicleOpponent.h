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
    unsigned int boostTimeCount_;   //�u�[�X�g���Ă��鎞�� (�P�ʁF�t���[��)
    unsigned int boostTimeMax_;     //�u�[�X�g����K�v�����鎞�� (�P�ʁF�t���[��)

    float randomTurnOdds_;    //�����_���Ƀn���h����؂�m��
    float boostOdds_;         //�u�[�X�g����m��
    float boostTurnOdds_;     //�Ȃ����Ă���Ƃ��Ƀu�[�X�g����m��
    float booststraightOdds_; //�Ȃ����Ă��Ȃ��Ƃ��Ɂ@�V
    float straightLimit_;   //�n���h�������̒l�ȉ��Ȃ璼���Ƃ݂Ȃ�
    float turnLimit_;       //���̒l�ȏ�Ȃ�Ȃ����Ă�

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