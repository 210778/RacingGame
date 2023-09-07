#include "Engine/Model.h"
#include "Background.h"

//�R���X�g���N�^
Background::Background(GameObject* parent)
    :GameObject(parent, "Background"), hModel_(-1)

{
}

//�f�X�g���N�^
Background::~Background()
{
}

//������
void Background::Initialize()
{
    hModel_ = Model::Load("model\\SkyFieldSphere.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Background::Update()
{
}

//�`��
void Background::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);


}

//�J��
void Background::Release()
{
}