#include "Engine/Model.h"

#include "VehicleWheel.h"
#include "Vehicle.h"
#include "Engine/Particle.h"

//�R���X�g���N�^
VehicleWheel::VehicleWheel(GameObject* parent)
    :GameObject(parent, "VehicleWheel"), hModel_(-1)
    , pVehicle_(nullptr)
    , rotateSpeedX_(0.0f), handleRotateY_(0.0f)
{
}

//�f�X�g���N�^
VehicleWheel::~VehicleWheel()
{
}

//������
void VehicleWheel::Initialize()
{
}

//�X�V
void VehicleWheel::Update()
{
    transform_.rotate_.x += rotateSpeedX_;
}

//�`��
void VehicleWheel::Draw()
{
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);

    //�S�̃^�C�������ꂼ��̈ʒu�A��]�ŕ\������
    Transform wheelTrans;
    for (int i = 0; i < 4; i++)
    {
        wheelTrans = transform_;

        switch (i)
        {
        default:
            wheelTrans.position_.x += Wheels.posFL_.x;
            wheelTrans.position_.y += Wheels.posFL_.y;
            wheelTrans.position_.z += Wheels.posFL_.z;

            XMFLOAT3 addPos;
            XMStoreFloat3(&addPos, Wheels.centerToLeft_);

            wheelTrans.position_.x += addPos.x;//+= Wheels.posFL_.x;
            wheelTrans.position_.y += addPos.y;
            wheelTrans.position_.z += addPos.z;

            wheelTrans.rotate_.x += rotateSpeedX_;
            wheelTrans.rotate_.y += handleRotateY_;
            //wheelTrans.rotate_.x = wheelTrans.rotate_.x * -1.0f + rotateSpeedX_;
            //wheelTrans.rotate_.y += handleRotateY_ + 180.0f;
            break;

        case 1:
            wheelTrans.position_.x += Wheels.posFR_.x;
            wheelTrans.position_.y += Wheels.posFR_.y;
            wheelTrans.position_.z += Wheels.posFR_.z;

            //wheelTrans.position_.x = Model::GetBonePosition(hModel_, "left").x;
            //wheelTrans.position_.y = Model::GetBonePosition(hModel_, "left").y;
            //wheelTrans.position_.z = Model::GetBonePosition(hModel_, "left").z;

            wheelTrans.rotate_.x += rotateSpeedX_;
            wheelTrans.rotate_.y += handleRotateY_;
            break;
        case 2:
            wheelTrans.position_.x += Wheels.posRL_.x;
            wheelTrans.position_.y += Wheels.posRL_.y;
            wheelTrans.position_.z += Wheels.posRL_.z;

            //wheelTrans.position_.x = Model::GetBonePosition(hModel_, "right").x;
            //wheelTrans.position_.y = Model::GetBonePosition(hModel_, "right").y;
            //wheelTrans.position_.z = Model::GetBonePosition(hModel_, "right").z;

            wheelTrans.rotate_.x += rotateSpeedX_;
            break;
        case 3:
            wheelTrans.position_.x += Wheels.posRR_.x;
            wheelTrans.position_.y += Wheels.posRR_.y;
            wheelTrans.position_.z += Wheels.posRR_.z;

            //wheelTrans.position_.x = Model::GetBonePosition(hModel_, "left").x;
            //wheelTrans.position_.y = Model::GetBonePosition(hModel_, "left").y;
            //wheelTrans.position_.z = Model::GetBonePosition(hModel_, "left").z;

            wheelTrans.rotate_.x += rotateSpeedX_;
            break;
        }

        Model::SetTransform(hModel_, wheelTrans);
        Model::Draw(hModel_);
    }

}

//�J��
void VehicleWheel::Release()
{
}

//�^�C���̏�񂢂낢��̃Z�b�^�[
void VehicleWheel::SetVehicleWheel(Vehicle* pVehicle, int hModel
    , XMFLOAT3 PosFL, XMFLOAT3 PosFR, XMFLOAT3 PosRL, XMFLOAT3 PosRR)
{
    pVehicle_ = pVehicle;
    hModel_ = hModel;

    Wheels.posFL_ = PosFL;
    Wheels.posFR_ = PosFR;
    Wheels.posRL_ = PosRL;
    Wheels.posRR_ = PosRR;

    //���ꂼ��̃^�C���܂ł̃x�N�g��
    XMFLOAT3 centerPos = Model::GetBonePosition(hModel_, "center");
    XMVECTOR centerVec = XMLoadFloat3(&centerPos);
    Wheels.cToFL_ = XMLoadFloat3(&Wheels.posFL_) - centerVec;
    Wheels.cToFR_ = XMLoadFloat3(&Wheels.posFR_) - centerVec;
    Wheels.cToRL_ = XMLoadFloat3(&Wheels.posRL_) - centerVec;
    Wheels.cToRR_ = XMLoadFloat3(&Wheels.posRR_) - centerVec;

    //�{�[���Ԃ̃x�N�g��
    XMFLOAT3 bottomPos = Model::GetBonePosition(hModel_, "bottom");
    Wheels.centerToBottom_ = XMLoadFloat3(&bottomPos) - centerVec;
    XMFLOAT3 leftPos = Model::GetBonePosition(hModel_, "left");
    Wheels.centerToLeft_ = XMLoadFloat3(&leftPos) - centerVec;
    XMFLOAT3 rightPos = Model::GetBonePosition(hModel_, "right");
    Wheels.centerToRight_ = XMLoadFloat3(&rightPos) - centerVec;

    //�^�C���̍������v�Z���Ďԗ��ɋ�����
    calculateHeight();
}

void VehicleWheel::calculateHeight()
{
    XMFLOAT3 wheelBottom = Model::GetBonePosition(hModel_, "bottom");
    Wheels.wheelHeight_ = *XMVector3Length(XMLoadFloat3(&wheelBottom)).m128_f32;

    //�ꉞ�g��ɑΉ�
    pVehicle_->SetWheelHeight(Wheels.wheelHeight_ * transform_.scale_.y);
}