#include "Engine/Model.h"
#include "MeasurePole.h"

//�R���X�g���N�^
MeasurePole::MeasurePole(GameObject* parent)
    :GameObject(parent, "MeasurePole"), hModel_(-1)
    , life_(1)
{
}

//�f�X�g���N�^
MeasurePole::~MeasurePole()
{
}

//������
void MeasurePole::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole_Z.fbx");
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(10.0f, 10.0f, 10.0f);
}

//�X�V
void MeasurePole::Update()
{
    if (life_-- <= 0)
        KillMe();
}

//�`��
void MeasurePole::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void MeasurePole::Release()
{
}

//���C�L���X�g�̉���
void MeasurePole::ViewRayCast(const XMFLOAT3* start, const XMVECTOR* direction, const float length)
{
    transform_.scale_.y = length;

    XMVECTOR sToE = XMVector3Normalize(*direction) - XMVector3Normalize((XMLoadFloat3(start)));
    XMVECTOR vecS = (XMLoadFloat3(start));
    XMVECTOR vecZ = { 1.0f,0.0f,0.0f,0.0f };
    //float toX = XMConvertToDegrees(*XMVector3AngleBetweenVectors(sToE, { 1.0f,0.0f,0.0f,0.0f }).m128_f32);
    //float toY = XMConvertToDegrees(*XMVector3AngleBetweenVectors(sToE, { 0.0f,1.0f,0.0f,0.0f }).m128_f32);
    //float toZ = XMConvertToDegrees(*XMVector3AngleBetweenVectors(sToE, { 0.0f,0.0f,1.0f,0.0f }).m128_f32);
    //transform_.rotate_.x = toX;
    //transform_.rotate_.y = toY;
    //transform_.rotate_.z = toZ;

    //transform_.rotate_.x = Calculator::AngleBetweelVector(*direction, vecZ, { 0.0f,1.0f,0.0f,0.0f });
    transform_.rotate_.y = Calculator::AngleBetweelVector(*direction, vecZ, { 0.0f,1.0f,0.0f,0.0f });
    //transform_.rotate_.z = Calculator::AngleBetweelVector(*direction, vecZ, { 0.0f,1.0f,0.0f,0.0f });
}

//�I�[�o�[���[�h
void MeasurePole::ViewRayCast(const XMFLOAT3* start, const XMFLOAT3* direction, const float length)
{
    XMVECTOR vecDir = XMLoadFloat3(direction);
    ViewRayCast(start, &vecDir, length);
}