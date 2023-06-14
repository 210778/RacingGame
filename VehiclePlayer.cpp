#include "VehiclePlayer.h"

#include "Engine/Text.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"

#include "Speedometer.h"
#include "Viewer.h"
#include "Ground.h"
#include "Bullet.h"
#include "Speedometer.h"
#include "ParticlePackage.h"
#include "Sample.h"

using std::string;
using std::to_string;

//コンストラクタ
VehiclePlayer::VehiclePlayer(GameObject* parent)
    :Vehicle(parent, "VehiclePlayer")
    , pSpeedometer_(nullptr), pTextSpeed_(nullptr), pTextTime_(nullptr)
    , pTextLap_(nullptr), pTextRanking_(nullptr), pTextAcceleration_(nullptr)
    , km_hAdd(120.0f)
    , pSample_(nullptr)
{
}

//コンストラクタ
VehiclePlayer::VehiclePlayer(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehiclePlayer")
    , pSpeedometer_(nullptr), pTextSpeed_(nullptr), pTextTime_(nullptr)
    , pTextLap_(nullptr), pTextRanking_(nullptr), pTextAcceleration_(nullptr)
    , km_hAdd(120.0f)
    , pSample_(nullptr)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
}

//デストラクタ
VehiclePlayer::~VehiclePlayer()
{
}

//開放
void VehiclePlayer::Release()
{
    pTextSpeed_->Release();
    pTextTime_->Release();
    pTextLap_->Release();
    pTextRanking_->Release();
    pTextAcceleration_->Release();
}

//UIの初期化
void VehiclePlayer::PlayerUI_Initialize()
{
    //スピードメーター
    pSpeedometer_ = Instantiate<Speedometer>(this);

    //文字
    pTextSpeed_ = new Text;
    pTextSpeed_->Initialize();

    pTextTime_ = new Text;
    pTextTime_->Initialize();

    pTextLap_ = new Text;
    pTextLap_->Initialize();

    pTextRanking_ = new Text;
    pTextRanking_->Initialize();

    pTextAcceleration_ = new Text;
    pTextAcceleration_->Initialize();

    pSample_ = Instantiate<Sample>(GetParent());
}

//UIの表示
void VehiclePlayer::PlayerUI_Draw()
{
    //時速表示
    //平面のベクトルの長さ
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    string speedStr = to_string((int)(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd));
    speedStr += "km/h";
    pTextSpeed_->Draw(30, 30, speedStr.c_str());

    //経過時間表示
    int min = 0;
    int sec = 0;
    int mSec = 0;
    int rest = 0;
    min = time_ / 3600;
    rest = time_ % 3600;
    sec = rest / 60;
    rest = rest % 60;
    mSec = rest;
    string timeStr = "time:";    //= to_string(min) + ":" + to_string(sec) + ":" + to_string(mSec);
    if (min < 10)
        timeStr += "0";
    timeStr += to_string(min) + ":";
    if (sec < 10)
        timeStr += "0";
    timeStr += to_string(sec) + ":";
    if (mSec < 10)
        timeStr += "0";
    timeStr += to_string(mSec);
    pTextTime_->Draw(30, 70, timeStr.c_str());

    //周回数表示
    string lapStr = "Lap:";
    lapStr += to_string(lapCount_) + "/" + to_string(lapMax_);
    pTextLap_->Draw(30, 110, lapStr.c_str());

    //順位表示
    string rank = to_string(ranking_) + "/" + to_string(population_);
    pTextLap_->Draw(30, 150, rank.c_str());

#ifdef _DEBUG
    //
    pSample_->SetPosition(*GetNextCheckPosition());
    pSample_->SetScale({ 20.0,20.0,20.0 });

    //加速度表示
    XMFLOAT3 floAcc;
    //ベクトルY軸で回転用行列
    XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
    XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));
    string accStr = "X:" + to_string(floAcc.x) + " Y:" + to_string(floAcc.y) + " Z:" + to_string(floAcc.z);
    pTextAcceleration_->Draw(280, 30, accStr.c_str());
#endif
}

//UIの情報更新
void VehiclePlayer::PlayerUI_Update()
{
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    pSpeedometer_->SetSpeed(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd);
}

//カメラの用意
void VehiclePlayer::PlayerCamera_Initialize()
{
    Viewer* pViewer = Instantiate<Viewer>(this);
    pViewer->SetPosition(transform_.position_);
}

void VehiclePlayer::PlayerParticle()
{
    //ゴールしたら
    if (goalFlag_)
    {
        ParticlePackage::ActRainbowFire(pParticle_, transform_.position_);
    }

    //走行中のタイヤの軌跡
    if (wheelParticleLength_ < *XMVector3Length(acceleration_).m128_f32)
    {
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRL"));
    }
}
#if 0

//初期化
void VehiclePlayer::Initialize()
{
    hModel_ = Model::Load("model\\Tri_Lines.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = { 3,3,3 };
}

//更新
void VehiclePlayer::Update()
{
    /*
    XMFLOAT3 → XMVECTOR

    XMFLOAT3 f;	//何か入ってるとして
    XMVECTOR v = XMLoadFloat3(&f);

    XMFLOAT3 ← XMVECTOR

    XMVECTOR v; 	//何か入ってるとして
    XMFLOAT3 f;
    XMStoreFloat3(&f, v);
    */
}

//何かに当たった
void VehiclePlayer::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
}

//描画
void VehiclePlayer::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}


#endif