#include "Engine/Model.h"
#include "MeasurePole.h"

//コンストラクタ
MeasurePole::MeasurePole(GameObject* parent)
    :GameObject(parent, "MeasurePole"), hModel_(-1)
{
}

//デストラクタ
MeasurePole::~MeasurePole()
{
}

//初期化
void MeasurePole::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole.fbx");
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(1.5f, 1.0f, 1.5f);
}

//更新
void MeasurePole::Update()
{
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

    XMVECTOR startVec = XMLoadFloat3(start);
    XMVECTOR sToE = *direction - XMLoadFloat3(start);

}