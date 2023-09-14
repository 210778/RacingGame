#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/Camera.h"

#include "Sample.h"

//コンストラクタ
Sample::Sample(GameObject* parent)
    :GameObject(parent, "Sample"), hModel_(-1)

{
}

//デストラクタ
Sample::~Sample()
{
}

//初期化
void Sample::Initialize()
{
    hModel_ = Model::Load("model\\Tri_Lines.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = { 15,15,15 };
}

//更新
void Sample::Update()
{
}

//何かに当たった
void Sample::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
}

//描画
void Sample::Draw()
{
#ifdef _DEBUG
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
#endif
}

//開放
void Sample::Release()
{
}
