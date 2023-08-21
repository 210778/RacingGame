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
    //�`�F�b�N�|�C���g
    XMFLOAT3 checkPos = *GetNextCheckPosition();
    //�`�F�b�N�|�C���g�܂ł̃x�N�g��
    XMVECTOR posToCheckVec = XMLoadFloat3(&checkPos) - XMLoadFloat3(&transform_.position_);
    //Z�x�N�g���Ƃ̊p�x
    float anleToCheck = Calculator::AngleBetweenVector(posToCheckVec, worldVector_.z);
    //�����̊p�x������
    anleToCheck = Calculator::AngleNormalize(anleToCheck - transform_.rotate_.y);


    //�O����
    if (rayCastHit_[RayCastHit::front].dist < Size.frontToRear_ * 5)
    {
        //���
        Operation.inputNow[Operation.inputName::moveRear] = 1.0f;

        //�S�[���̕����ɃJ�[�u
        if (anleToCheck > 180)
        {
            Operation.inputNow[Operation.inputName::turnLeft] = 1.0f;
            Operation.inputNow[Operation.inputName::turnRight] = 0.0f;

            Operation.inputNow[Operation.inputName::handleLeft] = 0.0f;
            Operation.inputNow[Operation.inputName::handleRight] = 1.0f;
        }
        else
        {
            Operation.inputNow[Operation.inputName::turnRight] = 1.0f;
            Operation.inputNow[Operation.inputName::turnLeft] = 0.0f;

            Operation.inputNow[Operation.inputName::handleRight] = 0.0f;
            Operation.inputNow[Operation.inputName::handleLeft] = 1.0f;
        }
    }
    //�����Ȃ��Ȃ�
    else
    {
        //�O�i
        Operation.inputNow[Operation.inputName::moveFront] = 1.0f;

        if (anleToCheck > 180)
        {
            Operation.inputNow[Operation.inputName::turnLeft] = 1.0f;
            //Operation.inputNow[Operation.inputName::handleLeft] = 1.0f;
        }
        else
        {
            Operation.inputNow[Operation.inputName::turnRight] = 1.0f;
            //Operation.inputNow[Operation.inputName::handleRight] = 1.0f;
        }

        //�����_����
        if (rand() % 5 == 0)
        {
            Operation.inputNow[Operation.inputName::handleRight] = 1.0f;
        }
        if (rand() % 5 == 0)
        {
            Operation.inputNow[Operation.inputName::handleLeft] = 1.0f;
        }
        //�����_����
        if (rand() % 20 == 0)
        {
            //Operation.inputNow[Operation.inputName::boost] = 1.0f;
        }

    }

}

#if 0
//�E���ǂȂ獶��
if (rayCastHit_[RayCastHit::frontRight].dist < Size.toFrontRight_ * 5 ||
    rayCastHit_[RayCastHit::right].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearRight].dist < Size.toRearRight_ * 5)
{
    Operation.inputNow[Operation.inputName::turnLeft] = 1.0f;
    Operation.inputNow[Operation.inputName::turnRight] = 0.0f;
}

//�����ǂȂ�E��
if (rayCastHit_[RayCastHit::frontLeft].dist < Size.toFrontLeft_ * 5 ||
    rayCastHit_[RayCastHit::left].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearLeft].dist < Size.toRearLeft_ * 5)
{
    Operation.inputNow[Operation.inputName::turnRight] = 1.0f;
    Operation.inputNow[Operation.inputName::turnLeft] = 0.0f;
}
#endif