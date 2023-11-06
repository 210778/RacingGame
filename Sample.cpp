#include "Engine/Model.h"
#include "Engine/Image.h"

#include "Sample.h"

//�R���X�g���N�^
Sample::Sample(GameObject* parent)
    :GameObject(parent, "Sample"), hModel_(-1)
    , modelScale_({ 15.0f,15.0f,15.0f })

{
}

//�f�X�g���N�^
Sample::~Sample()
{
}

//������
void Sample::Initialize()
{
    hModel_ = Model::Load("model\\Tri_Lines.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = modelScale_;
}

//�X�V
void Sample::Update()
{
}

//�����ɓ�������
void Sample::OnCollision(GameObject* pTarget)
{
    //���������Ƃ��̏���
}

//�`��
void Sample::Draw()
{
#ifdef _DEBUG
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
#endif
}

//�J��
void Sample::Release()
{
}
