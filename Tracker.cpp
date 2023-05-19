#include "Engine/Model.h"
#include "Engine/Input.h"

#include "Tracker.h"

//�R���X�g���N�^
Tracker::Tracker(GameObject* parent)
    :GameObject(parent, "Tracker"), hModel_(-1)
    , life_(3)
{
    life_ *= 60;
}

//�f�X�g���N�^
Tracker::~Tracker()
{
}

//������
void Tracker::Initialize()
{
    hModel_ = Model::Load("model\\GreenCube.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = XMFLOAT3(20.0f, 20.0f, 20.0f);
}

//�X�V
void Tracker::Update()
{
    if (life_ <= 0)
    {
        KillMe();
    }
    else
    {
        life_--;
    }
}

//�`��
void Tracker::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Tracker::Release()
{
}
