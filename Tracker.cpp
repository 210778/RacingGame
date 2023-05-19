#include "Engine/Model.h"
#include "Engine/Input.h"

#include "Tracker.h"

//コンストラクタ
Tracker::Tracker(GameObject* parent)
    :GameObject(parent, "Tracker"), hModel_(-1)
    , life_(3)
{
    life_ *= 60;
}

//デストラクタ
Tracker::~Tracker()
{
}

//初期化
void Tracker::Initialize()
{
    hModel_ = Model::Load("model\\GreenCube.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = XMFLOAT3(20.0f, 20.0f, 20.0f);
}

//更新
void Tracker::Update()
{
    if (life_ <= 0)
    {
        KillMe();
    }
    else
    {
        life_--;
    }
}

//描画
void Tracker::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Tracker::Release()
{
}
