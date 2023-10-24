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
#include "Engine/Global.h"
#include "Engine/SceneManager.h"

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
#include "Music.h"
#include "ImagePrinter.h"
#include "PoryLine.h"
#include "Circuit.h"

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
    , ranking_(0), population_(1), goalTime_(0)
    , mass_(1.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
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
    , ranking_(0), goalRanking_(0), population_(1), goalTime_(0), standbyTime_(0)
    , mass_(1.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f), wheelParticleLengthMax_(0.5f)
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_(), restartTransform_()

    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.99f)
    , handleRight_(1), handleLeft_(-1)
    //ブースト
    , boostCapacityMax_(2000.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.5f), boostValue_(2.0f)
    , isPlayer_(false)
    , collideBoxValue_(0.5f)
    , isOperationInvalid_(false), pauseFlag_(false)
    , pViewer_(nullptr)
    , pPoryLine_(nullptr)
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
    GroundTypeFriction_[Circuit::circuitType::road].acceleration = 1.0f;
    GroundTypeFriction_[Circuit::circuitType::road].landing = 0.99f;
    GroundTypeFriction_[Circuit::circuitType::road].side = 0.2f;
    //草地
    GroundTypeFriction_[Circuit::circuitType::turf].acceleration = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].landing = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].side = 0.2f;
    //砂地　草と同じ
    GroundTypeFriction_[Circuit::circuitType::dirt].acceleration = GroundTypeFriction_[Circuit::circuitType::turf].acceleration;
    GroundTypeFriction_[Circuit::circuitType::dirt].landing = GroundTypeFriction_[Circuit::circuitType::turf].landing;
    GroundTypeFriction_[Circuit::circuitType::dirt].side = GroundTypeFriction_[Circuit::circuitType::turf].side;
    //氷床
    GroundTypeFriction_[Circuit::circuitType::ice].acceleration = 0.2f;
    GroundTypeFriction_[Circuit::circuitType::ice].landing = 0.999f;
    GroundTypeFriction_[Circuit::circuitType::ice].side = 0.02f;
    //加速床
    GroundTypeFriction_[Circuit::circuitType::boost].acceleration = 1.0f;
    GroundTypeFriction_[Circuit::circuitType::boost].landing = 1.05f;
    GroundTypeFriction_[Circuit::circuitType::boost].side = 0.1f;
    //奈落
    GroundTypeFriction_[Circuit::circuitType::abyss]; //作るだけ
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

    //サイズ計算
    SetVehicleSize(hModel_);

    //プレイヤーのカメラ
    PlayerCamera_Initialize();

    //タイヤ
    hWheelModel_ = Model::Load(wheelModelName_);
    assert(hWheelModel_ >= 0);
    MakeWheels(hWheelModel_);

    //当たり判定
    SphereCollider* collision = new SphereCollider(Model::GetBonePosition(hModel_, "center")
                                , Size.centerFrontToRear_);
    AddCollider(collision);

    //エフェクトのポインタとそれのまとめ
    pParticle_ = Instantiate<Particle>(this);

    //ステージオブジェクトを探す
    //ちゃんとパーツが入っているか (これ以降はポインタがあってパーツがあることを前提にする)
    //pGround_ = (Ground*)FindObject("Ground");
    //assert(pGround_ != nullptr);
    //assert(!(pGround_->GetCircuitUnion() == nullptr));
    //assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    //再スタート地点
    restartTransform_ = startTransform_;

    //UI初期化
    PlayerUI_Initialize();

    //ポリライン初期化
    pPoryLine_ = new PoryLine;
    pPoryLine_->Load("image\\PaticleAssets\\tex.png");
}

//更新
void Vehicle::Update()
{
    //ポーズ
    if (pauseFlag_)
        return;

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

    //ゴール  
    if (lapCount_ >= lapMax_)
    {
        //ゴール順位が更新されてないならセットする
        if (!goalFlag_)
        {
            goalRanking_ = ranking_;
            goalTime_ = time_;
        }

        goalFlag_ = true;
    }

    //前向きに進んでるか後ろ向きか判定
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
        accZDirection_ = -1;

    Debug::TimerLogStart("vehicle操作受けつけ");

    InputReceive(vecX, vecZ);

    Debug::TimerLogEnd("vehicle操作受けつけ");

    //地面の種類によって
    if (landingFlag_)
    {
        acceleration_ *= { GroundTypeFriction_[landingType_].landing, 1.0f
                         , GroundTypeFriction_[landingType_].landing, 1.0f };
        
        //復活地点更新
        if (landingType_ == Circuit::circuitType::road)
        {
            restartTransform_.position_ = transform_.position_;
            restartTransform_.rotate_ = transform_.rotate_;
        }

        if (landingType_ == Circuit::circuitType::abyss)
        {
            //奈落に落下
            acceleration_ *= {0.0f, 0.0f, 0.0f, 0.0f};
            transform_.position_ = restartTransform_.position_;
            transform_.rotate_   = restartTransform_.rotate_;
            //今すぐカメラ移動
            if (pViewer_ != nullptr)
                pViewer_->WatchPresentPosition();
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
    //正面やタイヤの方向へは減速しないが、タイヤの方向と平行の方向へは減速する
    //タイヤの方向と平行なら何もしないが、垂直に近いほどタイヤの方向にベクトルを発生させる
    //メモ：これのせいで重い可能性があるから変えるべきかもしれない
    Debug::TimerLogStart("vehicleタイヤ横押し");
    TurnWheel();
    Debug::TimerLogEnd("vehicleタイヤ横押し");

    //位置　＋　ベクトル
    XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));

    Debug::TimerLogStart("vehicle壁床衝突");
    //接地、壁衝突
    VehicleCollide();
    Debug::TimerLogEnd("vehicle壁床衝突");

    //タイヤの値セット
    pWheels_->SetWheelSpeedRotate(*XMVector3LengthEst(acceleration_).m128_f32
        * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //カメラ更新
    PlayerCamera_Update();

    //スピードメーター
    PlayerUI_Update();

    //エフェクト
    //PlayerParticle();
    VehicleParticle();

    //角度正規化
    transform_.rotate_.x = Calculator::AngleNormalize(transform_.rotate_.x);
    transform_.rotate_.y = Calculator::AngleNormalize(transform_.rotate_.y);
    transform_.rotate_.z = Calculator::AngleNormalize(transform_.rotate_.z);

    //ポリラインに現在の位置を伝える
    pPoryLine_->AddPosition(transform_.position_);
}

//何かに当たった
void Vehicle::OnCollision(GameObject* pTarget)
{
    //ポーズ
    if (pauseFlag_)
        return;

    Debug::TimerLogStart("vehicleチェックポイント");

    //当たったときの処理
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() == pointCount_)
            pointCount_++;

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

    //ポリラインを描画
    //pPoryLine_->Draw();

    Debug::TimerLogEnd("vehicle描画");
}

//開放
void Vehicle::Release()
{
    //ポリライン解放
    pPoryLine_->Release();
}

void Vehicle::SpeedLimit(XMVECTOR& speed, const float limit)
{
    //長さを調べる
    float* pLength = XMVector3LengthEst(speed).m128_f32;
    if (*pLength > limit)
        *pLength = limit;

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
        minusFlag = true;

    //角度の絶対値が最大角度の絶対値を超えていたら最大角度にする
    //軽量化できるか
    if (fabs(angle) > fabs(limit))
    {
        angle = fabs(limit);
        //マイナスだったらマイナスに戻す
        if (minusFlag == true)
            angle *= -1;
    }
}

//タイヤ回転
void Vehicle::TurnWheel()
{
    //地面にいるなら実行
    if (!landingFlag_)
        return;
    //加速度を正規化
    //左だとy軸が上向き+、右だと下向き-
    //回転して加速度に足す
    acceleration_ +=  *XMVector3LengthEst(acceleration_).m128_f32
                      * XMVector3TransformCoord(XMVector3Cross(vehicleVector_.z
                      , XMVector3Normalize(acceleration_))
                      , XMMatrixRotationNormal(vehicleVector_.z
                      , XMConvertToRadians(transform_.rotate_.z) + XM_PIDIV2))
                      * GroundTypeFriction_[landingType_].side;

    //読みやすいコード
#if 0
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

    //種類の分だけ
    for(auto& itr : Circuit::GetChosenCircuit()->parts_)
    {
        landingFlag_ = false;   //接地フラグリセット

        //地面
        Landing(itr.model_, itr.type_);

        if (landingFlag_)
            isLanding = true;
        landingFlag_ = isLanding;   //一度でもtrueなら接地してることにする

        //壁
        CollideWall(itr.model_, itr.type_);
    }

    //落下 ここにあるとうまくいく
    if (!landingFlag_)
    {
        acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
    }
}

//接地 
void Vehicle::Landing(int hModel,int type)
{
    //レイの発射位置
    RayCastData data;
    data.start = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    data.start = Transform::Float3Add(data.start, upF);

    data.dir = { 0.0f, -1.0f, 0.0f };   //レイの方向
    Model::RayCast(hModel, &data);      //レイを発射

    //レイが当たったら
    if (data.hit)
    {
        //NPC用
        SetRayCastHit(RayCastHit::Number::down, data);

        if (-data.dist > XMVectorGetY(acceleration_) - Size.toWheelBottom_)
        {
            landingType_ = type;    //地面のタイプ

            //下方向の加速度が大きいなら　地面にワープ　落下速度を０
            if (!landingFlag_)
            {
                transform_.position_.y -= data.dist - Size.toWheelBottom_;
                acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            }

            landingFlag_ = true;
        }
        else
            landingFlag_ = false;

        //角度を変える
        if (landingFlag_ || data.dist < Size.toWheelBottom_ + Size.wheelHeight_)
        {
            landingFlag_ = true;
            landingType_ = type;

            //回転
            if (VehicleRotateSlope(data.normal, slopeLimitAngle_, RayCastHit::Number::down) == false)
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};  //坂道なら落下
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

        XMFLOAT3 wheelFlo;
        XMStoreFloat3(&wheelFlo, -vehicleVector_.y * (vehicleData.dist - Size.toWheelBottom_));
        transform_.position_ = Transform::Float3Add(transform_.position_, wheelFlo);
        landingFlag_ = true;
    }
    //天井 ほんとは分割するべきだろうけど天井に当たることは珍しいし重そうだからここに置く
    XMStoreFloat3(&data.dir, vehicleVector_.y);
    Model::RayCast(hModel, &data);  //レイを発射
    //当たった
    if (data.hit)
    {   
        //NPC用
        SetRayCastHit(RayCastHit::Number::up, data);

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
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};  //止める
    }
}

void Vehicle::CollideWall(int hModel, int type)
{
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

        //レイキャストの時用
        enum Direction
        {
            front = 0,
            right = 1,
            rear = 2,
            left = 3,
        };

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
            //NPC用
            int NPC_Number = 0;

            //方向ごとの加速度
            float dirAcc;
            //方向ごとの車体の長さ
            float dirSize;

            //加速度を回転
            XMVECTOR accRotVec = acceleration_;
            VectorRotateMatrixZXY(accRotVec);

            //方向ごとに代入　なんか頭わるいことしてそうな気がする
            switch (i)
            {             
            default://前方、その他
            case Direction::front:
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toFront_;
                NPC_Number = RayCastHit::Number::front; 
                break;
            case Direction::right://右
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toRight_;
                NPC_Number = RayCastHit::Number::right; 
                break;
            case Direction::rear://後ろ
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toRear_;
                NPC_Number = RayCastHit::Number::rear; 
                break;
            case Direction::left://左
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toLeft_;
                NPC_Number = RayCastHit::Number::left;
                break;
            }

            //NPC用
            SetRayCastHit(NPC_Number, wallCollideVertical[i]);

            if (wallCollideVertical[i].dist < abs(dirAcc) + dirSize)
            {
                //偽 == 壁に衝突 / 真 == 坂道
                //回転
                if (VehicleRotateSlope(wallCollideVertical[i].normal, slopeLimitAngle_, NPC_Number) == false)
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

                    switch (i){
                    default://前方、その他
                    case Direction::front:
                        accRotVec *= {1.0f, 1.0f, 0.0f, 1.0f}; break;
                    case Direction::right://右
                        accRotVec *= {0.0f, 1.0f, 1.0f, 1.0f}; break;
                    case Direction::rear://後ろ
                        accRotVec *= {1.0f, 1.0f, 0.0f, 1.0f}; break;
                    case Direction::left://左
                        accRotVec *= {0.0f, 1.0f, 1.0f, 1.0f}; break;
                    }
                    //加速度を戻す
                    acceleration_ = accRotVec;
                    VectorRotateMatrixZXY_R(acceleration_);
                    //減速
                    acceleration_ *= wallReflectionForce_;
                    //壁反射
                    acceleration_ -= XMLoadFloat3(&wallCollideVertical[i].dir) * moveSPD_;
                }
            }
        }
    }

    //斜め
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

        //加速度回転
        XMVECTOR accRotVec = acceleration_;
        VectorRotateMatrixZXY(accRotVec);

        float dirAccX = XMVectorGetX(accRotVec),
            dirAccZ = XMVectorGetZ(accRotVec);

        int NPC_Number = 0; //NPC用

        switch (i) {
        default:
        case Oblique::frontLeft://前左
            theta = Size.angleFrontLeft_;
            vehicleLen = Size.toFrontLeft_;
            NPC_Number = RayCastHit::Number::frontLeft;
            break;
        case Oblique::frontRight://前右
            theta = Size.angleFrontRight_;
            vehicleLen = Size.toFrontRight_;
            NPC_Number = RayCastHit::Number::frontRight;
            break;
        case Oblique::rearLeft://後ろ左
            theta = Size.angleRearLeft_;
            vehicleLen = Size.toRearLeft_;
            NPC_Number = RayCastHit::Number::rearLeft;
            break;
        case Oblique::rearRight://後ろ右
            theta = Size.angleRearRight_;
            vehicleLen = Size.toRearRight_;
            NPC_Number = RayCastHit::Number::rearRight;
            break;
        }

        //回転行列
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(theta));	//Ｙ軸で回転させる行列   
        //回転
        XMVECTOR dirVec = XMVector3TransformCoord(worldVector_.z, matRot);
        VectorRotateMatrixZXY(dirVec);

        XMStoreFloat3(&wallCollideOblique[i].dir, dirVec);

        Model::RayCast(hModel, &wallCollideOblique[i]);  //レイを発射

        //当たったら
        if (wallCollideOblique[i].hit)
        {
            //NPC用
            SetRayCastHit(NPC_Number, wallCollideOblique[i]);

            //長さと加速度の判定
            if (wallCollideOblique[i].dist * wallCollideOblique[i].dist <
                (dirAccX * dirAccX + dirAccZ * dirAccZ) + (vehicleLen * vehicleLen))
            {
                if (VehicleRotateSlope(wallCollideOblique[i].normal, slopeLimitAngle_, NPC_Number) == false)
                {
                    //衝突する時の壁までの距離
                    XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideOblique[i].dist - vehicleLen, 0.0f };
                    ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                    //回転
                    VectorRotateMatrixZXY(ajustVec);
                    //壁にくっつく
                    transform_.position_.x += XMVectorGetX(ajustVec);
                    transform_.position_.y += XMVectorGetY(ajustVec);
                    transform_.position_.z += XMVectorGetZ(ajustVec);

                    //加速度を０にしたりはしない
                    //減速させる
                    acceleration_ *= wallReflectionForce_;

                    //壁反射
                    acceleration_ -= XMLoadFloat3(&wallCollideOblique[i].dir) * moveSPD_;
                }
            }
        }
    }
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

    Size.centerRightToLeft_ = Size.rightToLeft_ * 0.5f;
    //Size.centerTopToBottom_ = (Size.topToBottom_ + Size.wheelRemainder_) * 0.5f;
    Size.centerFrontToRear_ = Size.frontToRear_ * 0.5f;

    //角度
    Size.angleFrontLeft_    = XMConvertToDegrees(        acos(Size.toFront_ / Size.toFrontLeft_));
    Size.angleFrontRight_   = XMConvertToDegrees(XM_PI - acos(Size.toFront_ / Size.toFrontRight_));
    Size.angleRearLeft_     = XMConvertToDegrees(         acos(Size.toRear_  / Size.toFrontLeft_) + XM_PI);
    Size.angleRearRight_    = XMConvertToDegrees(XM_2PI - acos(Size.toRear_  / Size.toFrontRight_));
}

//ハンドルの操作
void Vehicle::HandleTurnLR(int LR)
{
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
    return Circuit::GetNextCheckPointPosition(pointCount_);
}
//次のチェックポイントまでの距離を取得
float Vehicle::GetNextCheckDistance()
{
    return *XMVector3LengthEst(XMLoadFloat3(GetNextCheckPosition()) - XMLoadFloat3(&transform_.position_)).m128_f32;
}

//ベクトルを回転行列で回転 (ZXY順)
void Vehicle::VectorRotateMatrixZXY(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateZ);
    vec = XMVector3TransformCoord(vec, matRotateX);
    vec = XMVector3TransformCoord(vec, matRotateY);
}

//ベクトルを回転行列で"逆"回転 (YXZ順)
void Vehicle::VectorRotateMatrixZXY_R(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateY_R);
    vec = XMVector3TransformCoord(vec, matRotateX_R);
    vec = XMVector3TransformCoord(vec, matRotateZ_R);
}

// 坂道に応じて車両を回転(X、Z軸)
bool Vehicle::VehicleRotateSlope(const XMVECTOR& normal, float limitAngle, int rayCastType)
{
    //法線のY軸を反転
    XMVECTOR normalR = normal * XMVECTOR{ 1.0f,-1.0f,1.0f,1.0f };

    //角度が指定角度より急だとやめる
    //角度計算は謎
    if (XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32) > limitAngle)
    {
        SetRayCastSlope(rayCastType, false);
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
        SetRayCastSlope(rayCastType, false);
        return false;
    }

    SetRayCastSlope(rayCastType, true);
    return true;
}

// NPC用レイキャストにデータセット
void Vehicle::SetRayCastHit(int number, const RayCastData& rcd)
{
    if (isPlayer_)
    {
        return;
    }

    rayCastHit_[number].dir = rcd.dir;
    rayCastHit_[number].dist = rcd.dist;
    rayCastHit_[number].end = rcd.end;
    rayCastHit_[number].hit = rcd.hit;
}

//NPCのレイキャスト用
void Vehicle::SetRayCastSlope(int number, bool flag)
{
    if (isPlayer_)
    {
        return;
    }

    rayCastHit_[number].road = flag;
}

//バウンディングボックスの衝突
void Vehicle::CollideBoundingBox(Vehicle* pVehicle)
{
    XMFLOAT3 posCenter = GetBoundingBoxCenter();
    XMVECTOR posVec = XMLoadFloat3(&posCenter);

    XMFLOAT3 OppCenter = pVehicle->GetBoundingBoxCenter();
    XMVECTOR oppVec = XMLoadFloat3(&OppCenter);

    XMVECTOR posToOpp = XMVector3Normalize(posVec - oppVec);

    //減速させる
    acceleration_ *= wallReflectionForce_;

    //これでもそれっぽい
    acceleration_ += posToOpp * *XMVector3LengthEst(pVehicle->GetAcceleration()).m128_f32
                    * pVehicle->GetMass() * collideBoxValue_;

    //acceleration_ += posToOpp * *XMVector3LengthEst(acceleration_).m128_f32;
    //acceleration_ += pVehicle->GetAcceleration();
    //acceleration_ += posToOpp * ((*XMVector3LengthEst(pVehicle->GetAcceleration()).m128_f32
    //    + *XMVector3LengthEst(acceleration_).m128_f32) * 0.5f);
}

//操作入力の反映
void Vehicle::InputReceive(const XMVECTOR& vecX, const XMVECTOR& vecZ)
{
    //リセット
    operation_.Refresh();

    //操作入力
    InputOperate();

    //ハンドルリセット
    handleFlag_ = false;

    //操作できない状態か？
    if (isOperationInvalid_)
    {
        return;
    }

    //ハンドルの操作
    if (operation_.inputNow[operation_.inputName::handleLeft])
    {
        HandleTurnLR(handleLeft_);
    }
    if (operation_.inputNow[operation_.inputName::handleRight])
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
        if (operation_.inputNow[operation_.inputName::moveFront])
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (operation_.inputNow[operation_.inputName::moveRear])
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //ブースト
    if (operation_.inputNow[operation_.inputName::boost])
    {
        //容量があるなら
        if (boostCapacity_ >= boostSpending_)
        {
            //消費
            boostCapacity_ -= boostSpending_;

            slideFlag_ = true;

            acceleration_ += vecZ * boostValue_;
        }
        else
        {
            slideFlag_ = false;
        }
    }
    else
    {
        slideFlag_ = false;

        if (landingFlag_)
        {
            boostCapacity_ += boostIncrease_;   //増える
        }

        //あふれる
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }

#ifdef _DEBUG
    //左右
    if (operation_.inputNow[operation_.inputName::turnLeft])
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (operation_.inputNow[operation_.inputName::turnRight])
    {
        transform_.rotate_.y += rotateSPD_;
    }

    //ジャンプ
    if (operation_.IsDown(operation_.inputName::jump))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (operation_.inputNow[operation_.inputName::moveLeft])
    {
        acceleration_ -= vecX;
    }
    if (operation_.inputNow[operation_.inputName::moveRight])
    {
        acceleration_ += vecX;
    }
#endif
}

//タイヤの高さセッター //車体との差分も計算
void Vehicle::SetWheelHeight(float height)
{
    Size.wheelHeight_ = height;

    Size.wheelRemainder_ = height - Size.wheelFL_.y;
    if (Size.wheelRemainder_ < 0.0f)
        Size.wheelRemainder_ = 0.0f;

    Size.toWheelBottom_ = Size.toBottom_ + Size.wheelRemainder_;
    Size.topToWheelBottom_ = Size.toWheelBottom_ + Size.toTop_;
    Size.centerPositionRemainder_ = Size.centerTopToBottom_ - Size.wheelRemainder_;
    Size.centerTopToBottom_ = (Size.topToBottom_ + Size.wheelRemainder_) * 0.5f;
}

//エフェクト
void Vehicle::VehicleParticle()
{
    float accLength = *XMVector3Length(acceleration_).m128_f32;

    //ゴールしたら
    if (goalFlag_)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::gold
            , transform_.position_);
    }

    //ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::gold
    //    , transform_.position_);

    //ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::spark
    //   , Model::GetBonePosition(hModel_, "rear"), acceleration_);

    //ブースト
    if (slideFlag_)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::boost
            , Model::GetBonePosition(hModel_, "rear"), acceleration_);
    }

    //走行中のタイヤの軌跡
    if (wheelParticleLength_ < accLength
        && wheelParticleLengthMax_ > accLength)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRL"));
    }

    if (landingFlag_ && wheelParticleLength_ < accLength)
    {
        switch (landingType_)
        {
        default:
            break;
        case Circuit::circuitType::turf://草地乗り上げ
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::grass
                , transform_.position_);
            break;
        case Circuit::circuitType::dirt://砂地
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::dirt
                , transform_.position_);
            break;
        }
    }
}

//車両の操作、入力の受付
void Vehicle::InputOperate() {};

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
#endif
