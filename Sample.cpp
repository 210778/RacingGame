#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/Camera.h"

#include "Sample.h"

//�R���X�g���N�^
Sample::Sample(GameObject* parent)
    :GameObject(parent, "Sample"), hModel_(-1)

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

    transform_.scale_ = { 3,3,3 };
}

//�X�V
void Sample::Update()
{
/*
XMFLOAT3 �� XMVECTOR

XMFLOAT3 f;	//���������Ă�Ƃ���
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 �� XMVECTOR

XMVECTOR v; 	//���������Ă�Ƃ���
XMFLOAT3 f;
XMStoreFloat3(&f, v);
*/
}

//�����ɓ�������
void Sample::OnCollision(GameObject* pTarget)
{
    //���������Ƃ��̏���
}

//�`��
void Sample::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Sample::Release()
{
}
