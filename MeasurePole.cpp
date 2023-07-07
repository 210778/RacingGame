#include "Engine/Model.h"
#include "MeasurePole.h"

//�R���X�g���N�^
MeasurePole::MeasurePole(GameObject* parent)
    :GameObject(parent, "MeasurePole"), hModel_(-1)
{
}

//�f�X�g���N�^
MeasurePole::~MeasurePole()
{
}

//������
void MeasurePole::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole.fbx");
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_ = XMFLOAT3(1.5f, 1.0f, 1.5f);
}

//�X�V
void MeasurePole::Update()
{
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

    XMVECTOR startVec = XMLoadFloat3(start);
    XMVECTOR sToE = *direction - XMLoadFloat3(start);

}