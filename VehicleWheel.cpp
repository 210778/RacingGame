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
    XMFLOAT3 addPos;
    XMFLOAT3 wheelPos;
    float addRotateY;

    for (int i = 0; i < 4; i++)
    {
        wheelTrans = transform_;
        addRotateY = 0;

        switch (i)
        {
        default:
            XMStoreFloat3(&addPos, Wheels.centerToLeft_);
            wheelPos = Wheels.posFL_;
            addRotateY = handleRotateY_;
            break;

        case 1:
            XMStoreFloat3(&addPos, Wheels.centerToRight_);
            wheelPos = Wheels.posFR_;
            addRotateY = handleRotateY_;
            break;
        case 2:
            XMStoreFloat3(&addPos, Wheels.centerToLeft_);
            wheelPos = Wheels.posRL_;
            break;
        case 3:
            XMStoreFloat3(&addPos, Wheels.centerToRight_);
            wheelPos = Wheels.posRR_;
            break;
        }

        wheelTrans.position_.x += addPos.x + wheelPos.x;
        wheelTrans.position_.y += addPos.y + wheelPos.y;
        wheelTrans.position_.z += addPos.z + wheelPos.z;

        wheelTrans.rotate_.x += rotateSpeedX_;
        wheelTrans.rotate_.y += addRotateY;

        //���͍��E�Ώ̂����ǑO��Ώۂɂ������Ȃ�
        //wheelTrans.rotate_.x = wheelTrans.rotate_.x * -1.0f + rotateSpeedX_;
        //wheelTrans.rotate_.y += addRotateY + 180.0f;

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