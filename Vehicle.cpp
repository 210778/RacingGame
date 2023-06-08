#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
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
#include "VehicleWheel.h"
#include "ParticlePackage.h"

using std::vector;
using std::string;

//コンストラクタ
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle"), hModel_(-1), hGroundModel_(-1)
    , acceleration_({ 0, 0, 0, 0 })
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10), wheelSpeed_(0), wheelDirection_({ 0, 0, 0, 0 })
    , handleRotate_(0), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(wheelFriction_ * 1.0f)
    , slideFlag_(false)
    , sideFriction_(0.2f), sideSlideFriction_(0.1f)
    , rayStartHeight(1.0f)
    , turnAdjust_(0.05f), driveAdjust_(20), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , pTextSpeed_(nullptr), pTextTime_(nullptr), pTextLap_(nullptr), pSpeedometer(nullptr), pTextEngine_(nullptr)
    , time_(0), goalFlag_(false), pointCount_(-1), lapCount_(0), lapMax_(1)
    , hImage_(-1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0, 0.0, 0.0, 0.0 })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
{
    Size.wheelHeight_ = 0.1;
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
    hModel_ = Model::Load("model\\Car1_blue.fbx");
    assert(hModel_ >= 0);

    hImage_ = Image::Load("image\\clearImage.png");
    assert(hImage_ >= 0);

    transform_.position_ = XMFLOAT3(0.0f, 0.0f, -200.0f);
    transform_.rotate_   = XMFLOAT3(0.0f, 0.0f, 0.0f);

    Viewer* pViewer = Instantiate<Viewer>(this);
    pViewer->SetPosition(transform_.position_);

    //サイズ計算
    SetVehicleSize(hModel_, "car1");

    //タイヤ
    int wheelModel = Model::Load("model\\wheel1.fbx");
    assert(wheelModel >= 0);
    MakeWheels(wheelModel);

    //当たり判定
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, Size.toCenter_, 0), Size.frontToRear_ * 0.5f);
    AddCollider(collision);

    //エフェクトのポインタとそれのまとめ
    pParticle_ = Instantiate<Particle>(this);

    //ステージオブジェクトを探す
    pGround_ = (Ground*)FindObject("Ground");
    assert(pGround_ != nullptr);
    //ちゃんとパーツが入っているか (これ以降はポインタがあってパーツがあることを前提にする)
    assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    lapMax_ = pGround_->GetCircuitUnion()->maxLap_;

    //文字
    pTextSpeed_ = new Text;
    pTextSpeed_->Initialize();

    pTextTime_ = new Text;
    pTextTime_->Initialize();

    pTextLap_ = new Text;
    pTextLap_->Initialize();

    pTextEngine_ = new Text;
    pTextEngine_->Initialize();

    //スピードメーター
    pSpeedometer = Instantiate<Speedometer>(GetParent());
}

//更新
void Vehicle::Update()
{
    //ゴールしたら
    if (goalFlag_)
    {
        ParticlePackage::ActRainbowFire(pParticle_, transform_.position_);
    }

    //走行中のタイヤの軌跡
    if (wheelParticleLength_ < *XMVector3Length(acceleration_).m128_f32)
    {
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "car1_wheelRR"));
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "car1_wheelRL"));
    }

    //クールタイム
    if (coolTime_ > 0)
        coolTime_--;

    //時間
    if (!(goalFlag_))
        time_++;

    //Ｇゴール  
    if (lapCount_ >= lapMax_)
        goalFlag_ = true;

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

    //前向きに進んでるか後ろ向きか判定
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
    {
        accZDirection_ = -1;
    }

    //ハンドルの操作
    handleFlag_ = false;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
    {
        HandleTurnLR(left);
    }
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
    {
        HandleTurnLR(right);
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

    if (landingFlag_)
    {
        //全身後退
        if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
        {
            acceleration_ += vecZ;
        }

        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            acceleration_ -= vecZ;
        }
    }

    //滑る
    if (Input::IsKeyDown(DIK_SPACE))
        slideFlag_ = true;
    if (Input::IsKeyUp(DIK_SPACE))
        slideFlag_ = false;

    //
    if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
    {
        acceleration_ += vecZ;

        XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "car1_rear");
        boosterPos.y += Size.toCenter_;
        ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
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
        vecPos += vecX * 30;
    }
    if (Input::IsKey(DIK_Q))
    {
        vecPos -= vecX * 30;
    }

    //上昇
    if (Input::IsKeyDown(DIK_M))
    {
        if (landingFlag_ || true)
        {
            acceleration_ += {0.0f,jumpForce_,0.0f,0.0f};
        }
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
    XMVECTOR vecFront = XMVector3NormalizeEst(vecZ);
    vecFront = XMVector3NormalizeEst(vecFront);//正規化
    frontVec_ = vecFront;

    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //地上ならハンドルに合わせて回転
    if (landingFlag_)
    {
        //前後に動いているなら
        transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_ * accZDirection_;
        handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
    }
    //ハンドルを戻す
    if (handleFlag_ == false)
        handleRotate_ *= handleAdjust_;

    //タイヤの角度と減速
    //正面やタイヤの方向へは減速しないが、タイヤの方向と平行のほうこうへは減速する
    //タイヤの方向と平行なら何もしないが、垂直に近いほどタイヤの方向にベクトルを発生させる
    if (landingFlag_)
    {
        XMVECTOR normalAcc = XMVector3Normalize(acceleration_);//正規化
        //進行方向と、車両の正面の、角度
        //これは精度を犠牲にして高速らしい。角度は0 ~ 180 の間なのでなんとか方向を判別したい
        //XMVector3NormalizeEst は長さが０だとバグるっぽい
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

    //長さの調整
    SpeedLimit(acceleration_, speedLimit_);

    //位置ベクトル　＋　加速度ベクトル
    vecPos += acceleration_;

    //ベクトルを位置に入れる
    XMStoreFloat3(&transform_.position_, vecPos);

    //接地、壁衝突
    VehicleCollide();

    //タイヤの値セット
    pWheels_->SetWheelSpeedRotate(
        *XMVector3LengthEst(acceleration_).m128_f32 * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //スピードメーター
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    pSpeedometer->SetSpeed(*XMVector3LengthEst(speedVec).m128_f32 * 120);

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
        BulletPos.y += Size.toCenter_;
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

        if (pCP->GetNumber() - 1 == pointCount_)
        {
            pointCount_++;
        }
        if (pCP->GetNumber() == 1 && pointCount_ == pGround_->GetCircuitUnion()->checkPointPosition_.size())
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
    lapStr += "/";
    lapStr += std::to_string(lapMax_);
    pTextLap_->Draw(30, 110, lapStr.c_str());

    //エンジン表示
    XMFLOAT3 floAcc;
    //ベクトルY軸で回転用行列
    XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
    XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));
    //string engineStr = std::to_string(engineRotate_);
    //string engineStr = std::to_string((int)transform_.rotate_.y);
    string engineStr = std::to_string(floAcc.x) + "/" + std::to_string(floAcc.y) + "/" + std::to_string(floAcc.z);

    //engineStr += "C'";//engineStr += "rpm";
    pTextEngine_->Draw(280, 30, engineStr.c_str());

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
}


//地面、壁、敵との衝突をまとめる？？
void Vehicle::VehicleCollide()
{
    //種類の分だけ
    for (int i = 0; i < pGround_->GetCircuitUnion()->parts_.size(); i++)
    {
        //地面
        Landing(pGround_->GetCircuitUnion()->parts_[i].model_, pGround_->GetCircuitUnion()->parts_[i].type_);
        
        //壁
        CollideWall(pGround_->GetCircuitUnion()->parts_[i].model_, pGround_->GetCircuitUnion()->parts_[i].type_);
    }
}

//接地 
void Vehicle::Landing(int hModel,int type)
{
    RayCastData data;
    data.start = transform_.position_;  //レイの発射位置
    data.start.y -= Size.wheelHeight_;  //地面に張り付いてるとうまくいかない この値ぶんだけ地面から浮く　タイヤの高さにする
    data.dir = XMFLOAT3(0, -1, 0);      //レイの方向
    Model::RayCast(hModel, &data);      //レイを発射
        
    //レイが当たったら
    if (data.hit)
    {
        if (type == Ground::turf)
            landingType_ = Ground::turf;
        if (type == Ground::road)
            landingType_ = Ground::road;

        if (-data.dist > XMVectorGetY(acceleration_) - gravity_)
        {
            //下方向の加速度が大きいなら　地面にワープ　落下速度を０
            transform_.position_.y -= data.dist;
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            landingFlag_ = true;
        }
        else
        {
            //位置を下げる
            acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
            landingFlag_ = false;
        }
    }

    //天井
    data.dir = XMFLOAT3(0, 1, 0);       //真上に発射
    Model::RayCast(hModel, &data);  //レイを発射

    if (data.hit)
    {   
        //ちょっと地面に埋まったとき
        if (data.dist < Size.toTop_)
        {
            //その分位置を上げる
            transform_.position_.y += data.dist;
            acceleration_ *= {1, 0, 1, 1};
        }
        //天井にぶつかったとき
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            //その分位置を上げる
            transform_.position_.y += data.dist - Size.toTop_;
            acceleration_ *= {1, 0, 1, 1};
        }
    }

    //接地表示
    if (landingFlag_)
    {
        transform_.scale_.y = 1;
    }
    else
    {
        transform_.scale_.y = 1.3f;
    }
}

void Vehicle::CollideWall(int hModel, int type)
{
    enum
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    } direction;

    //前後左右と斜めで分割することにする
    std::array<RayCastData, 4>wallCollideVertical;

    //前後左右編
    for (int i = 0; i < wallCollideVertical.size(); i++)
    {
        wallCollideVertical[i].start = transform_.position_;
        wallCollideVertical[i].start.y += Size.toCenter_;

        //90度ずつ回転
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(90 * i));	//Ｙ軸で回転させる行列    
        XMStoreFloat3(&wallCollideVertical[i].dir, XMVector3TransformCoord(frontVec_, matRot));//ベクトルを行列で変形

        Model::RayCast(hModel, &wallCollideVertical[i]);  //レイを発射
        //当たったら
        if (wallCollideVertical[i].hit)
        { 
            XMFLOAT3 floAcc;
            //ベクトルY軸で回転用行列
            XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
            XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));

            XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));

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
            case right:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = Size.toRight_;
                dirPlusMinus = 1.0f;
                break;
                //後ろ
            case rear:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toRear_;
                dirPlusMinus = -1.0f;
                break;
                //左
            case left:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toLeft_;
                dirPlusMinus = -1.0f;
                break;
            }

            if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
            {
                //衝突する直前で止まった時の壁までの距離
                XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - (dirSize * dirPlusMinus), 0.0f };
                ajustVec = XMVector3TransformCoord(ajustVec, matRot);
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);
                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);

                //平行移動させる
                float accY = XMVectorGetY(acceleration_);
                acceleration_ = wallCollideVertical[i].parallelism * *XMVector3LengthEst(acceleration_).m128_f32;
                acceleration_ = XMVectorSetY(acceleration_, accY);
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

void Vehicle::SetVehicleSize(int hModel, std::string modelName)
{
    //ボーン
    XMFLOAT3 vehicleBottom  = Model::GetBonePosition(hModel, modelName + "_bottom");
    XMFLOAT3 vehicleTop     = Model::GetBonePosition(hModel, modelName + "_top");
    XMFLOAT3 vehicleLeft    = Model::GetBonePosition(hModel, modelName + "_left");
    XMFLOAT3 vehicleRight   = Model::GetBonePosition(hModel, modelName + "_right");
    XMFLOAT3 vehicleRear    = Model::GetBonePosition(hModel, modelName + "_rear");
    XMFLOAT3 vehicleFront   = Model::GetBonePosition(hModel, modelName + "_front");
    Size.wheelFR_ = Model::GetBonePosition(hModel, modelName + "_wheelFR");
    Size.wheelFL_ = Model::GetBonePosition(hModel, modelName + "_wheelFL");
    Size.wheelRR_ = Model::GetBonePosition(hModel, modelName + "_wheelRR");
    Size.wheelRL_ = Model::GetBonePosition(hModel, modelName + "_wheelRL");
    //車両の大きさ計算

    Size.toRight_   = *XMVector3Length(XMLoadFloat3(&vehicleRight)).m128_f32;
    Size.toLeft_    = *XMVector3Length(XMLoadFloat3(&vehicleLeft)).m128_f32;
    Size.toFront_   = *XMVector3Length(XMLoadFloat3(&vehicleFront)).m128_f32;
    Size.toRear_    = *XMVector3Length(XMLoadFloat3(&vehicleRear)).m128_f32;
    Size.toTop_     = *XMVector3Length(XMLoadFloat3(&vehicleTop)).m128_f32;
    Size.toCenter_  = Size.toTop_ * 0.5f;
    Size.rightToLeft_   = Size.toRight_ + Size.toLeft_;
    Size.frontToRear_   = Size.toFront_ + Size.toRear_;
    Size.toFrontRight_  = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toRight_ * Size.toRight_));
    Size.toFrontLeft_   = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toLeft_ * Size.toLeft_));
    Size.toRearRight_   = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toRight_ * Size.toRight_));
    Size.toRearLeft_    = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toLeft_ * Size.toLeft_));
}

//ハンドルの操作
void Vehicle::HandleTurnLR(int LR)
{
    handleFlag_ = true;

    //滑ってると曲がりやすい
    if (slideFlag_)
        handleRotate_ += rotateSPD_ * slideHandleRotateAdd_ * LR;
    else
        handleRotate_ += rotateSPD_ * LR;
}

#if 0
void Vehicle::CollideWall(int hModel, int type)
{
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
                //acceleration_ += rayCar[i].normal * hitSpeed * -0.25f / sideFriction_;
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
#endif

#if 0
//初期化
void TestScene::Initialize()
{
    Camera::SetPosition(XMFLOAT3(0, 10, -20));
    Camera::SetTarget(XMFLOAT3(0, 0, 0));
    hModel_ = Model::Load("model\\GroundGrid.fbx");

    pParticle__ = Instantiate<Particle>(this);

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
        pParticle__->Start(data);

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
        pParticle__->Start(data);
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
        pParticle__->Start(data);
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
        pParticle__->Start(data);

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
        pParticle__->Start(data);
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
        pParticle__->Start(data);

        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle__->Start(data);
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
        pParticle__->Start(data);


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
        pParticle__->Start(data);
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
        pParticle__->Start(data);

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