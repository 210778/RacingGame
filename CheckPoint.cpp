#include "CheckPoint.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//�R���X�g���N�^
CheckPoint::CheckPoint(GameObject* parent)
    :GameObject(parent, "CheckPoint")
    ,number_(-1)
{
}

//�f�X�g���N�^
CheckPoint::~CheckPoint()
{
}

//������
void CheckPoint::Initialize()
{
}

//�X�V
void CheckPoint::Update()
{
}

//�`��
void CheckPoint::Draw()
{
}

//�J��
void CheckPoint::Release()
{
}

//�����ɓ�������
void CheckPoint::OnCollision(GameObject* pTarget)
{
}

//�����蔻������
void CheckPoint::MakeSphereCollider(const XMFLOAT3* position, float size, int number)
{
    //const�Ȃ̂�
    transform_.position_ = *position;
    //transform_.position_.x *= -1;  //maya�Ƃ̂���̉���
    SphereCollider* collision = new SphereCollider({ 0.0f,0.0f,0.0f } , size);
    AddCollider(collision);
    number_ = number;
}