#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

#include "Bullet.h"
#include "Ground.h"

//�R���X�g���N�^
Bullet::Bullet(GameObject* parent)
    :GameObject(parent, "Bullet"), hModel_(-1),
    acceleration_(0.0f, 0.0f, 0.0f), life_(60)
{
    life_ *= 1;
}

//�f�X�g���N�^
Bullet::~Bullet()
{
}

//������
void Bullet::Initialize()
{
    hModel_ = Model::Load("model\\low_ball.fbx");//model\\TestBullet.fbx
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
}

//�X�V
void Bullet::Update()
{
    Accelerator();

    life_--;
    if (life_ <= 0)
    {
        KillMe();
    }
}

//�����ɓ�������
void Bullet::OnCollision(GameObject* pTarget)
{
    //���������Ƃ��̏���
}

//�`��
void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Bullet::Release()
{
}

void Bullet::Accelerator()
{
    transform_.position_.x += acceleration_.x;
    transform_.position_.y += acceleration_.y;
    transform_.position_.z += acceleration_.z;
}