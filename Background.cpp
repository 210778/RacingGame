#include "Engine/Model.h"
#include "Background.h"
#include "Engine/Direct3D.h"

//コンストラクタ
Background::Background(GameObject* parent)
    :GameObject(parent, "Background"), hModel_(-1)

{
}

//デストラクタ
Background::~Background()
{
}

//初期化
void Background::Initialize()
{
    hModel_ = Model::Load("model\\SkyFieldCylinder_2.fbx");
    assert(hModel_ >= 0);
}

//更新
void Background::Update()
{
}

//描画
void Background::Draw()
{
    //影なし
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_, Direct3D::SHADER_SIMPLE3D);
}

//開放
void Background::Release()
{
}
