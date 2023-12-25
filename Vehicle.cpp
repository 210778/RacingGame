#include <directxcollision.h>
#include <directxmath.h>
#include <directxpackedvector.h>

#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/Camera.h"
#include "Engine/Debug.h"
#include "Engine/Particle.h"
#include "Engine/Global.h"

#include "Vehicle.h"
#include "Viewer.h"
#include "Speedometer.h"
#include "CheckPoint.h"
#include "VehicleWheel.h"
#include "ParticlePackage.h"
#include "Circuit.h"

using std::vector;
using std::string;

//�R���X�g���N�^
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle")
    //���f���ԍ�
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    //�x�N�g��
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    //�X�s�[�h�Ȃ�
    , moveSPD_(0.01f), rotateSPD_(1.1f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f)

    //�n���h���֌W
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    //���C
    , wheelFriction_(0.99f), airFriction_(0.999f), turfFriction_(0.98f)
    , iceFriction_(0.999f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f), sideIceFriction_(0.02f)
    , landingFriction_(1.0f), sideFriction_(1.0f)

    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.5f)
    , handleFlag_(false), handleRotateMax_(60.0f)

    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), goalRanking_(0), population_(1), lapCountFlag_(false), goalTime_(0), standbyTime_(0)
    , mass_(1.0f)
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f), wheelParticleLengthMax_(0.5f)
    , sparkParticleLength_(0.95), sparkParticleHanlde_(16), npcParticleRandom_(67)
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_(), restartTransform_()

    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.99f)
    , handleRight_(1), handleLeft_(-1)
    //�u�[�X�g
    , boostCapacityMax_(200.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.25f), boostValue_(1.5f)
    , isPlayer_(false), toPlayerVehicleLength_(0.0f), particleLimitLength_(150.0f)
    , collideBoxValue_(0.5f), isOperationInvalid_(false)
    , pauseFlag_(false)
    , farLimitPosition_({ 1000.f, 500.0f, 1000.f })
    , pViewer_(nullptr)
{
    matRotateX = XMMatrixIdentity();
    matRotateY = XMMatrixIdentity();
    matRotateZ = XMMatrixIdentity();
    matRotateX_R = XMMatrixIdentity();
    matRotateY_R = XMMatrixIdentity();
    matRotateZ_R = XMMatrixIdentity();

    worldVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });
    vehicleVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });

    //�n�ʂ̃^�C�v
    //�ʏ퓹�H
    GroundTypeFriction_[Circuit::circuitType::road].acceleration = 1.0f;
    GroundTypeFriction_[Circuit::circuitType::road].landing = 0.99f;
    GroundTypeFriction_[Circuit::circuitType::road].side = 0.2f;
    //���n
    GroundTypeFriction_[Circuit::circuitType::turf].acceleration = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].landing = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].side = 0.2f;
    //���n�@���Ɠ���
    GroundTypeFriction_[Circuit::circuitType::dirt].acceleration = GroundTypeFriction_[Circuit::circuitType::turf].acceleration;
    GroundTypeFriction_[Circuit::circuitType::dirt].landing = GroundTypeFriction_[Circuit::circuitType::turf].landing;
    GroundTypeFriction_[Circuit::circuitType::dirt].side = GroundTypeFriction_[Circuit::circuitType::turf].side;
    //�X��
    GroundTypeFriction_[Circuit::circuitType::ice].acceleration = 0.2f;
    GroundTypeFriction_[Circuit::circuitType::ice].landing = 0.999f;
    GroundTypeFriction_[Circuit::circuitType::ice].side = 0.02f;
    //������
    GroundTypeFriction_[Circuit::circuitType::boost].acceleration = 8.0f;
    GroundTypeFriction_[Circuit::circuitType::boost].landing = 0.999f;
    GroundTypeFriction_[Circuit::circuitType::boost].side = 0.1f;
    //�ޗ�
    GroundTypeFriction_[Circuit::circuitType::abyss]; //��邾��
}

//�p���p
Vehicle::Vehicle(GameObject* parent, const std::string& name)
    :GameObject(parent, name)
    //���f���ԍ�
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    //�x�N�g��
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    //�X�s�[�h�Ȃ�
    , moveSPD_(0.01f), rotateSPD_(1.1f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f)

    //�n���h���֌W
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    //���C
    , wheelFriction_(0.99f), airFriction_(0.999f), turfFriction_(0.98f)
    , iceFriction_(0.999f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f), sideIceFriction_(0.02f)
    , landingFriction_(1.0f), sideFriction_(1.0f)

    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.5f)
    , handleFlag_(false), handleRotateMax_(60.0f)

    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), goalRanking_(0), population_(1), lapCountFlag_(false), goalTime_(0), standbyTime_(0)
    , mass_(1.0f)
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f), wheelParticleLengthMax_(0.5f)
    , sparkParticleLength_(0.95), sparkParticleHanlde_(16), npcParticleRandom_(67)
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_(), restartTransform_()

    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.99f)
    , handleRight_(1), handleLeft_(-1)
    //�u�[�X�g
    , boostCapacityMax_(200.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.25f), boostValue_(1.5f)
    , isPlayer_(false), toPlayerVehicleLength_(0.0f), particleLimitLength_(150.0f)
    , collideBoxValue_(0.5f), isOperationInvalid_(false)
    , pauseFlag_(false)
    , farLimitPosition_({ 1000.f, 500.0f, 1000.f })
    , pViewer_(nullptr)
{
    matRotateX = XMMatrixIdentity();
    matRotateY = XMMatrixIdentity();
    matRotateZ = XMMatrixIdentity();
    matRotateX_R = XMMatrixIdentity();
    matRotateY_R = XMMatrixIdentity();
    matRotateZ_R = XMMatrixIdentity();

    worldVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });
    vehicleVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });

    //�n�ʂ̃^�C�v
    //�ʏ퓹�H
    GroundTypeFriction_[Circuit::circuitType::road].acceleration = 1.0f;
    GroundTypeFriction_[Circuit::circuitType::road].landing = 0.99f;
    GroundTypeFriction_[Circuit::circuitType::road].side = 0.2f;
    //���n
    GroundTypeFriction_[Circuit::circuitType::turf].acceleration = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].landing = 0.98f;
    GroundTypeFriction_[Circuit::circuitType::turf].side = 0.2f;
    //���n�@���Ɠ���
    GroundTypeFriction_[Circuit::circuitType::dirt].acceleration = GroundTypeFriction_[Circuit::circuitType::turf].acceleration;
    GroundTypeFriction_[Circuit::circuitType::dirt].landing = GroundTypeFriction_[Circuit::circuitType::turf].landing;
    GroundTypeFriction_[Circuit::circuitType::dirt].side = GroundTypeFriction_[Circuit::circuitType::turf].side;
    //�X��
    GroundTypeFriction_[Circuit::circuitType::ice].acceleration = 0.2f;
    GroundTypeFriction_[Circuit::circuitType::ice].landing = 0.999f;
    GroundTypeFriction_[Circuit::circuitType::ice].side = 0.02f;
    //������
    GroundTypeFriction_[Circuit::circuitType::boost].acceleration = 8.0f;
    GroundTypeFriction_[Circuit::circuitType::boost].landing = 0.999f;
    GroundTypeFriction_[Circuit::circuitType::boost].side = 0.1f;
    //�ޗ�
    GroundTypeFriction_[Circuit::circuitType::abyss]; //��邾��
}

//�f�X�g���N�^
Vehicle::~Vehicle()
{
}


//������
void Vehicle::Initialize()
{
    hModel_ = Model::Load(vehicleModelName_);
    assert(hModel_ >= 0);

    //�T�C�Y�v�Z
    SetVehicleSize(hModel_);

    //�v���C���[�̃J����
    PlayerCamera_Initialize();

    //�^�C��
    hWheelModel_ = Model::Load(wheelModelName_);
    assert(hWheelModel_ >= 0);
    MakeWheels(hWheelModel_);

    //�����蔻��
    SphereCollider* collision =
        new SphereCollider(Model::GetBonePosition(hModel_, "center"), Size.centerFrontToRear_);
    AddCollider(collision);

    //�G�t�F�N�g�̃|�C���^�Ƃ���̂܂Ƃ�
    pParticle_ = Instantiate<Particle>(this);

    //�ăX�^�[�g�n�_
    restartTransform_ = startTransform_;

    //UI������
    PlayerUI_Initialize();
}

//�X�V
void Vehicle::Update()
{
    //�|�[�Y
    if (pauseFlag_)
        return;

    //�s���p��
    //���ꂼ��̒l�ɍ��킹�Ď���]������s��
    matRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    matRotateZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));
    //�t��]�s��
    matRotateX_R = XMMatrixInverse(nullptr, matRotateX);
    matRotateY_R = XMMatrixInverse(nullptr, matRotateY);
    matRotateZ_R = XMMatrixInverse(nullptr, matRotateZ);

    //�⓹�ɑΉ�
    //���Ԃ�ZXY
    vehicleVector_.x = XMVector3TransformCoord(worldVector_.x, matRotateZ);
    vehicleVector_.x = XMVector3TransformCoord(vehicleVector_.x, matRotateY);

    vehicleVector_.y = XMVector3TransformCoord(worldVector_.y, matRotateZ);
    vehicleVector_.y = XMVector3TransformCoord(vehicleVector_.y, matRotateX);

    vehicleVector_.z = XMVector3TransformCoord(worldVector_.z, matRotateX);
    vehicleVector_.z = XMVector3TransformCoord(vehicleVector_.z, matRotateY);

    XMVECTOR vecX = moveSPD_ * vehicleVector_.x;
    XMVECTOR vecY = moveSPD_ * vehicleVector_.y;
    XMVECTOR vecZ = moveSPD_ * vehicleVector_.z;

    //�S�[������
    GoalJudgement();


    //�O�����ɐi��ł邩������������
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
        accZDirection_ = -1;

    //����
    InputReceive(vecX, vecZ);


    //�n�ʂ̎�ނɂ���Ė��C�Ȃ�
    LandingProcess();


    //�n���h����]
    HandleRotate();

    //�^�C���̊p�x�ƌ���
    //���ʂ�^�C���̕����ւ͌������Ȃ����A�^�C���̕����ƕ��s�̕����ւ͌�������
    //�^�C���̕����ƕ��s�Ȃ牽�����Ȃ����A�����ɋ߂��قǃ^�C���̕����Ƀx�N�g���𔭐�������
    TurnWheel();

    //�ʒu�@�{�@�x�N�g��
    XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));

    //�ڒn�A�ǏՓ�
    VehicleCollide();

    //�^�C���̒l�Z�b�g
    pWheels_->SetWheelSpeedRotate(*XMVector3LengthEst(acceleration_).m128_f32
        * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //�J�����X�V
    PlayerCamera_Update();

    //�X�s�[�h���[�^�[
    PlayerUI_Update();

    //�G�t�F�N�g
    VehicleParticle();

    //�p�x���K��
    transform_.rotate_.x = Calculator::AngleNormalize(transform_.rotate_.x);
    transform_.rotate_.y = Calculator::AngleNormalize(transform_.rotate_.y);
    transform_.rotate_.z = Calculator::AngleNormalize(transform_.rotate_.z);
}

//�����ɓ�������
void Vehicle::OnCollision(GameObject* pTarget)
{
    //�|�[�Y
    if (pauseFlag_)
        return;

    //���������Ƃ��̏���
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() == pointCount_)
            pointCount_++;

        if (pCP->GetNumber() <= 0 && pointCount_ >= pointCountMax_)
        {
            pointCount_ = 0;
            LapCountAdd(1);
        }
    }
}

//�`��
void Vehicle::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    PlayerUI_Draw();
}

//�J��
void Vehicle::Release()
{
}

void Vehicle::SpeedLimit(XMVECTOR& speed, const float limit)
{
    //�����𒲂ׂ�
    float* pLength = XMVector3LengthEst(speed).m128_f32;
    if (*pLength > limit)
        *pLength = limit;

    speed = XMVector3Normalize(speed);
    speed *= *pLength;
}

void Vehicle::SpeedLimit(XMFLOAT3& speed, const float limit)
{
    XMVECTOR speedVec = XMLoadFloat3(&speed);
    SpeedLimit(speedVec, limit);
    XMStoreFloat3(&speed, speedVec);
}

void Vehicle::AngleLimit(float& angle, const float limit)
{
    //�}�C�i�X�����ׂ�
    bool minusFlag = false;
    if (angle < 0)
        minusFlag = true;

    //�p�x�̐�Βl���ő�p�x�̐�Βl�𒴂��Ă�����ő�p�x�ɂ���
    //�y�ʉ��ł��邩
    if (fabs(angle) > fabs(limit))
    {
        angle = fabs(limit);
        //�}�C�i�X��������}�C�i�X�ɖ߂�
        if (minusFlag == true)
            angle *= -1;
    }
}

//�^�C����]
void Vehicle::TurnWheel()
{
    //�n�ʂɂ���Ȃ���s
    if (!landingFlag_)
        return;
    //�����x�𐳋K��
    //������y���������+�A�E���Ɖ�����-
    //��]���ĉ����x�ɑ���
    acceleration_ += *XMVector3LengthEst(acceleration_).m128_f32
        * XMVector3TransformCoord(XMVector3Cross(vehicleVector_.z
            , XMVector3Normalize(acceleration_))
            , XMMatrixRotationNormal(vehicleVector_.z
                , XMConvertToRadians(transform_.rotate_.z) + XM_PIDIV2))
        * GroundTypeFriction_[landingType_].side;

    //�ǂ݂₷���R�[�h
#if 0
    //�����x�𐳋K��
    XMVECTOR normalAcc = XMVector3Normalize(acceleration_);

    //������y���������+�A�E���Ɖ�����-
    XMVECTOR outerProduct = XMVector3Cross(vehicleVector_.z, normalAcc);

    //��]���ė��p����
    XMMATRIX MatRotateZ = XMMatrixRotationNormal(vehicleVector_.z, XM_PIDIV2);
    outerProduct = XMVector3TransformCoord(outerProduct, MatRotateZ);

    acceleration_ += (*XMVector3LengthEst(acceleration_).m128_f32)
        * outerProduct * GroundTypeFriction_[landingType_].side;
#endif
}

//�n�ʁA�ǁA�G�Ƃ̏Փ˂��܂Ƃ߂�H�H
void Vehicle::VehicleCollide()
{
    bool isLanding = false;

    //��ނ̕�����
    for (auto& itr : Circuit::GetChosenCircuit()->parts_)
    {
        landingFlag_ = false;   //�ڒn�t���O���Z�b�g

        //�n��
        Landing(itr.model_, itr.type_);

        if (landingFlag_)
            isLanding = true;

        landingFlag_ = isLanding;   //��x�ł�true�Ȃ�ڒn���Ă邱�Ƃɂ���

        //��
        CollideWall(itr.model_, itr.type_);
    }

    //����
    if (!landingFlag_)
    {
        acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
    }
}

//�ڒn 
void Vehicle::Landing(int hModel, int type)
{
    //���C�̔��ˈʒu
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    RayCastData data(Transform::Float3Add(transform_.position_, upF), XMFLOAT3{ 0.0f, -1.0f, 0.0f });

    Model::RayCast(hModel, &data);      //���C�𔭎�

    //���C������������
    if (data.hit)
    {
        //NPC�p
        SetRayCastHit(RayCastHit::Number::down, data);

        if (-data.dist > XMVectorGetY(acceleration_) - Size.toWheelBottom_)
        {

            //�������̉����x���傫���Ȃ�@�n�ʂɃ��[�v�@�������x���O
            if (!landingFlag_)
            {
                transform_.position_.y -= data.dist - Size.toWheelBottom_;
                acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            }

            landingFlag_ = true;
            landingType_ = type;    //�n�ʂ̃^�C�v
        }
        else
        {
            landingFlag_ = false;
        }

        //�p�x��ς���
        if (landingFlag_ || data.dist < Size.toWheelBottom_ + Size.wheelHeight_)
        {
            landingFlag_ = true;
            landingType_ = type;

            //��]
            if (VehicleRotateSlope(data.normal, slopeLimitAngle_, RayCastHit::Number::down) == false)
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};  //�⓹�Ȃ痎��
        }
    }

    //�ڒn�ʒu����
    //���ƍ⓹���ړ����Ă�Ƃ��ɍ⓹�ɒ���t���@�}������ƌ��ʂȂ�
    RayCastData vehicleData;
    vehicleData.start = data.start;
    XMStoreFloat3(&vehicleData.dir, -vehicleVector_.y);
    Model::RayCast(hModel, &vehicleData);      //���C�𔭎�
    //���፷���^�C���̒��a���炢�̎�
    if (vehicleData.hit && vehicleData.dist > Size.toWheelBottom_
        && vehicleData.dist < Size.toWheelBottom_ + Size.wheelHeight_)
    {
        XMFLOAT3 wheelFlo;
        XMStoreFloat3(&wheelFlo, -vehicleVector_.y * (vehicleData.dist - Size.toWheelBottom_));
        transform_.position_ = Transform::Float3Add(transform_.position_, wheelFlo);

        //�����̎���Y���𖳂���
        float plus = XMVectorGetY(acceleration_);
        XMVECTOR plusVec = { 0.0f, -plus, 0.0f, 0.0f };
        VectorRotateMatrixZXY(plusVec);
        acceleration_ += plusVec;

        landingFlag_ = true;
        landingType_ = type;    //�n�ʂ̃^�C�v
    }

    //�V�� �ق�Ƃ͕�������ׂ����낤���ǓV��ɓ����邱�Ƃ͒��������d���������炱���ɒu��
    XMStoreFloat3(&data.dir, vehicleVector_.y);
    Model::RayCast(hModel, &data);  //���C�𔭎�
    //��������
    if (data.hit)
    {
        //NPC�p
        SetRayCastHit(RayCastHit::Number::up, data);

        //������ƒn�ʂɖ��܂����Ƃ�
        if (data.dist < Size.toTop_)
        {
            //���̕��ʒu���グ��
            XMFLOAT3 upVec;
            XMStoreFloat3(&upVec, data.normal * -(data.dist + Size.toWheelBottom_));
            transform_.position_ = Transform::Float3Add(transform_.position_, upVec);

            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
        //�V��ɂԂ������Ƃ�
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};  //�~�߂�
        }
    }
}

void Vehicle::CollideWall(int hModel, int type)
{
    //�O�㍶�E�Ǝ΂߂ŕ������邱�Ƃɂ���
    std::array<RayCastData, 4>wallCollideVertical;

    //���C�̔��ˈʒu(�{�[���擾�������Ƃ��܂������Ȃ�����)
    XMFLOAT3 startPos = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    startPos = Transform::Float3Add(startPos, upF);

    //�O�㍶�E��
    for (int i = 0; i < wallCollideVertical.size(); i++)
    {
        wallCollideVertical[i].start = startPos;

        //90�x����]�s��
        XMMATRIX matRot = XMMatrixRotationY(XM_PIDIV2 * i);	//�x���ŉ�]������s��   
        //��]
        XMVECTOR dirVec = XMVector3TransformCoord(worldVector_.z, matRot);
        VectorRotateMatrixZXY(dirVec);

        XMStoreFloat3(&wallCollideVertical[i].dir, dirVec);

        Model::RayCast(hModel, &wallCollideVertical[i]);  //���C�𔭎�

        //����������
        if (wallCollideVertical[i].hit)
        {
            //NPC�p
            int NPC_Number = 0;

            //�������Ƃ̉����x
            float dirAcc;
            //�������Ƃ̎ԑ̂̒���
            float dirSize;

            //�����x����]
            XMVECTOR accRotVec = acceleration_;
            VectorRotateMatrixZXY(accRotVec);

            //�������Ƃɑ���@�Ȃ񂩓���邢���Ƃ��Ă����ȋC������
            switch (i)
            {
            default://�O���A���̑�
            case Direction::front:
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toFront_;
                NPC_Number = RayCastHit::Number::front;
                break;
            case Direction::right://�E
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toRight_;
                NPC_Number = RayCastHit::Number::right;
                break;
            case Direction::rear://���
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toRear_;
                NPC_Number = RayCastHit::Number::rear;
                break;
            case Direction::left://��
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toLeft_;
                NPC_Number = RayCastHit::Number::left;
                break;
            }

            //NPC�p
            SetRayCastHit(NPC_Number, wallCollideVertical[i]);

            if (wallCollideVertical[i].dist < abs(dirAcc) + dirSize)
            {
                //�U == �ǂɏՓ� / �^ == �⓹
                //��]
                if (VehicleRotateSlope(wallCollideVertical[i].normal, slopeLimitAngle_, NPC_Number) == false)
                {
                    //�Փ˂��钼�O�Ŏ~�܂������̕ǂ܂ł̋���
                    XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - dirSize, 0.0f };
                    ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                    //��]
                    VectorRotateMatrixZXY(ajustVec);

                    //�ړ�
                    transform_.position_.x += XMVectorGetX(ajustVec);
                    transform_.position_.y += XMVectorGetY(ajustVec);
                    transform_.position_.z += XMVectorGetZ(ajustVec);

                    switch (i) {
                    default://�O���A���̑�
                    case Direction::front:
                        accRotVec *= {1.0f, 1.0f, 0.0f, 1.0f}; break;
                    case Direction::right://�E
                        accRotVec *= {0.0f, 1.0f, 1.0f, 1.0f}; break;
                    case Direction::rear://���
                        accRotVec *= {1.0f, 1.0f, 0.0f, 1.0f}; break;
                    case Direction::left://��
                        accRotVec *= {0.0f, 1.0f, 1.0f, 1.0f}; break;
                    }
                    //�����x��߂�
                    acceleration_ = accRotVec;
                    VectorRotateMatrixZXY_R(acceleration_);
                    //����
                    acceleration_ *= wallReflectionForce_;
                    //�ǔ���
                    acceleration_ -= XMLoadFloat3(&wallCollideVertical[i].dir) * moveSPD_;
                }
            }
        }
    }

    //�΂�
    std::array<RayCastData, 4>wallCollideOblique;
    for (int i = 0; i < wallCollideOblique.size(); i++)
    {
        wallCollideOblique[i].start = startPos;

        //z������̊p�x �ƃT�C�Y
        float theta = 0.0f;
        float vehicleLen = 0.0f;

        //�����x��]
        XMVECTOR accRotVec = acceleration_;
        VectorRotateMatrixZXY(accRotVec);

        float dirAccX = XMVectorGetX(accRotVec),
            dirAccZ = XMVectorGetZ(accRotVec);

        int NPC_Number = 0; //NPC�p

        switch (i) {
        default:
        case Oblique::frontLeft://�O��
            theta = Size.angleFrontLeft_;
            vehicleLen = Size.toFrontLeft_;
            NPC_Number = RayCastHit::Number::frontLeft;
            break;
        case Oblique::frontRight://�O�E
            theta = Size.angleFrontRight_;
            vehicleLen = Size.toFrontRight_;
            NPC_Number = RayCastHit::Number::frontRight;
            break;
        case Oblique::rearLeft://��덶
            theta = Size.angleRearLeft_;
            vehicleLen = Size.toRearLeft_;
            NPC_Number = RayCastHit::Number::rearLeft;
            break;
        case Oblique::rearRight://���E
            theta = Size.angleRearRight_;
            vehicleLen = Size.toRearRight_;
            NPC_Number = RayCastHit::Number::rearRight;
            break;
        }

        //��]�s��
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(theta));	//�x���ŉ�]������s��   
        //��]
        XMVECTOR dirVec = XMVector3TransformCoord(worldVector_.z, matRot);
        VectorRotateMatrixZXY(dirVec);

        XMStoreFloat3(&wallCollideOblique[i].dir, dirVec);

        Model::RayCast(hModel, &wallCollideOblique[i]);  //���C�𔭎�

        //����������
        if (wallCollideOblique[i].hit)
        {
            //NPC�p
            SetRayCastHit(NPC_Number, wallCollideOblique[i]);

            //�����Ɖ����x�̔���
            if (wallCollideOblique[i].dist * wallCollideOblique[i].dist <
                (dirAccX * dirAccX + dirAccZ * dirAccZ) + (vehicleLen * vehicleLen))
            {
                if (VehicleRotateSlope(wallCollideOblique[i].normal, slopeLimitAngle_, NPC_Number) == false)
                {
                    //�Փ˂��鎞�̕ǂ܂ł̋���
                    XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideOblique[i].dist - vehicleLen, 0.0f };
                    ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                    //��]
                    VectorRotateMatrixZXY(ajustVec);
                    //�ǂɂ�����
                    transform_.position_.x += XMVectorGetX(ajustVec);
                    transform_.position_.y += XMVectorGetY(ajustVec);
                    transform_.position_.z += XMVectorGetZ(ajustVec);

                    //����������
                    acceleration_ *= wallReflectionForce_;

                    //�ǔ���
                    acceleration_ -= XMLoadFloat3(&wallCollideOblique[i].dir) * moveSPD_;
                }
            }
        }
    }
}

void Vehicle::MakeWheels(int hModel)
{
    //�^�C��
    pWheels_ = Instantiate<VehicleWheel>(this);
    pWheels_->SetVehicleWheel(this, hModel, Size.wheelFL_, Size.wheelFR_, Size.wheelRL_, Size.wheelRR_);
}

void Vehicle::SetVehicleSize(int hModel)
{
    //�{�[��
    XMFLOAT3 vehicleCenter;
    if (!(Model::GetBonePosition(&vehicleCenter, hModel, "center"))) {
        Debug::Log("failed get bone:center", true);
    }
    XMFLOAT3 vehicleBottom;
    if (!(Model::GetBonePosition(&vehicleBottom, hModel, "bottom"))) {
        Debug::Log("failed get bone:bottom", true);
    }
    XMFLOAT3 vehicleTop;
    if (!(Model::GetBonePosition(&vehicleTop, hModel, "top"))) {
        Debug::Log("failed get bone:top", true);
    }
    XMFLOAT3 vehicleLeft;
    if (!(Model::GetBonePosition(&vehicleLeft, hModel, "left"))) {
        Debug::Log("failed get bone:left", true);
    }
    XMFLOAT3 vehicleRight;
    if (!(Model::GetBonePosition(&vehicleRight, hModel, "right"))) {
        Debug::Log("failed get bone:right", true);
    }
    XMFLOAT3 vehicleRear;
    if (!(Model::GetBonePosition(&vehicleRear, hModel, "rear"))) {
        Debug::Log("failed get bone:rear", true);
    }
    XMFLOAT3 vehicleFront;
    if (!(Model::GetBonePosition(&vehicleFront, hModel, "front"))) {
        Debug::Log("failed get bone:front", true);
    }

    if (!(Model::GetBonePosition(&Size.wheelFR_, hModel, "wheelFR"))) {
        Debug::Log("failed get bone:wheelFR", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelFL_, hModel, "wheelFL"))) {
        Debug::Log("failed get bone:wheelFL", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelRR_, hModel, "wheelRR"))) {
        Debug::Log("failed get bone:wheelRR", true);
    }
    if (!(Model::GetBonePosition(&Size.wheelRL_, hModel, "wheelRL"))) {
        Debug::Log("failed get bone:wheelRL", true);
    }

    //�ԗ��̑傫���v�Z
    Size.toRight_ = *XMVector3Length(XMLoadFloat3(&vehicleRight) - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toLeft_ = *XMVector3Length(XMLoadFloat3(&vehicleRight) - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toFront_ = *XMVector3Length(XMLoadFloat3(&vehicleFront) - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toRear_ = *XMVector3Length(XMLoadFloat3(&vehicleRear) - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toTop_ = *XMVector3Length(XMLoadFloat3(&vehicleTop) - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toBottom_ = *XMVector3Length(XMLoadFloat3(&vehicleBottom) - XMLoadFloat3(&vehicleCenter)).m128_f32;

    Size.rightToLeft_ = Size.toRight_ + Size.toLeft_;
    Size.topToBottom_ = Size.toBottom_ + Size.toTop_;
    Size.frontToRear_ = Size.toFront_ + Size.toRear_;
    Size.toFrontRight_ = sqrt((Size.toFront_ * Size.toFront_) + (Size.toRight_ * Size.toRight_));
    Size.toFrontLeft_ = sqrt((Size.toFront_ * Size.toFront_) + (Size.toLeft_ * Size.toLeft_));
    Size.toRearRight_ = sqrt((Size.toRear_ * Size.toRear_) + (Size.toRight_ * Size.toRight_));
    Size.toRearLeft_ = sqrt((Size.toRear_ * Size.toRear_) + (Size.toLeft_ * Size.toLeft_));

    Size.centerRightToLeft_ = Size.rightToLeft_ * 0.5f;
    //Size.centerTopToBottom_ = (Size.topToBottom_ + Size.wheelRemainder_) * 0.5f;
    Size.centerFrontToRear_ = Size.frontToRear_ * 0.5f;

    //�p�x
    Size.angleFrontLeft_ = XMConvertToDegrees(acos(Size.toFront_ / Size.toFrontLeft_));
    Size.angleFrontRight_ = XMConvertToDegrees(XM_PI - acos(Size.toFront_ / Size.toFrontRight_));
    Size.angleRearLeft_ = XMConvertToDegrees(acos(Size.toRear_ / Size.toFrontLeft_) + XM_PI);
    Size.angleRearRight_ = XMConvertToDegrees(XM_2PI - acos(Size.toRear_ / Size.toFrontRight_));
}

//�n���h���̑���
void Vehicle::HandleTurnLR(int LR)
{
    //��₱�������n���h����]�̂��߂ɂ������Ă���
    if ((LR < 0 && handleRotate_ < 0) || (LR > 0 && handleRotate_ > 0))
        handleFlag_ = true;

    //�����Ă�ƋȂ���₷��
    if (slideFlag_)
        handleRotate_ += rotateSPD_ * slideHandleRotateAdd_ * LR;
    else
        handleRotate_ += rotateSPD_ * LR;
}

//���n���Ă�Ƃ��ɖ��C�Ȃǂ̏����A�ޗ���͈͊O���ƃ��Z�b�g
void Vehicle::LandingProcess()
{
    if (landingFlag_)
    {
        acceleration_ *= { GroundTypeFriction_[landingType_].landing, 1.0f
            , GroundTypeFriction_[landingType_].landing, 1.0f };

        //�����n�_�X�V
        if (landingType_ == Circuit::circuitType::road)
        {
            restartTransform_.position_ = transform_.position_;
            restartTransform_.rotate_ = transform_.rotate_;
        }

        if (landingType_ == Circuit::circuitType::abyss)
        {
            //�ޗ��ɗ���
            FallAbyss();
        }
    }
    //���ꂷ�����狭�����Z�b�g
    if (abs(transform_.position_.x) > abs(farLimitPosition_.x) ||
        abs(transform_.position_.y) > abs(farLimitPosition_.y) ||
        abs(transform_.position_.z) > abs(farLimitPosition_.z))
    {
        FallAbyss();
    }
}

void Vehicle::HandleRotate()
{
    //�n��Ȃ�n���h���ɍ��킹�ĉ�]
    if (landingFlag_)
    {
        float accLength = *XMVector3LengthEst(acceleration_).m128_f32;

        //�O��ɓ����Ă���Ȃ�
        transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_ * accZDirection_;
        handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
    }
    //�n���h����߂�
    if (!handleFlag_)
    {
        handleRotate_ *= handleAdjust_;
    }
}

//���̃`�F�b�N�|�C���g�̈ʒu���擾
XMFLOAT3* Vehicle::GetNextCheckPosition()
{
    return Circuit::GetNextCheckPointPosition(pointCount_);
}
//���̃`�F�b�N�|�C���g�܂ł̋������擾
float Vehicle::GetNextCheckDistance()
{
    return *XMVector3LengthEst(XMLoadFloat3(GetNextCheckPosition()) - XMLoadFloat3(&transform_.position_)).m128_f32;
}

//�x�N�g������]�s��ŉ�] (ZXY��)
void Vehicle::VectorRotateMatrixZXY(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateZ);
    vec = XMVector3TransformCoord(vec, matRotateX);
    vec = XMVector3TransformCoord(vec, matRotateY);
}

//�x�N�g������]�s���"�t"��] (YXZ��)
void Vehicle::VectorRotateMatrixZXY_R(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateY_R);
    vec = XMVector3TransformCoord(vec, matRotateX_R);
    vec = XMVector3TransformCoord(vec, matRotateZ_R);
}

//�S�[������
void Vehicle::GoalJudgement()
{
    if (lapCount_ >= lapMax_)
    {
        //�S�[�����ʂ��X�V����ĂȂ��Ȃ�Z�b�g����
        if (!goalFlag_)
        {
            goalRanking_ = ranking_;
            goalTime_ = time_;
        }

        goalFlag_ = true;
    }
}

//���񐔑���
void Vehicle::LapCountAdd(int value)
{
    lapCount_ += value;

    if (lapCount_ > lapMax_)
    {
        lapCount_ = lapMax_;
    }

    //������炷���߂̃t���O
    if (!goalFlag_)
    {
        lapCountFlag_ = true;
    }
}

// �⓹�ɉ����Ďԗ�����](X�AZ��)
bool Vehicle::VehicleRotateSlope(const XMVECTOR& normal, float limitAngle, int rayCastType)
{
    //�@����Y���𔽓]
    XMVECTOR normalR = normal * XMVECTOR{ 1.0f,-1.0f,1.0f,1.0f };

    //�p�x���w��p�x���}���Ƃ�߂�
    //�p�x�v�Z�͓�
    if (XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32) > limitAngle)
    {
        SetRayCastSlope(rayCastType, false);
        return false;
    }

    //��]
    XMVECTOR normalVec = XMVector3TransformCoord(normalR, matRotateY_R);

    XMFLOAT3 rotate;
    rotate.x = XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.z, normalVec).m128_f32) - 90.0f;

    rotate.z = -(XMConvertToDegrees(*XMVector3AngleBetweenNormals(
        worldVector_.x, normalVec).m128_f32) - 90.0f);

    //�p�x����
    if (abs(rotate.x) <= limitAngle && abs(rotate.z) <= limitAngle)
    {
        transform_.rotate_.x = rotate.x;
        transform_.rotate_.z = rotate.z;
    }
    else
    {
        SetRayCastSlope(rayCastType, false);
        return false;
    }

    SetRayCastSlope(rayCastType, true);
    return true;
}

// NPC�p���C�L���X�g�Ƀf�[�^�Z�b�g
void Vehicle::SetRayCastHit(int number, const RayCastData& rcd)
{
    if (isPlayer_)
    {
        return;
    }

    rayCastHit_[number].dir = rcd.dir;
    rayCastHit_[number].dist = rcd.dist;
    rayCastHit_[number].end = rcd.end;
    rayCastHit_[number].hit = rcd.hit;
}

//NPC�̃��C�L���X�g�p
void Vehicle::SetRayCastSlope(int number, bool flag)
{
    if (isPlayer_)
    {
        return;
    }

    rayCastHit_[number].road = flag;
}

//�o�E���f�B���O�{�b�N�X�̏Փ�
void Vehicle::CollideBoundingBox(Vehicle* pVehicle)
{
    XMFLOAT3 posCenter = GetBoundingBoxCenter();
    XMVECTOR posVec = XMLoadFloat3(&posCenter);

    XMFLOAT3 OppCenter = pVehicle->GetBoundingBoxCenter();
    XMVECTOR oppVec = XMLoadFloat3(&OppCenter);

    XMVECTOR posToOpp = XMVector3Normalize(posVec - oppVec);

    //����������
    acceleration_ *= wallReflectionForce_;

    //����ł�������ۂ�
    acceleration_ += posToOpp * *XMVector3LengthEst(pVehicle->GetAcceleration()).m128_f32
        * pVehicle->GetMass() * collideBoxValue_;
}

//������͂̔��f
void Vehicle::InputReceive(const XMVECTOR& vecX, const XMVECTOR& vecZ)
{
    //���Z�b�g
    operation_.Refresh();

    //�������
    InputOperate();

    //�n���h�����Z�b�g
    handleFlag_ = false;

    //����ł��Ȃ���Ԃ��H
    if (isOperationInvalid_)
        return;

    //�n���h���̑���
    HandleTurnLR(operation_.ValueMap[Operation::Value::handleRightLeft]);

    //�n���h���p�x����
    //�Ȃ���₷��
    if (slideFlag_)
        AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
    else
        AngleLimit(handleRotate_, handleRotateMax_);

    //�O�i,���
    if (landingFlag_)
    {
        acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration *
            operation_.ValueMap[Operation::Value::moveFrontRear];
    }

    //�u�[�X�g
    if (operation_.ButtonMap[Operation::Button::boost])
    {
        //�e�ʂ�����Ȃ�
        if (boostCapacity_ >= boostSpending_)
        {
            //����
            boostCapacity_ -= boostSpending_;
            slideFlag_ = true;
            acceleration_ += vecZ * boostValue_;
        }
        else
            slideFlag_ = false;
    }
    else
    {
        slideFlag_ = false;

        //������
        if (landingFlag_)
            boostCapacity_ += boostIncrease_;
        //���ӂ��
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }

#ifdef _DEBUG

    //���E��]
    transform_.rotate_.y += rotateSPD_ * operation_.ValueMap[Operation::Value::turnRightLeft];

    //�W�����v
    if (operation_.ButtonMap[Operation::Button::jump])
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    //���E�ړ�
    acceleration_ += vecX * operation_.ValueMap[Operation::Value::moveRightLeft];
#endif
}

//�^�C���̍����Z�b�^�[ //�ԑ̂Ƃ̍������v�Z
void Vehicle::SetWheelHeight(float height)
{
    Size.wheelHeight_ = height;

    Size.wheelRemainder_ = height - Size.wheelFL_.y;
    if (Size.wheelRemainder_ < 0.0f)
        Size.wheelRemainder_ = 0.0f;

    Size.toWheelBottom_ = Size.toBottom_ + Size.wheelRemainder_;
    Size.topToWheelBottom_ = Size.toWheelBottom_ + Size.toTop_;
    Size.centerPositionRemainder_ = Size.centerTopToBottom_ - Size.wheelRemainder_;
    Size.centerTopToBottom_ = (Size.topToBottom_ + Size.wheelRemainder_) * 0.5f;
}

//�G�t�F�N�g
void Vehicle::VehicleParticle()
{
    //�����ɂ���NPC�̓G�t�F�N�g�\�����Ȃ�
    if (!isPlayer_ && toPlayerVehicleLength_ > particleLimitLength_)
        return;

    //NPC�͊m���ŃG�t�F�N�g�𔭐������Ȃ�(�d�����������邳���Ȃ�)
    if (!isPlayer_ && Calculator::IsProbability(npcParticleRandom_))
        return;

    float accLength = *XMVector3Length(acceleration_).m128_f32;

    //�u�[�X�g
    if (slideFlag_)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::boost
            , Model::GetBonePosition(hModel_, "rear"), acceleration_);
    }

    //���s���̃^�C���̋O��
    if (wheelParticleLength_ < accLength && wheelParticleLengthMax_ > accLength)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRL"));
    }
    //�h���t�g�̉Ή�
    else if (landingFlag_ && accLength > sparkParticleLength_)
    {
        if (handleRotate_ > sparkParticleHanlde_)
        {
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::spark
                , Model::GetBonePosition(hModel_, "wheelRL"), acceleration_);
        }
        else if (handleRotate_ < -sparkParticleHanlde_)
        {
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::spark
                , Model::GetBonePosition(hModel_, "wheelRR"), acceleration_);
        }
    }

    //���s���̂��ꂼ��̃G�t�F�N�g
    if (landingFlag_ && wheelParticleLength_ < accLength)
    {
        switch (landingType_)
        {
        default:
            break;
        case Circuit::circuitType::turf://���n
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::grass
                , transform_.position_);
            break;
        case Circuit::circuitType::dirt://���n
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::dirt
                , transform_.position_);
            break;
        case Circuit::circuitType::ice://�X��
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::ice
                , transform_.position_);
            break;
        }
    }
}

//�ޗ��ɗ���
void Vehicle::FallAbyss()
{
    //���Z�b�g
    acceleration_ *= {0.0f, 0.0f, 0.0f, 0.0f};
    transform_.position_ = restartTransform_.position_;
    transform_.rotate_ = restartTransform_.rotate_;

    //�������J�����ړ�
    if (pViewer_ != nullptr)
    {
        pViewer_->WatchPresentPosition();
    }
}


//�ԗ��̑���A���͂̎�t
void Vehicle::InputOperate() {};

//UI�̊֐��Q�@�v���C���[����ō�p����
    //UI�̏�����
void Vehicle::PlayerUI_Initialize() {};
//UI�̕\��
void Vehicle::PlayerUI_Draw() {};
//UI�̏��X�V
void Vehicle::PlayerUI_Update() {};
//�J�����̗p��
void Vehicle::PlayerCamera_Initialize() {};
//�J�����X�V
void Vehicle::PlayerCamera_Update() {};