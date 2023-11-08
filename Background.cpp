#include "Engine/Model.h"
#include "Background.h"
#include "Engine/Direct3D.h"

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
    hModel_ = Model::Load("model\\SkyFieldCylinder_2.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Background::Update()
{
}

//�`��
void Background::Draw()
{
    //�e�Ȃ�
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_, Direct3D::SHADER_SIMPLE3D);
}

//�J��
void Background::Release()
{
}
