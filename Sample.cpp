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

    transform_.scale_ = { 3,3,3 };
}

//更新
void Sample::Update()
{
/*
XMFLOAT3 → XMVECTOR

XMFLOAT3 f;	//何か入ってるとして
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 ← XMVECTOR

XMVECTOR v; 	//何か入ってるとして
XMFLOAT3 f;
XMStoreFloat3(&f, v);
*/
}

//何かに当たった
void Sample::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
}

//描画
void Sample::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Sample::Release()
{
}
