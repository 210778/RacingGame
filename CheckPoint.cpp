#include "CheckPoint.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//コンストラクタ
CheckPoint::CheckPoint(GameObject* parent)
    :GameObject(parent, "CheckPoint")
    ,number_(-1)
{
}

//デストラクタ
CheckPoint::~CheckPoint()
{
}

//初期化
void CheckPoint::Initialize()
{
}

//更新
void CheckPoint::Update()
{
}

//描画
void CheckPoint::Draw()
{
}

//開放
void CheckPoint::Release()
{
}

//何かに当たった
void CheckPoint::OnCollision(GameObject* pTarget)
{
}

//当たり判定を作る
void CheckPoint::MakeSphereCollider(const XMFLOAT3* position, float size, int number)
{
    //constなので
    transform_.position_ = *position;
    //transform_.position_.x *= -1;  //mayaとのずれの解消
    SphereCollider* collision = new SphereCollider({ 0.0f,0.0f,0.0f } , size);
    AddCollider(collision);
    number_ = number;
}