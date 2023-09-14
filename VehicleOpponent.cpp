#include "VehicleOpponent.h"

//コンストラクタ
VehicleOpponent::VehicleOpponent(GameObject* parent)
    :Vehicle(parent, "VehicleOpponent")
{
}

//コンストラクタ2
VehicleOpponent::VehicleOpponent(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehicleOpponent")
    , frontHitLength_(8.0f)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
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


    //前が壁
    if (rayCastHit_[RayCastHit::front].dist < Size.frontToRear_ * frontHitLength_
        && !rayCastHit_[RayCastHit::front].road)
    {
        //後退
        operation_.inputNow[operation_.inputName::moveRear] = 1.0f;

        //ゴールの方向にカーブ
        if (anleToCheck > 180)
        {
            //operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
            //operation_.inputNow[operation_.inputName::turnRight] = 0.0f;

            operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }
        else
        {
            //operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
            //operation_.inputNow[operation_.inputName::turnLeft] = 0.0f;

            operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
    }
    //何もないなら
    else
    {
        //前進
        operation_.inputNow[operation_.inputName::moveFront] = 1.0f;

        if (anleToCheck > 180)
        {
            //operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
            operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
        else
        {
            //operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
            operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }

        //ランダム性
        if (rand() % 5 == 0)
        {
            operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }
        if (rand() % 5 == 0)
        {
            operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
        //ランダム性
        if (rand() % 20 == 0)
        {
            //operation_.inputNow[operation_.inputName::boost] = 1.0f;
        }

    }

}

#if 0
//右が壁なら左へ
if (rayCastHit_[RayCastHit::frontRight].dist < Size.toFrontRight_ * 5 ||
    rayCastHit_[RayCastHit::right].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearRight].dist < Size.toRearRight_ * 5)
{
    operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
    operation_.inputNow[operation_.inputName::turnRight] = 0.0f;
}

//左が壁なら右へ
if (rayCastHit_[RayCastHit::frontLeft].dist < Size.toFrontLeft_ * 5 ||
    rayCastHit_[RayCastHit::left].dist < Size.rightToLeft_ * 5 ||
    rayCastHit_[RayCastHit::rearLeft].dist < Size.toRearLeft_ * 5)
{
    operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
    operation_.inputNow[operation_.inputName::turnLeft] = 0.0f;
}
#endif