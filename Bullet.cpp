#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

#include "Bullet.h"
#include "Ground.h"

//コンストラクタ
Bullet::Bullet(GameObject* parent)
    :GameObject(parent, "Bullet"), hModel_(-1),
    acceleration_(0.0f, 0.0f, 0.0f), life_(60)
{
    life_ *= 1;
}

//デストラクタ
Bullet::~Bullet()
{
}

//初期化
void Bullet::Initialize()
{
    hModel_ = Model::Load("model\\low_ball.fbx");//model\\TestBullet.fbx
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
}

//更新
void Bullet::Update()
{
    Accelerator();

    life_--;
    if (life_ <= 0)
    {
        KillMe();
    }
}

//何かに当たった
void Bullet::OnCollision(GameObject* pTarget)
{
    //当たったときの処理
}

//描画
void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Bullet::Release()
{
}

void Bullet::Accelerator()
{
    transform_.position_.x += acceleration_.x;
    transform_.position_.y += acceleration_.y;
    transform_.position_.z += acceleration_.z;
}