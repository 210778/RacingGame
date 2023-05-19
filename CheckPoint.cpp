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
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 40.0f);
    AddCollider(collision);
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
    //当たったときの処理
}
