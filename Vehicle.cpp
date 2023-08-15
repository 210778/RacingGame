#include <directxcollision.h>
#include <directxmath.h>
#include <directxpackedvector.h>

#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
#include "Engine/Particle.h"
#include "Engine/Audio.h"

#include "Vehicle.h"
#include "Ground.h"
#include "Bullet.h"
#include "Viewer.h"
#include "Sample.h"
#include "Tachometer.h"
#include "Tracker.h"
#include "Speedometer.h"
#include "CheckPoint.h"
#include "VehicleWheel.h"
#include "ParticlePackage.h"
#include "MeasurePole.h"

using std::vector;
using std::string;

//コンストラクタ
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle")
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(0.98f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f)
    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), population_(1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
    , startPosition_({ 0.0f,0.0f,0.0f }), startRotate_({ 0.0f,0.0f,0.0f })
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_()
    , handleRight_(1), handleLeft_(-1)
{
    Size.wheelHeight_ = 0.1f;
}

//継承用
Vehicle::Vehicle(GameObject* parent, const std::string& name)
    :GameObject(parent, name)
    //モデル番号
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    //ベクトル
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    //スピードなど
    , moveSPD_(0.01f), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)

    //ハンドル関係
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    //摩擦
    , wheelFriction_(0.99f), airFriction_(0.999f), turfFriction_(0.98f)
    , iceFriction_(0.999f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f), sideIceFriction_(0.02f)
    , landingFriction_(1.0f), sideFriction_(1.0f)

    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(60.0f)

    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), population_(1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
    , startPosition_({ 0.0f,0.0f,0.0f }), startRotate_({ 0.0f,0.0f,0.0f })
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_()
    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.95f)
    , handleRight_(1), handleLeft_(-1)
    //ブースト
    , boostCapacityMax_(2000.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.5f)
{
    matRotateX = XMMatrixIdentity();
    matRotateY = XMMatrixIdentity();
    matRotateZ = XMMatrixIdentity();
    matRotateX_R = XMMatrixIdentity();
    matRotateY_R = XMMatrixIdentity();
    matRotateZ_R = XMMatrixIdentity();

    worldVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });
    vehicleVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });

    //地面のタイプ
    //通常道路
    GroundTypeFriction_[Ground::road].acceleration = 1.0f;
    GroundTypeFriction_[Ground::road].landing = 0.99f;
    GroundTypeFriction_[Ground::road].side = 0.2f;
    //草地
    GroundTypeFriction_[Ground::turf].acceleration = 0.98f;
    GroundTypeFriction_[Ground::turf].landing = 0.98f;
    GroundTypeFriction_[Ground::turf].side = 0.2f;
    //砂地　草と同じ
    GroundTypeFriction_[Ground::turf].acceleration = GroundTypeFriction_[Ground::turf].acceleration;
    GroundTypeFriction_[Ground::turf].landing = GroundTypeFriction_[Ground::turf].landing;
    GroundTypeFriction_[Ground::turf].side = GroundTypeFriction_[Ground::turf].side;
    //氷床
    GroundTypeFriction_[Ground::ice].acceleration = 0.5f;
    GroundTypeFriction_[Ground::ice].landing = 0.999f;
    GroundTypeFriction_[Ground::ice].side = 0.02f;
    //加速床
    GroundTypeFriction_[Ground::boost].acceleration = 1.0f;
    GroundTypeFriction_[Ground::boost].landing = 1.05f;
    GroundTypeFriction_[Ground::boost].side = 0.2f;
    //奈落
    GroundTypeFriction_[Ground::abyss]; //作るだけ
}

//デストラクタ
Vehicle::~Vehicle()
{
}


//初期化
void Vehicle::Initialize()
{
    hModel_ = Model::Load(vehicleModelName_);
    assert(hModel_ >= 0);

    PlayerCamera_Initialize();

    //サイズ計算
    SetVehicleSize(hModel_);

    //タイヤ
    hWheelModel_ = Model::Load(wheelModelName_);
    assert(hWheelModel_ >= 0);
    MakeWheels(hWheelModel_);

    //当たり判定
    SphereCollider* collision = new SphereCollider(Model::GetBonePosition(hModel_, "center"), Size.frontToRear_ * 0.5f);
    AddCollider(collision);

    //エフェクトのポインタとそれのまとめ
    pParticle_ = Instantiate<Particle>(this);

    //ステージオブジェクトを探す
    //ちゃんとパーツが入っているか (これ以降はポインタがあってパーツがあることを前提にする)
    pGround_ = (Ground*)FindObject("Ground");
    assert(pGround_ != nullptr);
    assert(!(pGround_->GetCircuitUnion() == nullptr));
    assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    //UI初期化
    PlayerUI_Initialize();
}

//更新
void Vehicle::Update()
{
    BoundingBox abc;
    abc.Center = { 1.f,1.f,1.f };
    abc.Extents = { 1.f,1.f,1.f };

    BoundingBox def;
    abc.Center = { 1.f,1.f,1.f };
    abc.Extents = { 1.f,1.f,1.f };

    int ans;
    ans = abc.Contains(def);
    ans = def.Contains(abc);
    //BoundingBox::Contains(constBoundingOrientedBox&)

    BoundingOrientedBox bob1;
    bob1.Center = transform_.position_;
    bob1.Extents = { Size.toLeft_ * 2.f, Size.toTop_ * 2.f, Size.toFront_ * 2.f };
    bob1.Orientation;
    XMVECTOR qua = XMQuaternionRotationRollPitchYaw(transform_.rotate_.x
                                                  , transform_.rotate_.y
                                                  , transform_.rotate_.z);
    XMStoreFloat4(&bob1.Orientation, qua);

    BoundingOrientedBox bob2 = bob1;

    ans = bob1.Contains(bob2);
    ans = bob2.Contains(bob1);


    Debug::TimerLogStart("vehicle最初");

    //行列を用意
    //それぞれの値に合わせて軸回転させる行列
    matRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    matRotateZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));
    //逆回転行列
    matRotateX_R = XMMatrixInverse(nullptr, matRotateX);
    matRotateY_R = XMMatrixInverse(nullptr, matRotateY);
    matRotateZ_R = XMMatrixInverse(nullptr, matRotateZ);

    //坂道に対応
    //順番はZXY
    vehicleVector_.x = XMVector3TransformCoord(worldVector_.x, matRotateZ);
    vehicleVector_.x = XMVector3TransformCoord(vehicleVector_.x, matRotateY);

    vehicleVector_.y = XMVector3TransformCoord(worldVector_.y, matRotateZ);
    vehicleVector_.y = XMVector3TransformCoord(vehicleVector_.y, matRotateX);

    vehicleVector_.z = XMVector3TransformCoord(worldVector_.z, matRotateX);
    vehicleVector_.z = XMVector3TransformCoord(vehicleVector_.z, matRotateY);

    XMVECTOR vecX = moveSPD_ * vehicleVector_.x;
    XMVECTOR vecY = moveSPD_ * vehicleVector_.y;
    XMVECTOR vecZ = moveSPD_ * vehicleVector_.z;

    //その都度加速度を回転させるんじゃなくて最後に回転したほうがいいかも


    Debug::TimerLogEnd("vehicle最初");

    //クールタイム
    if (coolTime_ > 0)
        coolTime_--;

    //時間
    if (!(goalFlag_))
        time_++;

    //Ｇゴール  
    if (lapCount_ >= lapMax_)
        goalFlag_ = true;

    //前向きに進んでるか後ろ向きか判定
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
    {
        accZDirection_ = -1;
    }

    Debug::TimerLogStart("vehicle操作受けつけ");

    //リセット
    Operation.Refresh();
    //操作入力
    InputOperate();

    //ハンドルの操作
    handleFlag_ = false;
    if (Operation.inputNow[Operation.inputName::handleLeft])
    {
        HandleTurnLR(handleLeft_);
    }
    if (Operation.inputNow[Operation.inputName::handleRight])
    {
        HandleTurnLR(handleRight_);
    }

    //ハンドル角度制限
    //曲がりやすい
    if (slideFlag_)
        AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
    else
        AngleLimit(handleRotate_, handleRotateMax_);

    if (landingFlag_)
    {
        //全身後退
        if (Operation.inputNow[Operation.inputName::moveFront])
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (Operation.inputNow[Operation.inputName::moveRear])
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //ブースト
    if (Operation.inputNow[Operation.inputName::boost])
    {
        //容量があるなら
        if (boostCapacity_ >= boostSpending_)
        {
            //消費
            boostCapacity_ -= boostSpending_;

            slideFlag_ = true;

            acceleration_ += vecZ * 2.0f;

            XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "rear");
            ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
        }
    }
    else
    {
        slideFlag_ = false;

        //ちょっと位置が悪い
        boostCapacity_ += boostIncrease_;
        //あふれる
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }


#ifdef _DEBUG
    //左右
    if (Operation.inputNow[Operation.inputName::turnLeft])
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (Operation.inputNow[Operation.inputName::turnRight])
    {
        transform_.rotate_.y += rotateSPD_;
    }

    //ジャンプ
    //if (Operation.inputNow[Operation.inputName::jump])
    if(Operation.IsDown(Operation.inputName::jump))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (Operation.inputNow[Operation.inputName::moveLeft])
    {
        acceleration_ -= vecX;
    }
    if (Operation.inputNow[Operation.inputName::moveRight])
    {
        acceleration_ += vecX;
    }
#endif

#if 0
    //ハンドルの操作
    handleFlag_ = false;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
    {
        HandleTurnLR(handleLeft_);
    }
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
    {
        HandleTurnLR(handleRight_);
    }

    //ハンドル角度制限
    //曲がりやすい
    if (slideFlag_)
        AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
    else
        AngleLimit(handleRotate_, handleRotateMax_);

    if (landingFlag_)
    {
        //全身後退
        if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //ブースト
    if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
    {
        //容量があるなら
        if(boostCapacity_ >= boostSpending_)
        {
            //消費
            boostCapacity_ -= boostSpending_;

            slideFlag_ = true;

            acceleration_ += vecZ * 2.0f;

            XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "rear");
            ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
        }
    }
    else
    {
        slideFlag_ = false;

        //ちょっと位置が悪い
        boostCapacity_ += boostIncrease_;
        //あふれる
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }


#ifdef _DEBUG
    //左右
    if (Input::IsKey(DIK_Z))
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (Input::IsKey(DIK_X))
    {
        transform_.rotate_.y += rotateSPD_;
    }

    if (Input::IsKey(DIK_E))
    {
    }
    if (Input::IsKey(DIK_Q))
    {
    }

    //上昇
    if (Input::IsKeyDown(DIK_M))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (Input::IsKey(DIK_V))
    {
        acceleration_ -= vecX;
    }
    if (Input::IsKey(DIK_B))
    {
        acceleration_ += vecX;
    }
#endif

#endif
    Debug::TimerLogEnd("vehicle操作受けつけ");

    //地面の種類によって
    if (landingFlag_)
    {
        acceleration_ *= { GroundTypeFriction_[landingType_].landing, 1.0f
                         , GroundTypeFriction_[landingType_].landing, 1.0f };

        if (landingType_ == Ground::abyss)
        {
            //奈落に落下
            acceleration_ *= {0.0f, 0.0f, 0.0f, 0.0f};
            transform_.position_ = startTransform_.position_;
            transform_.rotate_   = startTransform_.rotate_;
        }
    }

    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //地上ならハンドルに合わせて回転
    if (landingFlag_)
    {
        //前後に動いているなら
        transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_ * accZDirection_;
        handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
    }
    //ハンドルを戻す
    if (!handleFlag_)
        handleRotate_ *= handleAdjust_;

    //タイヤの角度と減速
    //正面やタイヤの方向へは減速しないが、タイヤの方向と平行のほうこうへは減速する
    //タイヤの方向と平行なら何もしないが、垂直に近いほどタイヤの方向にベクトルを発生させる
    //メモ：これのせいで重い可能性があるから変えるべきかもしれない
    Debug::TimerLogStart("vehicleタイヤ横押し");
    if (landingFlag_)
    {
        TurnWheel();
    }
    Debug::TimerLogEnd("vehicleタイヤ横押し");


    //長さの調整
    //いらなくね
    //SpeedLimit(acceleration_, speedLimit_);

    //位置　＋　ベクトル
    XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));

    Debug::TimerLogStart("vehicle壁衝突");
        //接地、壁衝突
        VehicleCollide();
    Debug::TimerLogEnd("vehicle壁衝突");

    //タイヤの値セット
    pWheels_->SetWheelSpeedRotate(*XMVector3LengthEst(acceleration_).m128_f32
        * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //カメラ更新
    PlayerCamera_Update();

    //スピードメーター
    PlayerUI_Update();

    //エフェクト
    PlayerParticle();


    //サウンドデータのロード
    hSound_ = Audio::Load("music\\loop100201.wav");
    assert(hSound_ >= 0);
    int answ = 65535;
    if (Input::IsKeyDown(DIK_J))
    {
        Audio::Play(hSound_);
        answ = Audio::GetAudioState(hSound_);
    }
    if (Input::IsKeyDown(DIK_K))
    {
        Audio::Stop(hSound_);
        answ = Audio::GetAudioState(hSound_);
    }
    if (Input::IsKeyDown(DIK_L))
    {
        Audio::Pause(hSound_);
        answ = Audio::GetAudioState(hSound_);
    }

    answ = 99999;
}

//何かに当たった
void Vehicle::OnCollision(GameObject* pTarget)
{
    Debug::TimerLogStart("vehicleチェックポイント");

    //当たったときの処理
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() == pointCount_)
        {
            pointCount_++;
        }
        if (pCP->GetNumber() <= 0 && pointCount_ >= pointCountMax_)
        {
            pointCount_ = 0;
            lapCount_++;
        }
    }

    Debug::TimerLogEnd("vehicleチェックポイント");
}

//描画
void Vehicle::Draw()
{
    Debug::TimerLogStart("vehicle描画");

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    PlayerUI_Draw();

    Debug::TimerLogEnd("vehicle描画");
}

//開放
void Vehicle::Release()
{
}
/*
XMFLOAT3 → XMVECTOR

XMFLOAT3 f;	//何か入ってるとして
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 ← XMVECTOR

XMVECTOR v; 	//何か入ってるとして
XMFLOAT3 f;
XMStoreFloat3(&f, v);

std::string str;
        str = std::to_string(transform_.position_.x);
        str += "\n" + std::to_string(transform_.position_.y);
        str += "\n" + std::to_string(transform_.position_.z);
        MessageBox(NULL, (LPCWSTR)str.c_str(), L" puzzleList_.size() ", MB_OK);
*/
void Vehicle::Accelerator()
{
#if 0
    transform_.position_.x += acceleration_.x;
    transform_.position_.y += acceleration_.y;
    transform_.position_.z += acceleration_.z;
#endif
}

void Vehicle::SpeedLimit(XMVECTOR& speed, const float limit)
{
    //長さを調べる
    float* pLength = XMVector3LengthEst(speed).m128_f32;
    if (*pLength > limit)
    {
        *pLength = limit;
    }
    speed = XMVector3Normalize(speed);
    speed *= *pLength;
}

void Vehicle::SpeedLimit(XMFLOAT3& speed, const float limit)
{
    XMVECTOR speedVec = XMLoadFloat3(&speed);
    SpeedLimit(speedVec, limit);
    XMStoreFloat3(&speed, speedVec);
}

void Vehicle::AngleLimit(float& angle, const float limit)
{
    //マイナスか調べる
    bool minusFlag = false;
    if (angle < 0)
    {
        minusFlag = true;
    }
    //角度の絶対値が最大角度の絶対値を超えていたら最大角度にする
    //軽量化できるか
    if (fabs(angle) > fabs(limit))
    {
        angle = fabs(limit);

        //マイナスだったらマイナスに戻す
        if (minusFlag == true)
        {
            angle *= -1;
        }
    }
    //XMVector3ClampLength
}

//タイヤ回転
void Vehicle::TurnWheel()
{
    //加速度を正規化
    //左だとy軸が上向き+、右だと下向き-
    //回転して加速度に足す

    acceleration_ +=  *XMVector3LengthEst(acceleration_).m128_f32

                      * XMVector3TransformCoord(XMVector3Cross(vehicleVector_.z
                      , XMVector3Normalize(acceleration_))
                      , XMMatrixRotationNormal(vehicleVector_.z
                          , XMConvertToRadians(transform_.rotate_.z) + XM_PIDIV2))

                      * GroundTypeFriction_[landingType_].side;

#if 0
    //読みやすいコード

    //加速度を正規化
    XMVECTOR normalAcc = XMVector3Normalize(acceleration_);

    //左だとy軸が上向き+、右だと下向き-
    XMVECTOR outerProduct = XMVector3Cross(vehicleVector_.z, normalAcc);

    //回転して利用する
    XMMATRIX MatRotateZ = XMMatrixRotationNormal(vehicleVector_.z, XM_PIDIV2);
    outerProduct = XMVector3TransformCoord(outerProduct, MatRotateZ);

    acceleration_ += (*XMVector3LengthEst(acceleration_).m128_f32)
        * outerProduct * GroundTypeFriction_[landingType_].side;
#endif
}

//地面、壁、敵との衝突をまとめる？？
void Vehicle::VehicleCollide()
{
    bool isLanding = false;
    
    //壁も一回だけ当たる？
    bool isHitWall = false;

    //種類の分だけ
    for (int i = 0; i < pGround_->GetCircuitUnion()->parts_.size(); i++)
    {
        Debug::TimerLogStart("vehicle地面当たり判定");
            //地面
            if (!isLanding)
            { 
                isLanding = Landing(pGround_->GetCircuitUnion()->parts_[i].model_
                    , pGround_->GetCircuitUnion()->parts_[i].type_);
            }
        Debug::TimerLogEnd("vehicle地面当たり判定");


        Debug::TimerLogStart("vehicle壁当たり判定");
            //壁
            CollideWall(pGround_->GetCircuitUnion()->parts_[i].model_
                , pGround_->GetCircuitUnion()->parts_[i].type_);
        Debug::TimerLogEnd("vehicle壁当たり判定");
    }
}

//接地 
bool Vehicle::Landing(int hModel,int type)
{
    //レイの発射位置
    RayCastData data;
    data.start = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    data.start = Transform::Float3Add(data.start, upF);

    data.dir = { 0.0f, -1.0f, 0.0f };   //レイの方向
    Model::RayCast(hModel, &data);      //レイを発射
    
    bool isHit = false; //何かに当たったか??

    //レイが当たったら
    if (data.hit)
    {
        landingType_ = type;    //地面のタイプ
        isHit = true;           //何かに当たった

        if (-data.dist > XMVectorGetY(acceleration_) - Size.toWheelBottom_)
        {
            //下方向の加速度が大きいなら　地面にワープ　落下速度を０
            if (!landingFlag_)
            {
                transform_.position_.y -= data.dist - Size.toWheelBottom_;
                acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
                landingFlag_ = true;
            }
        }
        else
        {
            //落下 ここにあるとうまくいく
            if (!landingFlag_)
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};

            landingFlag_ = false;
        }

        //角度を変える
        if (landingFlag_ || data.dist < Size.toWheelBottom_ + Size.wheelHeight_)
        {
#if 0
            //回転
            XMVECTOR normalVec = XMVector3TransformCoord(data.normal, matRotateY_R);
            //X軸の角度を取得
            transform_.rotate_.x = XMConvertToDegrees(*XMVector3AngleBetweenNormals(
                worldVector_.z, normalVec).m128_f32) - 90.0f;
            //Z軸
            transform_.rotate_.z = -(XMConvertToDegrees(*XMVector3AngleBetweenNormals(
                worldVector_.x, normalVec).m128_f32) - 90.0f);
#endif
            //回転
            if (VehicleRotateSlope(data.normal, slopeLimitAngle_) == false)
            {
                //坂道なら落下
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
            }
        }
    }

    //接地位置調整
    //あと坂道を移動してるときに坂道に張り付く　急すぎると効果なし
    RayCastData vehicleData;
    vehicleData.start = data.start;
    XMStoreFloat3(&vehicleData.dir, -vehicleVector_.y);
    Model::RayCast(hModel, &vehicleData);      //レイを発射
    //高低差がタイヤの直径ぐらいの時
    if (vehicleData.hit && vehicleData.dist < Size.toWheelBottom_ + Size.wheelRemainder_)
    {
        landingType_ = type;    //地面のタイプ
        isHit = true;           //何かに当たった

        XMFLOAT3 wheelFlo;
        XMStoreFloat3(&wheelFlo, -vehicleVector_.y * (vehicleData.dist - Size.toWheelBottom_));
        transform_.position_ = Transform::Float3Add(transform_.position_, wheelFlo);
        landingFlag_ = true;
    }

    //天井 ほんとは分割するべきだろうけど天井に当たることは珍しいし重そうだからここに置く
    XMStoreFloat3(&data.dir, vehicleVector_.y);
    Model::RayCast(hModel, &data);  //レイを発射
    if (data.hit)
    {   
        //ちょっと地面に埋まったとき
        if (data.dist < Size.toTop_)
        {
            //その分位置を上げる
            XMFLOAT3 upVec;
            XMStoreFloat3(&upVec, data.normal * -(data.dist + Size.toWheelBottom_));
            transform_.position_ = Transform::Float3Add(transform_.position_, upVec);

            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
        //天井にぶつかったとき
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            //止める
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
    }

    return isHit;
}

bool Vehicle::CollideWall(int hModel, int type)
{
    //レイキャストの時つかう
    enum Direction
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    };

    //前後左右と斜めで分割することにする
    std::array<RayCastData, 4>wallCollideVertical;

    //レイの発射位置(ボーン取得方式だとうまくいかなかった)
    XMFLOAT3 startPos = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    startPos = Transform::Float3Add(startPos, upF);

    //前後左右編
    for (int i = 0; i < wallCollideVertical.size(); i++)
    {
        wallCollideVertical[i].start = startPos;

        //90度ずつ回転行列
        XMMATRIX matRot = XMMatrixRotationY(XM_PIDIV2 * i);	//Ｙ軸で回転させる行列   
        //回転
        XMVECTOR dirVec = XMVector3TransformCoord(worldVector_.z, matRot);
        VectorRotateMatrixZXY(dirVec);

        XMStoreFloat3(&wallCollideVertical[i].dir, dirVec);

        Model::RayCast(hModel, &wallCollideVertical[i]);  //レイを発射
        //当たったら
        if (wallCollideVertical[i].hit)
        { 
            //方向ごとの加速度
            float dirAcc;
            //方向ごとの車体の長さ
            float dirSize;
            //方向ごとのプラスマイナス： 1 と -1
            float dirPlusMinus;

            //追加
            XMVECTOR accRotVec = acceleration_;
            VectorRotateMatrixZXY(accRotVec);


            //方向ごとに代入　なんか頭わるいことしてそうな気がする
            switch (i)
            {
                //前方、その他
            default:
            case Direction::front:
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toFront_;
                dirPlusMinus = 1.0f;
                break;
                //右
            case Direction::right:
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toRight_;
                dirPlusMinus = 1.0f;
                break;
                //後ろ
            case Direction::rear:
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toRear_;
                dirPlusMinus = -1.0f;
                break;
                //左
            case Direction::left:
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toLeft_;
                dirPlusMinus = -1.0f;
                break;
            }

#if 0
            //方向ごとに代入　なんか頭わるいことしてそうな気がする
            switch (i)
            {
                //前方、その他
            default:
            case Direction::front:
                dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY));
                dirSize = Size.toFront_;
                dirPlusMinus = 1.0f;
                break;
                //右
            case Direction::right:
                dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY));
                dirSize = Size.toRight_;
                dirPlusMinus = 1.0f;
                break;
                //後ろ
            case Direction::rear:
                dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY));
                dirSize = Size.toRear_;
                dirPlusMinus = -1.0f;
                break;
                //左
            case Direction::left:
                dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY));
                dirSize = Size.toLeft_;
                dirPlusMinus = -1.0f;
                break;
            }
#endif

            //if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
            if (wallCollideVertical[i].dist < abs(dirAcc) + dirSize)
            {
                //偽 == 壁に衝突 / 真 == 坂道
                //回転
                if (VehicleRotateSlope(wallCollideVertical[i].normal, slopeLimitAngle_) == false)
                {
                    //衝突する直前で止まった時の壁までの距離
                    XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - dirSize, 0.0f };
                    ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                    //回転
                    VectorRotateMatrixZXY(ajustVec);
                    
                    //移動
                    transform_.position_.x += XMVectorGetX(ajustVec);
                    transform_.position_.y += XMVectorGetY(ajustVec);
                    transform_.position_.z += XMVectorGetZ(ajustVec);

                    //
                    acceleration_ -= XMLoadFloat3(&wallCollideVertical[i].dir) * abs(dirAcc);

                    //減速させてみる
                    acceleration_ *= wallReflectionForce_;
                    
#if 0
                    //
                    switch (i)
                    {
                        //前方、その他
                    default:
                    case Direction::front:
                    case Direction::rear:
                        accRotVec *= {1.0f, 1.0f, 0.0f, 1.0f};
                        break;
                        //右
                    case Direction::right:
                    case Direction::left:
                        accRotVec *= {0.0f, 1.0f, 1.0f, 1.0f};
                        break;
                    }

                    accRotVec += wallCollideVertical[i].normal * dirAcc;
                    acceleration_ = accRotVec;
                    VectorRotateMatrixZXY_R(acceleration_);
#endif



                    //float angle = Calculator::AngleBetweenNormalVector(
                    //    XMLoadFloat3(&wallCollideVertical[i].dir), wallCollideVertical[i].normal);


                    //float angle = Calculator::AngleBetweenNormalVector(worldVector_.z, wallCollideVertical[i].reflection);
                    //XMVECTOR rotateVec = XMVector3TransformCoord(acceleration_, XMMatrixRotationY(XMConvertToRadians(angle)));
                    //acceleration_ = rotateVec;

                    //壁反射ベクトル 
                    //acceleration_ -= XMLoadFloat3(&wallCollideVertical[i].dir)
                    //    * *XMVector3Length(acceleration_).m128_f32;
                    //acceleration_ = wallCollideVertical[i].reflection * moveSPD_ * 10.0f;

                    //acceleration_ *= {0.0f, 1.0f, 0.0f, 0.0f};
                    //壁反射ベクトル 
                    //acceleration_ -= XMLoadFloat3(&wallCollideVertical[i].dir)
                    //    * *XMVector3Length(acceleration_).m128_f32;
                    //減速させてみる
                    //acceleration_ *= wallReflectionForce_;
                    //acceleration_ *= 0;
                    //acceleration_ -= XMVector3Normalize(XMLoadFloat3(&wallCollideVertical[i].dir))
                    //    * *XMVector3Length(acceleration_).m128_f32;
                }
            }
        }
    }

    //斜め
#if 0
    std::array<RayCastData, 4>wallCollideOblique;
    for (int i = 0; i < wallCollideOblique.size(); i++)
    {
        wallCollideOblique[i].start = startPos;

        enum Oblique
        {
            frontLeft = 0,
            frontRight,
            rearLeft,
            rearRight
        };

        //z軸からの角度 とサイズ
        float theta = 0.0f;
        float vehicleLen = 0.0f;
        float dirAccZ = 0.0f;
        float dirAccX = 0.0f;

        switch (i){
        default:
        case frontLeft:
            theta = Size.angleFrontLeft_;
            vehicleLen = Size.toFrontLeft_;
            break;
        case frontRight:
            theta = Size.angleFrontRight_;
            vehicleLen = Size.toFrontRight_;
            break;
        case rearLeft:
            theta = Size.angleRearLeft_;
            vehicleLen = Size.toRearLeft_;
            break;
        case rearRight:
            theta = Size.angleRearRight_;
            vehicleLen = Size.toRearRight_;
            break;
        }

        //回転行列
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(theta));	//Ｙ軸で回転させる行列   
        //回転
        XMVECTOR dirVec = XMVector3TransformCoord(worldVector_.z, matRot);
        VectorRotateMatrixZXY(dirVec);

        XMStoreFloat3(&wallCollideOblique[i].dir, dirVec);

        if (coolTime_ <= 0 && i == 3)
        {
            Bullet* pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideOblique[i].start);
            pBullet->SetSpeed(wallCollideOblique[0].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideOblique[i].start);
            pBullet->SetSpeed(wallCollideOblique[1].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideOblique[i].start);
            pBullet->SetSpeed(wallCollideOblique[2].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideOblique[i].start);
            pBullet->SetSpeed(wallCollideOblique[3].dir);

            coolTime_ = heatAdd_;
        }

        Model::RayCast(hModel, &wallCollideOblique[i]);  //レイを発射
        //当たったら
        //重そうなので加速度の判定はここではやらない
        if (wallCollideOblique[i].hit
            && wallCollideOblique[i].dist < vehicleLen)
        {
            if (VehicleRotateSlope(wallCollideVertical[i].normal, slopeLimitAngle_) == false)
            {
                //衝突する時の壁までの距離
                XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideOblique[i].dist - vehicleLen, 0.0f };
                ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                //回転
                VectorRotateMatrixZXY(ajustVec);
                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.y += XMVectorGetY(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);

                //減速させる
                acceleration_ *= wallReflectionForce_;

                //壁反射ベクトル

                //反射する時の加速度を求める
                XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(theta));	//Ｙ軸で逆回転させる行列   
                //回転
                XMVECTOR dirVec = XMVector3TransformCoord(acceleration_, matRot);
                VectorRotateMatrixZXY(dirVec);

                float powX = XMVectorGetX(acceleration_);
                powX *= powX;

                float powZ = XMVectorGetZ(acceleration_);
                powZ *= powZ;
                //sqrt(powX + powZ;
                acceleration_ -= XMLoadFloat3(&wallCollideOblique[i].dir) * powZ;
            }
        }
    } 
#endif



    //縦横斜め統合
#if 0
    {
        enum direction
        {
            front = 0,
            right,
            rear,
            left,
            frontLeft,
            frontRight,
            rearLeft,
            rearRight
        };

        float vehicleSize;
        float directionAcc;
        float theta;

        switch (i)
        {
        default:
        case front:

            break;
        case right:

            break;
        case rear:
            
            break;
        case left:
            
            break;
        case frontLeft:
            
            break;
        case frontRight:
            
            break;
        case rearLeft:
            
            break;
        case rearRight:
            
            break;
        }
    }
#endif

#if 0
void Vehicle::CollideWall(int hModel, int type)
{
    //斜め***
    //z軸からの角度
    float theta = acos(vehicleSizeHalf_.z / vehicleSizeOblique_);

    XMVECTOR rotVec = vehicleVector_.z;

    //配列にする
    vector<RayCastData> rayCar(4);
    for (int i = 0; i < rayCar.size(); i++)
    {
        rayCar[i].start = transform_.position_;
        rayCar[i].start.y += vehicleSizeHalf_.y;

        //90度ずつ回転
        XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(90 * i));	//Ｙ軸で回転させる行列    
        rotVec = XMVector3TransformCoord(vehicleVector_.z, mat);	//ベクトルを行列で変形

        XMStoreFloat3(&rayCar[i].dir, rotVec);

        Model::RayCast(hModel, &rayCar[i]);  //レイを発射
        //レイが当たったら
        if (rayCar[i].hit)
        {
            //調整           
            //フロートにしとく
            XMFLOAT3 floAcc;
            //ベクトルY軸で回転用行列
            XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
            XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));

            XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));

            //正面行き、車両を正しい位置にワープさせ、加速度を正しく０にする
            //engineRotate_ = rayCar[0].dist - vehicleSizeHalf_.z;
            if (i == 0 && rayCar[i].dist < floAcc.z + vehicleSizeHalf_.z)
            {
                //衝突する直前で止まった時の壁までの距離
                XMVECTOR ajustVec = { 0,0,rayCar[i].dist - vehicleSizeHalf_.z,0 };
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);
                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);

                //平行移動させる
                //y軸に対応してないのでは??
                float accY = XMVectorGetY(acceleration_);
                acceleration_ = rayCar[i].parallelism * *XMVector3Length(acceleration_).m128_f32;
                acceleration_ = XMVectorSetY(acceleration_, accY);
            }

            //壁に衝突
            //移動速度+基礎値
            //
            float hitSpeed = *XMVector3LengthEst(acceleration_).m128_f32 + moveSPD_ * 2;
            if ((i == 0 || i == 2) && rayCar[i].dist < vehicleSizeHalf_.z)
            {
                //acceleration_ = rayCar[i].reflection * hitSpeed * -0.35f;
            }
            //横からあたると低反発
            else if ((i == 1 || i == 3) && (rayCar[i].dist < vehicleSizeHalf_.x))
            {
                //acceleration_ += rayCar[i].normal * hitSpeed * -0.25f / sideWheelFriction_;
            }
        }
    }

    //斜め
    //配列にする
    vector<RayCastData> rayCarOblique(4);
    for (int i = 0; i < rayCarOblique.size(); i++)
    {
        rayCarOblique[i].start = transform_.position_;
        rayCarOblique[i].start.y += vehicleSizeHalf_.y;
        //90度(?) ずつ回転
        float angle = theta;//π角度
        switch (i)
        {
        default:break;
        case 1:
            angle = XM_PI - theta; break;
        case 2:
            angle = XM_PI + theta; break;
        case 3:
            angle = -theta; break;
        }
        XMMATRIX mat = XMMatrixRotationY(angle);	//Ｙ軸で回転させる行列    
        rotVec = XMVector3TransformCoord(vehicleVector_.z, mat);	//ベクトルを行列で変形
        XMStoreFloat3(&rayCarOblique[i].dir, rotVec);

        Model::RayCast(hModel, &rayCarOblique[i]);  //レイを発射
        float hitSpeed = *XMVector3LengthEst(acceleration_).m128_f32 + moveSPD_ * 4;
        if (rayCarOblique[i].hit && rayCarOblique[i].dist < vehicleSizeOblique_)
        {
            //pMarker->SetPosition(rayCarOblique[i].end);
            //acceleration_ += rayCarOblique[i].normal * hitSpeed * -0.25;
        }
    }
}
#endif
#if 0
//前後左右編
for (int i = 0; i < wallCollideVertical.size(); i++)
{
    XMFLOAT3 pos = transform_.position_;
    XMVECTOR up = vehicleVector_.y * Size.toBottom_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, up);
    pos.x += upF.x;
    pos.y += upF.y;
    pos.z += upF.z;

    //wallCollideVertical[i].start = transform_.position_;
    wallCollideVertical[i].start = pos;
    //wallCollideVertical[i].start = Model::GetBonePosition(hModel_, "center");

    //90度ずつ回転
    XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(90.0f * i));	//Ｙ軸で回転させる行列    
    XMStoreFloat3(&wallCollideVertical[i].dir, XMVector3TransformCoord(vehicleVector_.z, matRot));//ベクトルを行列で変形

    Model::RayCast(hModel, &wallCollideVertical[i]);  //レイを発射
    //当たったら
    if (wallCollideVertical[i].hit)
    {
        //方向ごとの加速度
        float dirAcc;
        //方向ごとの車体の長さ
        float dirSize;
        //方向ごとのプラスマイナス： 1 と -1
        float dirPlusMinus;
        //方向ごとに代入　なんか頭わるいことしてそうな気がする
        switch (i)
        {
            //前方、その他
        default:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = Size.toFront_;
            dirPlusMinus = 1.0f;
            break;
            //右
        case Direction::right:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = Size.toRight_;
            dirPlusMinus = 1.0f;
            break;
            //後ろ
        case Direction::rear:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = -Size.toRear_;
            dirPlusMinus = -1.0f;
            break;
            //左
        case Direction::left:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = -Size.toLeft_;
            dirPlusMinus = -1.0f;
            break;
        }

        if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
        {
            //衝突する直前で止まった時の壁までの距離
            XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - (dirSize * dirPlusMinus), 0.0f };
            ajustVec = XMVector3TransformCoord(ajustVec, matRot);

            ajustVec = XMVector3TransformCoord(ajustVec, matRotateZ);
            ajustVec = XMVector3TransformCoord(ajustVec, matRotateX);
            ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);

            transform_.position_.x += XMVectorGetX(ajustVec);
            transform_.position_.z += XMVectorGetZ(ajustVec);


            //平行移動させる
            //float accY = XMVectorGetY(acceleration_);
            //acceleration_ = wallCollideVertical[i].parallelism * *XMVector3LengthEst(acceleration_).m128_f32;
            //acceleration_ = XMVectorSetY(acceleration_, accY);

            static float reflectForce = 0.1f;

            float accLen = *XMVector3LengthEst(acceleration_).m128_f32;
            acceleration_ =
                XMVector3NormalizeEst(wallCollideVertical[i].reflection + wallCollideVertical[i].parallelism)
                * accLen * reflectForce;
        }
    }
}
#endif

    return true;
}

void Vehicle::MakeWheels(int hModel)
{
    //タイヤ
    pWheels_ = Instantiate<VehicleWheel>(this);
    pWheels_->SetVehicleWheel(this, hModel, Size.wheelFL_, Size.wheelFR_, Size.wheelRL_, Size.wheelRR_);
}

void Vehicle::SetVehicleSize(int hModel)
{
    //ボーン
    XMFLOAT3 vehicleCenter;
    if (!(Model::GetBonePosition(&vehicleCenter, hModel, "center"))) {
        Debug::Log("failed get bone:center", true);
    }
    XMFLOAT3 vehicleBottom;
    if (!(Model::GetBonePosition(&vehicleBottom, hModel, "bottom"))) {
        Debug::Log("failed get bone:bottom", true);
    }
    XMFLOAT3 vehicleTop;
    if (!(Model::GetBonePosition(&vehicleTop, hModel, "top"))) {
        Debug::Log("failed get bone:top", true);
    }
    XMFLOAT3 vehicleLeft;
    if (!(Model::GetBonePosition(&vehicleLeft, hModel, "left"))) {
        Debug::Log("failed get bone:left", true);
    }
    XMFLOAT3 vehicleRight;
    if (!(Model::GetBonePosition(&vehicleRight, hModel, "right"))) {
        Debug::Log("failed get bone:right", true);
    }
    XMFLOAT3 vehicleRear;
    if (!(Model::GetBonePosition(&vehicleRear, hModel, "rear"))) {
        Debug::Log("failed get bone:rear", true);
    }
    XMFLOAT3 vehicleFront;
    if (!(Model::GetBonePosition(&vehicleFront, hModel, "front"))) {
        Debug::Log("failed get bone:front", true);
    }


    if (!(Model::GetBonePosition(&Size.wheelFR_, hModel, "wheelFR"))) {
        Debug::Log("failed get bone:wheelFR", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelFL_, hModel, "wheelFL"))) {
        Debug::Log("failed get bone:wheelFL", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelRR_, hModel, "wheelRR"))) {
        Debug::Log("failed get bone:wheelRR", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelRL_, hModel, "wheelRL"))) {
        Debug::Log("failed get bone:wheelRL", true);
    }

    //車両の大きさ計算
    Size.toRight_ = *XMVector3Length(XMLoadFloat3(&vehicleRight)    - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toLeft_    = *XMVector3Length(XMLoadFloat3(&vehicleRight)  - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toFront_   = *XMVector3Length(XMLoadFloat3(&vehicleFront)  - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toRear_    = *XMVector3Length(XMLoadFloat3(&vehicleRear)   - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toTop_     = *XMVector3Length(XMLoadFloat3(&vehicleTop)    - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toBottom_  = *XMVector3Length(XMLoadFloat3(&vehicleBottom) - XMLoadFloat3(&vehicleCenter)).m128_f32;

    Size.rightToLeft_   = Size.toRight_ + Size.toLeft_;
    Size.topToBottom_   = Size.toBottom_ + Size.toTop_;
    Size.frontToRear_   = Size.toFront_ + Size.toRear_;
    Size.toFrontRight_  = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toRight_ * Size.toRight_));
    Size.toFrontLeft_   = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toLeft_ * Size.toLeft_));
    Size.toRearRight_   = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toRight_ * Size.toRight_));
    Size.toRearLeft_    = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toLeft_ * Size.toLeft_));
    
    //角度
    Size.angleFrontLeft_    = XMConvertToDegrees(        acos(Size.toFront_ / Size.toFrontLeft_));
    Size.angleFrontRight_   = XMConvertToDegrees(XM_PI - acos(Size.toFront_ / Size.toFrontRight_));
    Size.angleRearLeft_     = XMConvertToDegrees(         acos(Size.toRear_  / Size.toFrontLeft_) + XM_PI);
    Size.angleRearRight_    = XMConvertToDegrees(XM_2PI - acos(Size.toRear_  / Size.toFrontRight_));
}

//ハンドルの操作
void Vehicle::HandleTurnLR(int LR)
{
    //handleFlag_ = true;

    //ややこしいがハンドル回転のためにこうしておく
    if((LR < 0 && handleRotate_ < 0) || (LR > 0 && handleRotate_ > 0))
        handleFlag_ = true;

    //滑ってると曲がりやすい
    if (slideFlag_)
        handleRotate_ += rotateSPD_ * slideHandleRotateAdd_ * LR;
    else
        handleRotate_ += rotateSPD_ * LR;
}

//次のチェックポイントの位置を取得
XMFLOAT3* Vehicle::GetNextCheckPosition()
{
    if(pointCount_ < pointCountMax_ && pointCount_ >= 0)
    {
        XMFLOAT3 pos = pGround_->GetCircuitUnion()->checkPoint_[pointCount_]->GetPosition();
        return &pos;
    }
    else
    {
        XMFLOAT3 pos = pGround_->GetCircuitUnion()->checkPoint_[0]->GetPosition();
        return &pos;
    }
}
//次のチェックポイントまでの距離を取得
float Vehicle::GetNextCheckDistance()
{
    return *XMVector3LengthEst(XMLoadFloat3(GetNextCheckPosition())).m128_f32
        - *XMVector3LengthEst(XMLoadFloat3(&transform_.position_)).m128_f32;
}

//ベクトルを回転行列で回転 (ZXY順)
void Vehicle::VectorRotateMatrixZXY(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateZ);
    vec = XMVector3TransformCoord(vec, matRotateX);
    vec = XMVector3TransformCoord(vec, matRotateY);
}

//ベクトルを回転行列で"逆"回転 (ZXY順)
void Vehicle::VectorRotateMatrixZXY_R(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateZ_R);
    vec = XMVector3TransformCoord(vec, matRotateX_R);
    vec = XMVector3TransformCoord(vec, matRotateY_R);
}

// 坂道に応じて車両を回転(X、Z軸)
bool Vehicle::VehicleRotateSlope(const XMVECTOR& normal, const float limitAngle)
{
    //法線のY軸を反転
       XMVECTOR normalR = normal * XMVECTOR{ 1.0f,-1.0f,1.0f,1.0f };

    //角度が指定角度より急だとやめる
    //角度計算は謎
    float e = XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32);

    if (XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32) > limitAngle)
    {
        return false;
    }

    //回転
    XMVECTOR normalVec = XMVector3TransformCoord(normalR, matRotateY_R);

    XMFLOAT3 rotate;
    rotate.x = XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.z, normalVec).m128_f32) - 90.0f;

    rotate.z = -(XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.x, normalVec).m128_f32) - 90.0f);

    //角度制限
    if (abs(rotate.x) <= limitAngle && abs(rotate.z) <= limitAngle)
    {
        transform_.rotate_.x = rotate.x;
        transform_.rotate_.z = rotate.z;
    }
    else
    {
        return false;
    }

    return true;

#if 0
    //法線のY軸を反転
    XMVECTOR normalR = normal * XMVECTOR{ 1.0f,-1.0f,1.0f,1.0f };

    float angleY = XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.z, normalR).m128_f32);

    //回転
    XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(-angleY));
    XMVECTOR normalVec = XMVector3TransformCoord(normalR, rotateY);

    float angleZ = XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32);


    //角度が指定角度より急だとやめる
    //角度計算は謎
    if (angleZ > limitAngle)
    {
        return false;
    }

    //回転
    normalVec = XMVector3TransformCoord(normalR, matRotateY_R);

    transform_.rotate_.x = XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.z, normalVec).m128_f32) - 90.0f;

    transform_.rotate_.z = -(XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.x, normalVec).m128_f32) - 90.0f);

    return true;
#endif
}

//地面、壁の車両の姿勢を統合する
void Vehicle::VehicleRotateTotal(std::vector<XMFLOAT3>* rotate)
{
    ;
}

//UIの関数群　プレイヤー限定で作用する
    //UIの初期化
    void Vehicle::PlayerUI_Initialize() {};
    //UIの表示
    void Vehicle::PlayerUI_Draw() {};
    //UIの情報更新
    void Vehicle::PlayerUI_Update() {};
    //カメラの用意
    void Vehicle::PlayerCamera_Initialize() {};
    //エフェクトを表示
    void Vehicle::PlayerParticle() {};
    //車両の操作、入力の受付
    void Vehicle::InputOperate() {};
    //カメラ更新
    void Vehicle::PlayerCamera_Update() {};

#if 0
物理演算に関するメモ

・運動の法則
加速度[a]は加えた力[F]に比例し、質量[m]に反比例する。[k]は比例定数
a = k * (F / m)     そして、
F = m * a
・慣性によって発生する見かけの力は　 - a * m

・遠心力
たぶん慣性の一部。向心力と釣り合う力
半径r[m]の円上を、質量m[kg]・速度v[m / s]で等速円運動している物体で考えると、[ω]は角速度
向心力 = mrω2 = m * (v ^ 2 / r)

・スリップ率
タイヤと路面間の摩擦特性がなんとか
スリップ率 = (車体速度 - 車輪の回転速度) / 車体速度


運動考察
・車の持つ値
・エンジン回転数：タコメーターで表示　クラッチが繋がってるなら徐々に値をタイヤに移す　W, Sキーで操作
・タイヤ回転数：接地してるなら前タイヤの方向へ回転数の分、加速度を加算する
・タイヤ角度：↑の時に角度の分だけベクトルを回転させる。慣性(遠心力)のために角度が大きいと回転しきれないことにするか
A, Dキーで操作


#endif
