#include "CheckPoint.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//コンストラクタ
CheckPoint::CheckPoint(GameObject* parent)
    :GameObject(parent, "CheckPoint")

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
void CheckPoint::MakeSphereCollider(XMFLOAT3 position, float size)
{
    SphereCollider* collision = new SphereCollider(position, size);
    AddCollider(collision);
}