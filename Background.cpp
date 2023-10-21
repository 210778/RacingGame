#include "Engine/Model.h"
#include "Background.h"

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
    hModel_ = Model::Load("model\\SkyFieldSphere.fbx");
    assert(hModel_ >= 0);
}

//更新
void Background::Update()
{
}

//描画
void Background::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    Direct3D::SetShader(Direct3D::SHADER_BILLBOARD);
    Direct3D::SetBlendMode(Direct3D::BLEND_ADD);
}

//開放
void Background::Release()
{
}
