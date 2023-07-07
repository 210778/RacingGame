#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

#include "Bullet.h"
#include "Ground.h"

//コンストラクタ
Bullet::Bullet(GameObject* parent)
    :GameObject(parent, "Bullet"), hModel_(-1),
    acceleration_(0.0f, 0.0f, 0.0f), life_(20)
{
    life_ *= 1;
}

//デストラクタ
Bullet::~Bullet()
{
}

//初期化
void Bullet::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole.fbx");//model\\TestBullet.fbx
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_;// = XMFLOAT3(0.4f, 0.4f, 0.4f);
}

//更新
void Bullet::Update()
{
    Accelerator();

    life_--;
    if (life_ <= 0)
    {
        KillMe();
    }
/*
XMFLOAT3 → XMVECTOR

XMFLOAT3 f;	//何か入ってるとして
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 ← XMVECTOR

XMVECTOR v; 	//何か入ってるとして
XMFLOAT3 f;
XMStoreFloat3(&f, v);
   
    XMVECTOR accVec = XMLoadFloat3(&acceleration_);
    transform_.rotate_ = XMFLOAT3(90.0f, 0.0f, 0.0f);
    XMVECTOR rotVec = XMLoadFloat3(&transform_.rotate_);
    XMStoreFloat3(&transform_.rotate_, accVec * rotVec);
    MessageBox(NULL, L"a", L" puzzleList_.size() ", MB_OK);
*/ 
#if 0
    Ground* pGround = (Ground*)FindObject("Ground"); //ステージオブジェクトを探す
    int hGroundModel = pGround->GetModelHandle();    //モデル番号を取得
    RayCastData data;
    data.start = transform_.position_;  //レイの発射位置
    data.start.y += 1;
    data.dir = acceleration_;           //レイの方向
    Model::RayCast(hGroundModel, &data); //レイを発射

    float length = 5.0f;    //よくわからない
    if (data.hit == true && data.dist < length)
    {
        //加速度をベクトル化
        XMVECTOR accVec = XMLoadFloat3(&acceleration_);
        //長さを覚える
        //float* pPower = XMVector3Length(accVec).m128_f32;
        //ベクトルを当たった法線に
        accVec = data.reflection;
        //accVec = XMVector3Normalize(accVec);//正規化
        //長さを戻す
        //accVec *= *pPower;
        //加速度に入れる
        XMStoreFloat3(&acceleration_, accVec);
    }
#endif
    //墓
#if 0
    XMVECTOR dirVec = XMLoadFloat3(&acceleration_);
    dirVec = XMVector3Normalize(dirVec);
    XMVECTOR headVec = { 0,1,0,0 };
    headVec = XMVector3Normalize(headVec);

    XMVECTOR crossed = XMVector3Cross(dirVec, headVec);

    float angle = acos(*XMVector3Dot(dirVec, headVec).m128_f32);

    transform_.rotate_.x = angle;
    transform_.rotate_.y = angle;
    transform_.rotate_.z = angle;


    //XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(30));	//Ｙ軸で30度回転させる行列
    //v = XMVector3TransformCoord(v, m);	//ベクトルｖを行列ｍで変形
#endif
#if 0
    //ベクトルの方向を向く http://marupeke296.com/DXG_No16_AttitudeControl.html
    XMVECTOR upVec      = XMLoadFloat3(&acceleration_);
    XMVECTOR vecNormal  = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //加速度

    //XMVECTOR absUp = XMVectorAbs(upVec) * XMVectorAbs(vecNormal);
    float* upLength = XMVector3Length(upVec).m128_f32;
    float* normalLength = XMVector3Length(vecNormal).m128_f32;

    float *dot = XMVector3Dot(upVec, vecNormal / (*upLength * *normalLength)).m128_f32;
    float rot1 = acos(*dot);
    //回転クォータニオン作成
    XMVECTOR ax1 = XMVector3Cross(upVec, vecNormal);  //姿勢制御回転軸ベクトル
    XMVECTOR qua1 = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    qua1 = XMQuaternionRotationAxis(ax1, rot1);//
    //方向合わせ回転
    XMVECTOR cq1;
    cq1 = XMQuaternionConjugate(qua1);//共役クォータニオン

    XMVECTOR odq = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); //ローカルの方向クォータニオン（Z軸）
    XMVECTOR res = XMQuaternionMultiply(cq1, odq);  //共役・ベクトル・ｑ１
    res = XMQuaternionMultiply(res, qua1);

    XMFLOAT3 floRes;
    XMStoreFloat3(&floRes, res);
    XMVECTOR od = XMVectorSet(floRes.x, floRes.y, floRes.z, 0.0f);  //姿勢合わせ回転後方向ベクトル

    XMVECTOR qua2   = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR accVec = XMLoadFloat3(&acceleration_); //加速度

    float* odLength = XMVector3Length(od).m128_f32;
    float* accLength = XMVector3Length(accVec).m128_f32;
    //float* dot = XMVector3Dot(upVec, vecNormal / (*upLength * *normalLength)).m128_f32;

    float rot2 = acos(*(XMVector3Dot(od, accVec / (*odLength * *accLength)).m128_f32));
    qua2 = XMQuaternionRotationAxis(vecNormal, rot2);
    //合成クォータニオンから回転行列を作成
    XMVECTOR qua = XMQuaternionMultiply(qua1, qua2);

    XMMATRIX rotMat = XMMatrixRotationQuaternion(qua);
    XMVECTOR vecRot = XMLoadFloat3(&transform_.rotate_);
    vecRot = XMVector3TransformCoord(vecRot, rotMat);
    //型を戻す
    XMStoreFloat3(&transform_.rotate_, vecRot);
#elif 0
    //ベクトルの方向を向く
    XMVECTOR posVec = XMLoadFloat3(&transform_.position_);
    XMVECTOR dirVec = XMLoadFloat3(&acceleration_);
    XMVECTOR upVec = { 90.0f,0.0f,0.0f,0.0f };

    XMMATRIX lookMat = XMMatrixLookAtRH(posVec, dirVec, upVec);
    XMVECTOR rotVec = { 90.0f,0.0f,0.0f,0.0f };
    rotVec = XMVector3TransformCoord(rotVec, lookMat);	//ベクトルｖを行列ｍで変形
    XMStoreFloat3(&transform_.rotate_, rotVec);
#endif
    //
#if 0
    //向きを変える
    XMVECTOR Z_Vec = { 0,0,1,0 };
    Z_Vec = XMVector3Normalize(Z_Vec); //正規化
    XMVECTOR SeekVec = XMLoadFloat3(&animal.statePos_[seek].goal);
    SeekVec = SeekVec - XMLoadFloat3(&animal.transform_.position_);
    SeekVec = XMVector3Normalize(SeekVec); //正規化
    float angle = *XMVector3AngleBetweenNormals(Z_Vec, SeekVec).m128_f32;
    angle = XMConvertToDegrees(angle);
    //
    if (animal.statePos_[seek].way > 0)
    {
        if (animal.transform_.position_.x > animal.statePos_[seek].goal.x)
        {
            angle = 360 - angle;
        }
    }
    else
    {
        if (animal.transform_.position_.x <= animal.statePos_[seek].goal.x)
        {
            angle -= 180;
        }
        else
        {
            angle = 180 - angle;
        }
    }
    animal.transform_.rotate_.y = angle;
#endif
}

//何かに当たった
void Bullet::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
}

//描画
void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Bullet::Release()
{
}

void Bullet::Accelerator()
{
    transform_.position_.x += acceleration_.x * 0.5f;
    transform_.position_.y += acceleration_.y * 0.5f;
    transform_.position_.z += acceleration_.z * 0.5f;
}