#include "Engine/Model.h"
#include "MeasurePole.h"

//コンストラクタ
MeasurePole::MeasurePole(GameObject* parent)
    :GameObject(parent, "MeasurePole"), hModel_(-1)
    , life_(1)
{
}

//デストラクタ
MeasurePole::~MeasurePole()
{
}

//初期化
void MeasurePole::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole_Z.fbx");
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(10.0f, 10.0f, 10.0f);
}

//更新
void MeasurePole::Update()
{
    if (life_-- <= 0)
        KillMe();
}

//描画
void MeasurePole::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void MeasurePole::Release()
{
}

//レイキャストの可視化
void MeasurePole::ViewRayCast(const XMFLOAT3* start, const XMVECTOR* direction, const float length)
{
    transform_.scale_.y = length;

    XMVECTOR sToE = XMVector3Normalize(*direction) - XMVector3Normalize((XMLoadFloat3(start)));
    XMVECTOR vecS = (XMLoadFloat3(start));
    XMVECTOR vecZ = { 0.0f,0.0f,1.0f,0.0f };
    XMVECTOR vecX = { 1.0f,0.0f,1.0f,0.0f };
    XMVECTOR vecY = { 0.0f,1.0f,0.0f,0.0f };

    XMVECTOR dir = *direction;
    transform_.rotate_.x = Calculator::AngleBetweenVector(dir * XMVECTOR{ 0.0f, 1.0f, 1.0f, 1.0f }, vecZ);
    transform_.rotate_.y = Calculator::AngleBetweenVector(dir * XMVECTOR{ 1.0f, 0.0f, 1.0f, 1.0f }, vecZ) + 180;
    transform_.rotate_.z = Calculator::AngleBetweenVector(dir * XMVECTOR{1.0f, 1.0f, 0.0f, 1.0f}, vecX);
}

//オーバーロード
void MeasurePole::ViewRayCast(const XMFLOAT3* start, const XMFLOAT3* direction, const float length)
{
    XMVECTOR vecDir = XMLoadFloat3(direction);
    ViewRayCast(start, &vecDir, length);
}