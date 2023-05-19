#include "CheckPoint.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//�R���X�g���N�^
CheckPoint::CheckPoint(GameObject* parent)
    :GameObject(parent, "CheckPoint")

{
}

//�f�X�g���N�^
CheckPoint::~CheckPoint()
{
}

//������
void CheckPoint::Initialize()
{
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 40.0f);
    AddCollider(collision);
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
    //���������Ƃ��̏���
}
