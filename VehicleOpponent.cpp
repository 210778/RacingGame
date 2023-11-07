#include "VehicleOpponent.h"
#include "VehicleInput.h"

//コンストラクタ
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
{
}

//コンストラクタ2
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

//デストラクタ
VehicleOpponent::~VehicleOpponent()
{
}

//操作
void VehicleOpponent::InputOperate()
{
    //チェックポイント
    XMFLOAT3 checkPos = *GetNextCheckPosition();
    //チェックポイントまでのベクトル
    XMVECTOR posToCheckVec = XMLoadFloat3(&checkPos) - XMLoadFloat3(&transform_.position_);
    //Zベクトルとの角度
    float anleToCheck = Calculator::AngleBetweenVector(posToCheckVec, worldVector_.z);
    //自分の角度を引く
    anleToCheck = Calculator::AngleNormalize(anleToCheck - transform_.rotate_.y);
    //加速度長さ
    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;



    //前が壁か目的地が真後ろ
    if ((anleToCheck > (180.0f - checkBackAngle_) && anleToCheck < (180.0f + checkBackAngle_))
        || (rayCastHit_[RayCastHit::front].dist < frontHitLength_ && !rayCastHit_[RayCastHit::front].road))
    {
        backFlag_ = true;
    }

    //バックする
    if (backFlag_)
    {
        //一定時間バックしたらやめる
        backTimeCount_++;
        if (backTimeCount_ >= backTimeMax_)
        {
            backFlag_ = false;
            backTimeCount_ = 0;
        }

        //後退
        operation_.ValueMap[Operation::Value::moveFrontRear] = min_;

        //ゴールの方向にカーブ
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

        //ゴールの方向に曲がる
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

        //ハンドルの向きが正しいなら進む
        if (frontFlag)
        {
            //前進
            operation_.ValueMap[Operation::Value::moveFrontRear] = max_;
        }

        //ランダムで左右に動く
        if (Calculator::IsProbability(randomTurnOdds_))
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        else if (Calculator::IsProbability(randomTurnOdds_))
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //右が壁なら左へ
        if (rayCastHit_[RayCastHit::frontRight].dist < sideHitLength_
            || rayCastHit_[RayCastHit::right].dist < sideHitLength_
            || rayCastHit_[RayCastHit::rearRight].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        //左が壁なら右へ
        else if (rayCastHit_[RayCastHit::frontLeft].dist < sideHitLength_
            || rayCastHit_[RayCastHit::left].dist < sideHitLength_
            || rayCastHit_[RayCastHit::rearLeft].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //ブースト切り替え
        if (Calculator::IsProbability(randomTurnOdds_))
        {
            boostFlag_ = boostFlag_ ? false : true;
        }
        //ブーストしたいのに出来てない
        if (!(slideFlag_) && boostFlag_)
        {
            boostFlag_ = false;
        }
        //ブースト
        if (boostFlag_)
        {
            operation_.ButtonMap[Operation::Button::boost] = true;
        }

    }


}