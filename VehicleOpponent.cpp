#include "VehicleOpponent.h"
#include "VehicleInput.h"

//�R���X�g���N�^
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
{
}

//�R���X�g���N�^2
VehicleOpponent::VehicleOpponent(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehicleOpponent")
    , frontHitLength_(8.0f)
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
    //�����x����
    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //�ő�A�ŏ��l
    float Max = VehicleInput::GetMaxValue();
    float Min = VehicleInput::GetMinValue();


    //�O����
    if (rayCastHit_[RayCastHit::front].dist < Size.frontToRear_ * frontHitLength_
        && !rayCastHit_[RayCastHit::front].road)
    {
        //���
        operation_.ValueMap[Operation::Value::moveFrontRear] = Min;

        //�S�[���̕����ɃJ�[�u
        if (anleToCheck > 180)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = Max;
            //operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
            //operation_.inputNow[operation_.inputName::turnRight] = 0.0f;
            //operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = Min;
            //operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
            //operation_.inputNow[operation_.inputName::turnLeft] = 0.0f;
            //operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
    }
    //�����Ȃ��Ȃ�
    else
    {
        //�O�i
        operation_.ValueMap[Operation::Value::moveFrontRear] = Max;


        if (anleToCheck > 180)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = Min;
            //operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
            //operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = Max;
            //operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
            //operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }

        //�����_����
        if (rand() % 5 == 0)
        {
            //operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }
        if (rand() % 5 == 0)
        {
            //operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
        //�����_����
        if (rand() % 20 == 0)
        {
            operation_.ButtonMap[Operation::Button::boost] = true;
            //operation_.inputNow[operation_.inputName::boost] = 1.0f;
        }

    }

}

#if 0
//�E���ǂȂ獶��
if (rayCastHit_[RayCastHit::frontRight].dist < Size.toFrontRight_ * 5 ||
    rayCastHit_[RayCastHit::right].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearRight].dist < Size.toRearRight_ * 5)
{
    operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
    operation_.inputNow[operation_.inputName::turnRight] = 0.0f;
}

//�����ǂȂ�E��
if (rayCastHit_[RayCastHit::frontLeft].dist < Size.toFrontLeft_ * 5 ||
    rayCastHit_[RayCastHit::left].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearLeft].dist < Size.toRearLeft_ * 5)
{
    operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
    operation_.inputNow[operation_.inputName::turnLeft] = 0.0f;
}
#endif