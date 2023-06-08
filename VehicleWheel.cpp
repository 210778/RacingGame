#include "Engine/Model.h"

#include "VehicleWheel.h"
#include "Vehicle.h"
#include "Engine/Particle.h"

//コンストラクタ
VehicleWheel::VehicleWheel(GameObject* parent)
    :GameObject(parent, "VehicleWheel"), hModel_(-1)
    , pVehicle_(nullptr)
    , rotateSpeedX_(0.0f), handleRotateY_(0.0f)
{
}

//デストラクタ
VehicleWheel::~VehicleWheel()
{
}

//初期化
void VehicleWheel::Initialize()
{
}

//更新
void VehicleWheel::Update()
{
    transform_.rotate_.x += rotateSpeedX_;
}

//描画
void VehicleWheel::Draw()
{
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);

    //４つのタイヤをそれぞれの位置、回転で表示する
    Transform wheelTrans;
    for (int i = 0; i < 4; i++)
    {
        wheelTrans = transform_;

        switch (i)
        {
        default:
            wheelTrans.position_.x += Wheels.posFL_.x;
            wheelTrans.position_.y += Wheels.posFL_.y;
            wheelTrans.position_.z += Wheels.posFL_.z;
            wheelTrans.rotate_.x += rotateSpeedX_;
            wheelTrans.rotate_.y += handleRotateY_;
            break;
        case 1:
            wheelTrans.position_.x += Wheels.posFR_.x;
            wheelTrans.position_.y += Wheels.posFR_.y;
            wheelTrans.position_.z += Wheels.posFR_.z;
            wheelTrans.rotate_.x += rotateSpeedX_;
            wheelTrans.rotate_.y += handleRotateY_;
            break;
        case 2:
            wheelTrans.position_.x += Wheels.posRL_.x;
            wheelTrans.position_.y += Wheels.posRL_.y;
            wheelTrans.position_.z += Wheels.posRL_.z;
            wheelTrans.rotate_.x += rotateSpeedX_;
            break;
        case 3:
            wheelTrans.position_.x += Wheels.posRR_.x;
            wheelTrans.position_.y += Wheels.posRR_.y;
            wheelTrans.position_.z += Wheels.posRR_.z;
            wheelTrans.rotate_.x += rotateSpeedX_;
            break;
        }

        Model::SetTransform(hModel_, wheelTrans);
        Model::Draw(hModel_);
    }

}

//開放
void VehicleWheel::Release()
{
}

//タイヤの情報いろいろのセッター
void VehicleWheel::SetVehicleWheel(Vehicle* pVehicle, int hModel
    , XMFLOAT3 PosFL, XMFLOAT3 PosFR, XMFLOAT3 PosRL, XMFLOAT3 PosRR)
{
    pVehicle_ = pVehicle;
    hModel_ = hModel;

    Wheels.posFL_ = PosFL;
    Wheels.posFR_ = PosFR;
    Wheels.posRL_ = PosRL;
    Wheels.posRR_ = PosRR;

    //タイヤの高さを計算して車両に教える
    calculateHeight();
}

void VehicleWheel::calculateHeight()
{
    XMFLOAT3 wheelBottom = Model::GetBonePosition(hModel_, "bottom");
    Wheels.wheelHeight_ = *XMVector3Length(XMLoadFloat3(&wheelBottom)).m128_f32;
    //一応拡大に対応
    pVehicle_->SetWheelHeight(Wheels.wheelHeight_ * transform_.scale_.y);
}