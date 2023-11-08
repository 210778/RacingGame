#include "VehicleOpponent.h"
#include "VehicleInput.h"

//コンストラクタ
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

    //乱数で設定する
    randomTurnOdds_ = rand() % 15;
    boostOdds_ = rand() % 2;
    boostTurnOdds_ = rand() % 2;
    booststraightOdds_ = rand() % 2;
}

//コンストラクタ2
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

    //乱数で設定する
    randomTurnOdds_     = rand() % 15;
    boostOdds_          = rand() % 2;
    boostTurnOdds_      = rand() % 2;
    booststraightOdds_  = rand() % 2;
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
        boostFlag_ = false;

        //一定時間バックしたらやめる
        backTimeCount_++;
        if (backTimeCount_ >= backTimeMax_)
        {
            backFlag_ = false;
            backTimeCount_ = 0;
            operation_.ValueMap[Operation::Value::handleRightLeft] = 0.0f;
        }
        else
        {
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
    }
    else
    {
        //ゴールの方向に曲がる
        if (anleToCheck > 180.0f)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        else
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }

        //前進
        operation_.ValueMap[Operation::Value::moveFrontRear] = max_;

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
            || rayCastHit_[RayCastHit::right].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = min_;
        }
        //左が壁なら右へ
        else if (rayCastHit_[RayCastHit::frontLeft].dist < sideHitLength_
            || rayCastHit_[RayCastHit::left].dist < sideHitLength_)
        {
            operation_.ValueMap[Operation::Value::handleRightLeft] = max_;
        }


        //容量が無くてブースト出来ない
        if (boostCapacity_ <= boostSpending_)
        {
            boostFlag_ = false;
        }
        //ブーストする
        if (boostFlag_)
        {
            //一定時間ブーストしたらやめる
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
            //ブーストしてないなら
            //現在の確率
            float currentBoostOdds = boostOdds_;
            //曲がってるなら
            if (abs(handleRotate_) > turnLimit_)
                currentBoostOdds = boostTurnOdds_;
            //直線なら
            if (abs(handleRotate_) < straightLimit_)
                currentBoostOdds = booststraightOdds_;
            //確率
            boostFlag_ = Calculator::IsProbability(currentBoostOdds);
        }
    }

}