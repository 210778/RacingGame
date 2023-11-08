#include "VehicleOpponent.h"
#include "VehicleInput.h"

//�R���X�g���N�^
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
    , frontHitLength_(5.0f), sideHitLength_(15.0)
    , checkBackAngle_(45.0f), backFlag_(false), backTimeCount_(0), backTimeMax_(2)
    , boostFlag_(false), boostTimeCount_(0), boostTimeMax_(1)
    , randomTurnOdds_(20), boostOdds_(1), boostTurnOdds_(10), booststraightOdds_(10)
    , straightLimit_(1.0f), turnLimit_(15.0f)
{
    vehicleModelName_ = "";
    wheelModelName_ = "";

    max_ = VehicleInput::GetMaxValue();
    min_ = VehicleInput::GetMinValue();

    backTimeMax_ = 2 * Global::GetStandardFPS();
    boostTimeMax_ = 2 * Global::GetStandardFPS();

    //�����Őݒ肷��
    randomTurnOdds_ = rand() % 15;
    boostOdds_ = rand() % 2;
    boostTurnOdds_ = rand() % 2;
    booststraightOdds_ = rand() % 2;
}

//�R���X�g���N�^2
VehicleOpponent::VehicleOpponent(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehicleOpponent")
    , frontHitLength_(5.0f), sideHitLength_(15.0)
    , checkBackAngle_(45.0f), backFlag_(false), backTimeCount_(0), backTimeMax_(2)
    , boostFlag_(false), boostTimeCount_(0), boostTimeMax_(1)
    , randomTurnOdds_(20), boostOdds_(1), boostTurnOdds_(10), booststraightOdds_(10)
    , straightLimit_(1.0f), turnLimit_(15.0f)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;

    max_ = VehicleInput::GetMaxValue();
    min_ = VehicleInput::GetMinValue();

    backTimeMax_ = 2 * Global::GetStandardFPS();
    boostTimeMax_ = 2 * Global::GetStandardFPS();

    //�����Őݒ肷��
    randomTurnOdds_     = rand() % 15;
    boostOdds_          = rand() % 2;
    boostTurnOdds_      = rand() % 2;
    booststraightOdds_  = rand() % 2;
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
        boostFlag_ = false;

        //��莞�ԃo�b�N�������߂�
        backTimeCount_++;
        if (backTimeCount_ >= backTimeMax_)
        {
            backFlag_ = false;
            backTimeCount_ = 0;
            operation_.ValueMap[Operation::Value::handleRightLeft] = 0.0f;
        }
        else
        {
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
    }
    else
    {
        //�S�[���̕����ɋȂ���
        if (anleToCheck > 180.0f)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //�O�i
        operation_.ValueMap[Operation::Value::moveFrontRear] = max_;

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
            || rayCastHit_[RayCastHit::right].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        //�����ǂȂ�E��
        else if (rayCastHit_[RayCastHit::frontLeft].dist < sideHitLength_
            || rayCastHit_[RayCastHit::left].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }


        //�e�ʂ������ău�[�X�g�o���Ȃ�
        if (boostCapacity_ <= boostSpending_)
        {
            boostFlag_ = false;
        }
        //�u�[�X�g����
        if (boostFlag_)
        {
            //��莞�ԃu�[�X�g�������߂�
            boostTimeCount_++;
            if (boostTimeCount_ >= boostTimeMax_)
            {
                boostFlag_ = false;
                boostTimeCount_ = 0;
                operation_.ButtonMap[Operation::Button::boost] = false;
            }
            else
            {
                operation_.ButtonMap[Operation::Button::boost] = true;
            }
        }
        else
        {
            //�u�[�X�g���ĂȂ��Ȃ�
            //���݂̊m��
            float currentBoostOdds = boostOdds_;
            //�Ȃ����Ă�Ȃ�
            if (abs(handleRotate_) > turnLimit_)
                currentBoostOdds = boostTurnOdds_;
            //�����Ȃ�
            if (abs(handleRotate_) < straightLimit_)
                currentBoostOdds = booststraightOdds_;
            //�m��
            boostFlag_ = Calculator::IsProbability(currentBoostOdds);
        }
    }

}