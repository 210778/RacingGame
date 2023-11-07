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
    , frontHitLength_(5.0f), sideHitLength_(10.0)
    , checkBackAngle_(45.0f), backFlag_(false), backTimeCount_(0), backTimeMax_(2)
    , boostFlag_(false)
    , backOdds_(90), randomTurnOdds_(10), wallTurnOdds_(90)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;

    max_ = VehicleInput::GetMaxValue();
    min_ = VehicleInput::GetMinValue();

    backTimeMax_ *= Global::GetStandardFPS();
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



    //�O���ǂ��ړI�n���^���
    if ((anleToCheck > (180.0f - checkBackAngle_) && anleToCheck < (180.0f + checkBackAngle_))
        || (rayCastHit_[RayCastHit::front].dist < frontHitLength_ && !rayCastHit_[RayCastHit::front].road))
    {
        backFlag_ = true;
    }

    //�o�b�N����
    if (backFlag_)
    {
        //��莞�ԃo�b�N�������߂�
        backTimeCount_++;
        if (backTimeCount_ >= backTimeMax_)
        {
            backFlag_ = false;
            backTimeCount_ = 0;
        }

        //���
        operation_.ValueMap[Operation::Value::moveFrontRear] = min_;

        //�S�[���̕����ɃJ�[�u
        if (anleToCheck > 180.0f)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
    }
    else
    {
        bool frontFlag = false;

        //�S�[���̕����ɋȂ���
        if (anleToCheck > 180.0f)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
            
            if (handleRotate_ < 0.0f)
            {
                frontFlag = true;
            }
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;

            if (handleRotate_ > 0.0f)
            {
                frontFlag = true;
            }
        }

        //�n���h���̌������������Ȃ�i��
        if (frontFlag)
        {
            //�O�i
            operation_.ValueMap[Operation::Value::moveFrontRear] = max_;
        }

        //�����_���ō��E�ɓ���
        if (Calculator::IsProbability(randomTurnOdds_))
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        else if (Calculator::IsProbability(randomTurnOdds_))
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //�E���ǂȂ獶��
        if (rayCastHit_[RayCastHit::frontRight].dist < sideHitLength_
            || rayCastHit_[RayCastHit::right].dist < sideHitLength_
            || rayCastHit_[RayCastHit::rearRight].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        //�����ǂȂ�E��
        else if (rayCastHit_[RayCastHit::frontLeft].dist < sideHitLength_
            || rayCastHit_[RayCastHit::left].dist < sideHitLength_
            || rayCastHit_[RayCastHit::rearLeft].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //�u�[�X�g�؂�ւ�
        if (Calculator::IsProbability(randomTurnOdds_))
        {
            boostFlag_ = boostFlag_ ? false : true;
        }
        //�u�[�X�g�������̂ɏo���ĂȂ�
        if (!(slideFlag_) && boostFlag_)
        {
            boostFlag_ = false;
        }
        //�u�[�X�g
        if (boostFlag_)
        {
            operation_.ButtonMap[Operation::Button::boost] = true;
        }

    }


}