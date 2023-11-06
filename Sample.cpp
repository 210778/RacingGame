#include "Engine/Model.h"
#include "Engine/Image.h"

#include "Sample.h"

//コンストラクタ
Sample::Sample(GameObject* parent)
    :GameObject(parent, "Sample"), hModel_(-1)
    , modelScale_({ 15.0f,15.0f,15.0f })

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

    transform_.scale_ = modelScale_;
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
