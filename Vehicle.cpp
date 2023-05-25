#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"

//#include "Engine/BillBoard.h"
#include "Engine/Particle.h"

#include "Vehicle.h"
#include "Ground.h"
#include "Bullet.h"
#include "Viewer.h"
#include "Sample.h"
#include "Tachometer.h"
#include "Tracker.h"
#include "Speedometer.h"
#include "CheckPoint.h"

using std::vector;
using std::string;

//コンストラクタ
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle"), hModel_(-1), hGroundModel_(-1)
    , acceleration_({ 0, 0, 0, 0 }), vehicleSize_(2, 2, 4)
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10), wheelSpeed_(0), wheelDirection_({ 0, 0, 0, 0 })
    , handleRotate_(0), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(wheelFriction_ * 1.02f)
    , slideFlag_(false)
    , sideFriction_(0.2f), sideSlideFriction_(0.1f)
    , rayStartHeight(1.0f)
    , turnAdjust_(0.05f), driveAdjust_(20), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , pMarker(nullptr), pTachometer(nullptr)
    , pTextSpeed_(nullptr), pTextTime_(nullptr), pTextLap_(nullptr), pSpeedometer(nullptr), pTextEngine_(nullptr)
    , time_(0), goalFlag_(false), pointCount_(-1), lapCount_(0)
    , hImage_(-1)
    //追加分
    , torque_(1.0f), mass_(1.0f), engineRotate_(0.0f), clutchFlag_(true)
    , landingType_(Ground::road)
    , pParticle(nullptr)
{
    vehicleSizeHalf_ = XMFLOAT3(vehicleSize_.x * 0.5f, vehicleSize_.y * 0.5f, vehicleSize_.z * 0.5f);
    //三平方の定理で斜めの値を求める
    vehicleSizeOblique_ = sqrt((vehicleSizeHalf_.x * vehicleSizeHalf_.x) + (vehicleSizeHalf_.z * vehicleSizeHalf_.z));

    XMFLOAT3 flo(0, 0, 0);
    frontVec_ = XMLoadFloat3(&flo);
}

//デストラクタ
Vehicle::~Vehicle()
{
}

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

//初期化
void Vehicle::Initialize()
{
    //hModel_ = Model::Load("model\\CarRed2.fbx");//"Assets\\model\\□□□.fbx"
    //hModel_ = Model::Load("model\\Ground12.fbx");
    hModel_ = Model::Load("model\\Car1_blue.fbx");
    assert(hModel_ >= 0);

    hImage_ = Image::Load("image\\clearImage.png");
    assert(hImage_ >= 0);

    transform_.position_ = XMFLOAT3(0.0f, 0.0f, -200.0f);
    transform_.rotate_ = XMFLOAT3(0.0f, 270.0f, 0.0f);

    Viewer* pViewer = Instantiate<Viewer>(this);
    pViewer->SetPosition(transform_.position_);

#ifdef _DEBUG
    pMarker = Instantiate<Sample>(GetParent());
    pMarker->SetPosition(transform_.position_);

    pTachometer = Instantiate<Tachometer>(GetParent());
    pTachometer->SetRotate(0, 90, 270);

    pSpeedometer = Instantiate<Speedometer>(GetParent());
#endif
    //文字
    pTextSpeed_ = new Text;
    pTextSpeed_->Initialize();

    pTextTime_ = new Text;
    pTextTime_->Initialize();

    pTextLap_ = new Text;
    pTextLap_->Initialize();

    pTextEngine_ = new Text;
    pTextEngine_->Initialize();

    //当たり判定
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, vehicleSizeHalf_.y, 0), vehicleSizeHalf_.z);
    AddCollider(collision);

    //BoxCollider* collisionB = new BoxCollider(XMFLOAT3(0, vehicleSizeHalf_.y, 0),
    //                                          XMFLOAT3(vehicleSize_.x, vehicleSize_.y, vehicleSize_.z));
    //AddCollider(collisionB);

    //炎
    pParticle = Instantiate<Particle>(this);

    //ボーン
    XMFLOAT3 vehicleBottom = Model::GetBonePosition(hModel_, "car1_bottom");
    XMFLOAT3 vehicleTop = Model::GetBonePosition(hModel_, "car1_top");
    XMFLOAT3 vehicleLeft = Model::GetBonePosition(hModel_, "car1_left");
    XMFLOAT3 vehicleRear = Model::GetBonePosition(hModel_, "car1_rear");
    XMFLOAT3 vehicleWheelFR = Model::GetBonePosition(hModel_, "car1_wheelFR");


    //XMFLOAT3 shotPos = Model::GetBonePosition(hModel_, "ShotPos");
    //XMFLOAT3 CannonRoot = Model::GetBonePosition(hModel_, "CannonRoot");

    int wheelModel = Model::Load("model\\wheel1.fbx");
    assert(wheelModel >= 0);
    XMFLOAT3 wheelPos = Model::GetBonePosition(wheelModel, "wheel1_center");
    XMFLOAT3 wheelBotPos = Model::GetBonePosition(wheelModel, "wheel1_bottom");
}

//更新
void Vehicle::Update()
{
    if (Input::IsKey(DIK_C))
    {
        static unsigned int rainbowTimer = 0;
        static const int colorSpeed = 2;
        static const float operand = 0.0027778f;// 1 / 360

        rainbowTimer += colorSpeed;

        XMFLOAT4 HSV((rainbowTimer % 360) * operand, 1.0, 1.0, 0.5);//色相、彩度、明度、アルファ
        XMVECTOR vecRainbowRGB = XMColorHSVToRGB(XMLoadFloat4(&HSV));
  
        EmitterData data;

        //炎
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = transform_.position_;
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        //data.delay = 1;
        data.number = 2;
        data.lifeTime = 15;
        data.gravity = 0.0f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(20, 0, 20);
        data.speed = 0.75f;
        data.speedErr = 0.2;
        data.size = XMFLOAT2(5, 5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(0.8, 0.8);
        XMStoreFloat4(&data.color, vecRainbowRGB);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.02);
        pParticle->Start(data);

        //火の粉
        data.number = 1;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(0.98, 0.98);
        data.lifeTime = 30;
        data.speed = 0.2f;
        data.gravity = 0.005;
        pParticle->Start(data);
    }

    //クールタイム
    if (coolTime_ > 0)
    {
        coolTime_--;
    }

    //時間
    if (!(goalFlag_))
        time_++;

    //Ｇゴール  
    if (lapCount_ >= 1)//3
        goalFlag_ = true;

    //滑る
    if (Input::IsKeyDown(DIK_SPACE))
        slideFlag_ = true;
    if (Input::IsKeyUp(DIK_SPACE))
        slideFlag_ = false;

    //接地
    //Landing();

    //それぞれの値に合わせて軸回転させる行列
    XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX matRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));

    //現在位置をベクトルにしておく
    XMVECTOR vecPos = XMLoadFloat3(&transform_.position_);

    //行列を用意
    XMVECTOR vecX = { moveSPD_, 0, 0 ,0 };
    XMVECTOR vecY = { 0, moveSPD_, 0 ,0 };
    XMVECTOR vecZ = { 0, 0, moveSPD_ ,0 };

    //ベクトル * 行列
    vecZ = XMVector3TransformCoord(vecZ, matRotateY);
    vecX = XMVector3TransformCoord(vecX, matRotateY);

    //ハンドルの操作
    handleFlag_ = false;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
    {
        handleFlag_ = true;
        //滑ってると曲がりやすい
        if (slideFlag_)
            handleRotate_ -= rotateSPD_ * slideHandleRotateAdd_;
        else
            handleRotate_ -= rotateSPD_;
    }
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
    {
        handleFlag_ = true;
        if (slideFlag_)
            handleRotate_ += rotateSPD_ * slideHandleRotateAdd_;
        else
            handleRotate_ += rotateSPD_;
    }

    //ハンドル角度制限
    if (handleRotate_ != 0)
    {
        //曲がりやすい
        if (slideFlag_)
            AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
        else
            AngleLimit(handleRotate_, handleRotateMax_);
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
#endif

    //全身後退
    if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
    {
        acceleration_ += vecZ;
    }

    if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
    {
        acceleration_ -= vecZ;
    }

#ifdef _DEBUG
    if (Input::IsKey(DIK_E))
    {
        vecPos += vecX * 30;
    }
    if (Input::IsKey(DIK_Q))
    {
        vecPos -= vecX * 30;
    }

    //上昇下降
    if (Input::IsKeyDown(DIK_M))
    {
        if (landingFlag_)
        {
            acceleration_ += {0.0f,jumpForce_,0.0f,0.0f};
        }
    }

    //
    if (Input::IsKey(DIK_LSHIFT))
    {
        //vecPos -= vecY;
        acceleration_ += vecZ;

        EmitterData data;

        //炎
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = transform_.position_;
        data.position.y += vehicleSizeHalf_.y;
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.number = 3;
        data.lifeTime = 60;
        data.gravity = 0.0f;
        //data.dir = XMFLOAT3(0, 1, 0);
        XMStoreFloat3(&data.dir, -vecZ);
        data.dirErr = XMFLOAT3(30, 30, 30);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1.0, 1.0);
        data.sizeErr = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(1.075, 1.075);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.04, 0, -0.03);
        pParticle->Start(data);

        //火の粉
        data.number = 3;
        data.positionErr = XMFLOAT3(0.5, 0.5, 0.5);
        data.dirErr = XMFLOAT3(50, 50, 50);
        data.size = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle->Start(data);
    }

    if (Input::IsKey(DIK_V))
    {
        acceleration_ -= vecX * 2;
    }
    if (Input::IsKey(DIK_B))
    {
        acceleration_ += vecX * 2;
    }
#endif

    //ベクトル加速度 摩擦抵抗　加速度も変える
    if (slideFlag_)
    {
        //acceleration_ *= {airFriction_, 1, airFriction_, 1};
        acceleration_ *= {wheelFriction_, 1, wheelFriction_, 1};
    }
    else
    {
        acceleration_ *= {wheelFriction_, 1, wheelFriction_, 1};
    }

    //芝生状態
    if (landingType_ == Ground::turf)
    {
        acceleration_ *= {turfFriction_, 1, turfFriction_, 1};
    }

    //ハンドルと旋回
    XMMATRIX matHandleRotate = XMMatrixRotationY(XMConvertToRadians(handleRotate_));
    //正面の行列を用意
    XMVECTOR vecFront = XMVector3Normalize(vecZ);
    vecFront = XMVector3Normalize(vecFront);//正規化
    frontVec_ = vecFront;

    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //地上ならハンドルに合わせて回転
    if (landingFlag_)
    {
        if (accLength > 0)
        {
            //前後に動いているなら
            transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_;
            handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
        }
    }
    //ハンドルを戻す
    if (handleFlag_ == false)
    {
        handleRotate_ *= handleAdjust_;
    }

    //タイヤの角度と減速
    //正面やタイヤの方向へは減速しないが、タイヤの方向と平行のほうこうへは減速する
    //タイヤの方向と平行なら何もしないが、垂直に近いほどタイヤの方向にベクトルを発生させる
    if (landingFlag_)
    {
        XMVECTOR normalAcc = XMVector3Normalize(acceleration_);//正規化
        //進行方向と、車両の正面の、角度
        //これは精度を犠牲にして高速らしい。角度は0 ~ 180 の間なのでなんとか方向を判別したい
        /*
        https://www.hiramine.com/programming/graphics/2d_vectorangle.html
        ２次元の２つのベクトル（ベクトルＡ、ベクトルＢ）の外積 (OA × OB) を求めたとき、外積の値が、
        正の値のとき、ベクトルＢは、ベクトルＡの進行方向に対して、左側を向いている、
        負の値のとき、ベクトルＢは、ベクトルＡの進行方向に対して、右側を向いている、という性質があります。
        */

        //左だとy軸が上向き+、右だと下向き-
        XMVECTOR outerProduct = XMVector3Cross(vecFront, normalAcc);
        //回転して利用する
        XMMATRIX MatRotateZ = XMMatrixRotationZ(XMConvertToRadians(90));
        outerProduct = XMVector3TransformCoord(outerProduct, MatRotateZ);
        //外積を回転
        outerProduct = XMVector3TransformCoord(outerProduct, matRotateY);

        /*
        * 遅いときは遠心力が弱く、速いときは強い、摩擦の影響を受ける
        * 
        * やりたいこと：車の前後へは抵抗が少ないが、左右方向への抵抗は多い。
        *               左右から押された場合(カーブなど)は辻つま合わせで押されている反対方向へ押し返す。
        *               これは押す力(車の左右方向の力)が強いほど摩擦が負けて弱い力で押し返す
        * 　問題：滑りを切り替えた時に左右に動く
        */

        float ajust = sideFriction_;
        if (slideFlag_)
            ajust = sideSlideFriction_;

        acceleration_ += (*XMVector3LengthEst(acceleration_).m128_f32) * outerProduct * ajust;
    }

    //正面のベクトルをハンドルの角度で回転
    XMMATRIX handleMatRotate = XMMatrixRotationY(XMConvertToRadians(handleRotate_));

#ifdef _DEBUG
    //テスト　矢印を表示
    XMFLOAT3 floAcc;
    XMStoreFloat3(&floAcc, acceleration_);
    //スピード
    pSpeedometer->SetSpeed(*XMVector3LengthEst(acceleration_).m128_f32 * 120);
    pTachometer->SetPosition(XMFLOAT3(transform_.position_.x, transform_.position_.y + 3, transform_.position_.z));
    pTachometer->SetRotate(0, handleRotate_ + 90 + transform_.rotate_.y, transform_.rotate_.z + 270);
    pTachometer->SetScale(pTachometer->GetScale().x, *XMVector3LengthEst(acceleration_).m128_f32, pTachometer->GetScale().z);

    //テスト　軌跡を設置
    static int seconds = 0;
    seconds++;
    if (seconds % 6 == 0 && *XMVector3LengthEst(acceleration_).m128_f32 > 0)
    {
        Tracker* pTracker = Instantiate<Tracker>(GetParent());
        pTracker->SetPosition(transform_.position_.x, transform_.position_.y + 1, transform_.position_.z);
        pTracker->SetScale(0.5, 0.5, 0.5);
    }
#endif

    //長さの調整
    SpeedLimit(acceleration_, speedLimit_);

    //位置ベクトル　＋　加速度ベクトル
    vecPos += acceleration_;
    //ベクトルを位置に入れる
    XMStoreFloat3(&transform_.position_, vecPos);

    //接地、壁衝突　（移動してきた）
    Landing();

#ifdef _DEBUG
    //テスト 正面にマーカーを設置
    XMVECTOR vecMark = { 0, 10, -1, 0 };    //後ろに伸びるベクトルを用意
    vecMark = XMVector3TransformCoord(vecMark, matRotateX);        //それを戦車の向きに合わせて回転
    vecMark = XMVector3TransformCoord(vecMark, matRotateY);        //それを戦車の向きに合わせて回転2
    vecMark = XMVector3TransformCoord(vecMark, matRotateZ);        //それを戦車の向きに合わせて回転3

    XMVECTOR rearVec = vecMark + vecPos;
    XMVECTOR frontVec = { transform_.position_.x,transform_.position_.y + 10,transform_.position_.z,0 };
    XMVECTOR bulletVec = frontVec - rearVec;
    //テスト
    bulletVec = XMVector3Normalize(vecFront);

    //弾を発射
    if (Input::IsMouseButton(0) && coolTime_ <= 0)
    {
        Bullet* pBullet = Instantiate<Bullet>(GetParent());
        XMFLOAT3 BulletPos = transform_.position_;
        BulletPos.y += vehicleSizeHalf_.y;
        pBullet->SetPosition(BulletPos);

        float power = bulletPower_;
        bulletVec *= power;
        XMFLOAT3 speed;
        XMStoreFloat3(&speed, bulletVec);
        pBullet->SetSpeed(speed);

        coolTime_ = heatAdd_;
    }
#endif
}

//何かに当たった
void Vehicle::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;
        if (pCP->GetNumber() == 1 && pointCount_ <= 0)
            pointCount_ = 1;
        else if (pCP->GetNumber() == 2 && pointCount_ == 1)
            pointCount_ = 2;
        else if (pCP->GetNumber() == 3 && pointCount_ == 2)
            pointCount_ = 3;
        else if (pCP->GetNumber() == 1 && pointCount_ == 3)
        {
            pointCount_ = 0;
            lapCount_++;
        }
    }
}

//描画
void Vehicle::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    //時速表示
    //平面のベクトルの長さ
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    string speedStr = std::to_string((float)(*XMVector3LengthEst(speedVec).m128_f32 * 120));
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
    string timeStr = std::to_string(min) + ":" + std::to_string(sec) + ":" + std::to_string(mSec);
    pTextTime_->Draw(30, 70, timeStr.c_str());

    //周回数表示
    string lapStr = std::to_string(lapCount_);
    lapStr += "/1";//"/3"
    pTextLap_->Draw(30, 110, lapStr.c_str());

    //エンジン表示
    string engineStr = std::to_string(engineRotate_);
    //string engineStr = std::to_string((int)transform_.rotate_.y);

    engineStr += "C'";//engineStr += "rpm";
    pTextEngine_->Draw(260, 30, engineStr.c_str());

    if (goalFlag_)
    {
        Transform imageTrans;
        imageTrans.position_ = XMFLOAT3(1.0f, 1.0f, 0.0f);
        imageTrans.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

        Image::SetRect(hImage_, 0, 0, 1024, 128);
        Image::SetTransform(hImage_, imageTrans);
        Image::Draw(hImage_);
    }

}

//開放
void Vehicle::Release()
{
    pTextSpeed_->Release();
    pTextTime_->Release();
    pTextLap_->Release();
    pTextEngine_->Release();
}
/*
XMFLOAT3 → XMVECTOR

XMFLOAT3 f;	//何か入ってるとして
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 ← XMVECTOR

XMVECTOR v; 	//何か入ってるとして
XMFLOAT3 f;
XMStoreFloat3(&f, v);
*/
/*
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
    if (fabs(angle) > fabs(limit))
    {
        angle = fabs(limit);

        //マイナスだったらマイナスに戻す
        if (minusFlag == true)
        {
            angle *= -1;
        }
    }
}

//接地 
void Vehicle::Landing()
{
    Ground* pGround = nullptr;
    pGround = (Ground*)FindObject("Ground");    //ステージオブジェクトを探す

    //安全
    if (pGround->GetCircuitUnion()->parts_.empty())
        return;

    //種類の分だけ
    for (int i = 0; i < pGround->GetCircuitUnion()->parts_.size(); i++)
    {
        //
        int hModel = pGround->GetCircuitUnion()->parts_[i].model_;

        RayCastData data;
        data.start = transform_.position_;   //レイの発射位置
        data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
        Model::RayCast(hModel, &data);  //レイを発射

        
        //レイが当たったら
        if (data.hit)
        {
            if (pGround->GetCircuitUnion()->parts_[i].type_ == Ground::turf)
                landingType_ = Ground::turf;
            if (pGround->GetCircuitUnion()->parts_[i].type_ == Ground::road)
                landingType_ = Ground::road;

            //フロートにしとく
            XMFLOAT3 floAcc;
            XMStoreFloat3(&floAcc, acceleration_);

            if (data.dist > abs(floAcc.y - gravity_))
            {
                //位置を下げる
                floAcc.y -= gravity_;
                landingFlag_ = false;
            }
            else// if (data.dist < floAcc.y && data.dist > 0 && floAcc.y < 0)
            {
                //下方向の加速度が大きいなら　地面にワープ　落下速度を０
                transform_.position_.y -= data.dist;
                floAcc.y = 0;
                landingFlag_ = true;
            }
            //戻す
            acceleration_ = XMLoadFloat3(&floAcc);
        }

        RayCastData ceiling;
        ceiling.start = transform_.position_;   //レイの発射位置

        ceiling.dir = XMFLOAT3(0, 1, 0);       //真上に発射
        Model::RayCast(hModel, &ceiling);  //レイを発射

        if (ceiling.hit && ceiling.dist < vehicleSizeHalf_.y)
        {
            //その分位置を上げる
            transform_.position_.y += ceiling.dist;
            acceleration_ *= {1, 0, 1, 1};
            landingFlag_ = true;
        }

        //壁
        RayCastData wallData;
        wallData.start = transform_.position_;   //レイの発射位置
        wallData.start.y += vehicleSizeHalf_.y;

        //斜め***
        //z軸からの角度
        float theta = acos(vehicleSizeHalf_.z / vehicleSizeOblique_);

        XMVECTOR rotVec = frontVec_;

        //配列にする
        vector<RayCastData> rayCar(4);
        for (int i = 0; i < rayCar.size(); i++)
        {
            rayCar[i].start = transform_.position_;
            rayCar[i].start.y += vehicleSizeHalf_.y;

            //90度ずつ回転
            XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(90 * i));	//Ｙ軸で回転させる行列    
            rotVec = XMVector3TransformCoord(frontVec_, mat);	//ベクトルを行列で変形

            XMStoreFloat3(&rayCar[i].dir, rotVec);

            Model::RayCast(hModel, &rayCar[i]);  //レイを発射
            //レイが当たったら
            if (rayCar[i].hit)
            {

                //調整           
                 //フロートにしとく
                XMFLOAT3 floAcc;
                //XMStoreFloat3(&floAcc, acceleration_);
                //加速度を逆回転させたベクトル
                XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
                XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY));

                //正面行き、車両を正しい位置にワープさせ、加速度を正しく０にする
                if (i == 0 && rayCar[i].dist < floAcc.z + vehicleSizeHalf_.z)
                {
                    acceleration_ *= {0, 1, 1, 1};
                    engineRotate_ = rayCar[i].dist;
                    //engineRotate_ = floAcc.z + vehicleSizeHalf_.z - rayCar[i].dist;

                    XMVECTOR ajustVec = { 0,0,floAcc.z + vehicleSizeHalf_.z - rayCar[i].dist,0 };
                    XMMATRIX matAjustRotateY = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
                    XMVector3TransformCoord(ajustVec, matAjustRotateY);
                    XMFLOAT3 ajustFlo;
                    XMStoreFloat3(&ajustFlo, ajustVec);
                    //transform_.position_.x += ajustFlo.x;
                    //transform_.position_.y += ajustFlo.y;
                    transform_.position_.z += ajustFlo.z;

                    pMarker->SetPosition(rayCar[i].end);
                }
                else// if (data.dist < floAcc.y && data.dist > 0 && floAcc.y < 0)
                {
                    //下方向の加速度が大きいなら　地面にワープ　落下速度を０
                    //transform_.position_.y -= data.dist;
                    //floAcc.y = 0;
                    //landingFlag_ = true;
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
                    acceleration_ += rayCar[i].normal * hitSpeed * -0.25f / sideFriction_;
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
            rotVec = XMVector3TransformCoord(frontVec_, mat);	//ベクトルを行列で変形
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

    //接地表示
    if (landingFlag_)
    {
        transform_.scale_.y = 1;
    }
    else
    {
        transform_.scale_.y = 1.1f;
    }
}

void Vehicle::CollideWall()
{
    return;
}

//地面、壁、敵との衝突をまとめる？？
void Vehicle::VehicleCollide()
{
    return;
}

#if 0
//初期化
void TestScene::Initialize()
{
    Camera::SetPosition(XMFLOAT3(0, 10, -20));
    Camera::SetTarget(XMFLOAT3(0, 0, 0));
    hModel_ = Model::Load("model\\GroundGrid.fbx");

    pParticle_ = Instantiate<Particle>(this);

    //炎
    {
        EmitterData data;

        //炎
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-4, 1.5, -4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1.5, 1.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);
        pParticle_->Start(data);

        //火の粉
        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle_->Start(data);
    }


    //煙
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(4, 1.5, 4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 150;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2, 2);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 1, 0.2);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.002);
        pParticle_->Start(data);
    }

    //水
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\Water.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.8, 0.4);
        data.scale = XMFLOAT2(1.02, 1.02);
        data.color = XMFLOAT4(1, 1, 1, 0.1);
        pParticle_->Start(data);

        //水滴
        data.textureFileName = "image\\PaticleAssets\\bubleB.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.positionErr = XMFLOAT3(0.5, 0, 0);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.3, 0.3);
        data.sizeErr = XMFLOAT2(0, 0);
        data.scale = XMFLOAT2(0.98, 0.98);
        data.color = XMFLOAT4(1, 1, 1, 1);
        pParticle_->Start(data);
    }

    //1
    {
        EmitterData data;
        //wind
        data.textureFileName = "image\\PaticleAssets\\circle_B.png";
        data.position = XMFLOAT3(-8, 1, 5);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2.5, 2.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(0.5, 0.8, 0.0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle_->Start(data);

        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle_->Start(data);
    }

}

//更新
void TestScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE))
    {
        EmitterData data;
        data.textureFileName = "image\\PaticleAssets\\CloudC.png";
        data.position = XMFLOAT3(0, 0.05, 0);
        data.delay = 0;
        data.number = 80;
        data.lifeTime = 20;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.1f;
        data.speedErr = 0.8;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.05, 1.05);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
        pParticle_->Start(data);


        data.delay = 0;
        data.number = 80;
        data.lifeTime = 100;
        data.positionErr = XMFLOAT3(0.5, 0, 0.5);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.25f;
        data.speedErr = 1;
        data.accel = 0.93;
        data.size = XMFLOAT2(0.1, 0.1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(0.99, 0.99);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, 0);
        data.gravity = 0.003f;
        pParticle_->Start(data);
    }

    if (Input::IsKey(DIK_V))
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 2;
        data.gravity = 0;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.05f;
        data.speedErr = 0.01f;
        data.size = XMFLOAT2(1.25, 1.25);
        data.sizeErr = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle_->Start(data);

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.1, 0.1, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 30;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.002;
        data.speed = 0.2f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.1, 0.1);
        data.scale = XMFLOAT2(0.9, 0.9);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle_->Start(data);
    }

    if (Input::IsKey(DIK_B))
    {
        EmitterData data;

        //炎
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-5, 1.0, -4);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0.15);
        data.delay = 0;
        data.number = 4;
        data.lifeTime = 15;
        data.gravity = 0.001f;
        data.dir = XMFLOAT3(-1, 0, 0);
        data.dirErr = XMFLOAT3(0.75, 0.75, 0.75);
        data.speed = 0.4f;
        data.speedErr = 0.01;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(1.15, 1.15);
        data.color = XMFLOAT4(1, 0.2, 0, 1);
        data.deltaColor = XMFLOAT4(0.0, 0.02, 0.01, -0.075);
        pParticle_->Start(data);
    }
}
#endif
//power oh-ra
#if 0
if (!Input::IsKey(DIK_C))
{
    EmitterData data;

    //炎
    data.textureFileName = "image\\PaticleAssets\\CloudA.png";
    data.position = transform_.position_;
    data.positionErr = XMFLOAT3(0.1, 0, 0.1);
    //data.delay = 1;
    data.number = 1;
    data.lifeTime = 10;
    data.gravity = -0.002f;
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(0, 0, 0);
    data.speed = 0.75f;
    data.speedErr = 0.2;
    data.size = XMFLOAT2(5, 5);
    data.sizeErr = XMFLOAT2(0.4, 0.4);
    data.scale = XMFLOAT2(0.9, 0.9);
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.1, -0.2);
    pParticle->Start(data);

    //火の粉
    data.number = 1;
    data.positionErr = XMFLOAT3(0.8, 0, 0.8);
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(10, 10, 10);
    data.size = XMFLOAT2(0.5, 0.5);
    data.scale = XMFLOAT2(0.98, 0.98);
    data.lifeTime = 30;
    data.speed = 0.2f;
    data.gravity = 0.005;
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.02, -0.015);
    pParticle->Start(data);
}
#endif