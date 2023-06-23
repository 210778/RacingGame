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
    XMFLOAT3 addPos;
    XMFLOAT3 wheelPos;
    float addRotateY;

    for (int i = 0; i < 4; i++)
    {
        wheelTrans = transform_;
        addRotateY = 0;

        switch (i)
        {
        default:
            XMStoreFloat3(&addPos, Wheels.centerToLeft_);
            wheelPos = Wheels.posFL_;
            addRotateY = handleRotateY_;
            break;

        case 1:
            XMStoreFloat3(&addPos, Wheels.centerToRight_);
            wheelPos = Wheels.posFR_;
            addRotateY = handleRotateY_;
            break;
        case 2:
            XMStoreFloat3(&addPos, Wheels.centerToLeft_);
            wheelPos = Wheels.posRL_;
            break;
        case 3:
            XMStoreFloat3(&addPos, Wheels.centerToRight_);
            wheelPos = Wheels.posRR_;
            break;
        }

        wheelTrans.position_.x += addPos.x + wheelPos.x;
        wheelTrans.position_.y += addPos.y + wheelPos.y;
        wheelTrans.position_.z += addPos.z + wheelPos.z;

        wheelTrans.rotate_.x += rotateSpeedX_;
        wheelTrans.rotate_.y += addRotateY;

        //今は左右対称だけど前後対象にしたいなら
        //wheelTrans.rotate_.x = wheelTrans.rotate_.x * -1.0f + rotateSpeedX_;
        //wheelTrans.rotate_.y += addRotateY + 180.0f;

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

    //それぞれのタイヤまでのベクトル
    XMFLOAT3 centerPos = Model::GetBonePosition(hModel_, "center");
    XMVECTOR centerVec = XMLoadFloat3(&centerPos);
    Wheels.cToFL_ = XMLoadFloat3(&Wheels.posFL_) - centerVec;
    Wheels.cToFR_ = XMLoadFloat3(&Wheels.posFR_) - centerVec;
    Wheels.cToRL_ = XMLoadFloat3(&Wheels.posRL_) - centerVec;
    Wheels.cToRR_ = XMLoadFloat3(&Wheels.posRR_) - centerVec;

    //ボーン間のベクトル
    XMFLOAT3 bottomPos = Model::GetBonePosition(hModel_, "bottom");
    Wheels.centerToBottom_ = XMLoadFloat3(&bottomPos) - centerVec;
    XMFLOAT3 leftPos = Model::GetBonePosition(hModel_, "left");
    Wheels.centerToLeft_ = XMLoadFloat3(&leftPos) - centerVec;
    XMFLOAT3 rightPos = Model::GetBonePosition(hModel_, "right");
    Wheels.centerToRight_ = XMLoadFloat3(&rightPos) - centerVec;

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