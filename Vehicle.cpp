#include <directxcollision.h>
#include <directxmath.h>
#include <directxpackedvector.h>

#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
#include "Engine/Particle.h"
#include "Engine/Audio.h"
#include "Engine/Global.h"
#include "Engine/SceneManager.h"

#include "Vehicle.h"
#include "Ground.h"
#include "Bullet.h"
#include "Viewer.h"
#include "Sample.h"
#include "Tachometer.h"
#include "Tracker.h"
#include "Speedometer.h"
#include "CheckPoint.h"
#include "VehicleWheel.h"
#include "ParticlePackage.h"
#include "MeasurePole.h"
#include "Music.h"
#include "ImagePrinter.h"
#include "PoryLine.h"
#include "Circuit.h"

using std::vector;
using std::string;

//�R���X�g���N�^
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle")
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(0.98f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f)
    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), population_(1), goalTime_(0)
    , mass_(1.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_()
    , handleRight_(1), handleLeft_(-1)
{
    Size.wheelHeight_ = 0.1f;
}

//�p���p
Vehicle::Vehicle(GameObject* parent, const std::string& name)
    :GameObject(parent, name)
    //���f���ԍ�
    , hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    //�x�N�g��
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    //�X�s�[�h�Ȃ�
    , moveSPD_(0.01f), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)

    //�n���h���֌W
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    //���C
    , wheelFriction_(0.99f), airFriction_(0.999f), turfFriction_(0.98f)
    , iceFriction_(0.999f)
    , slideFlag_(false)
    , sideWheelFriction_(0.2f), sideSlideFriction_(0.1f), sideIceFriction_(0.02f)
    , landingFriction_(1.0f), sideFriction_(1.0f)

    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(60.0f)

    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(0), pointCountMax_(1), lapCount_(0), lapMax_(1)
    , ranking_(0), goalRanking_(0), population_(1), goalTime_(0), standbyTime_(0)
    , mass_(1.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Circuit::circuitType::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f), wheelParticleLengthMax_(0.5f)
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_(), restartTransform_()

    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.99f)
    , handleRight_(1), handleLeft_(-1)
    //�u�[�X�g
    , boostCapacityMax_(2000.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.5f), boostValue_(2.0f)
    , isPlayer_(false)
    , collideBoxValue_(0.5f)
    , isOperationInvalid_(false), pauseFlag_(false)
    , pViewer_(nullptr)
    , pPoryLine_(nullptr)
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
    GroundTypeFriction_[Circuit::circuitType::boost].acceleration = 1.0f;
    GroundTypeFriction_[Circuit::circuitType::boost].landing = 1.05f;
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
    SphereCollider* collision = new SphereCollider(Model::GetBonePosition(hModel_, "center")
                                , Size.centerFrontToRear_);
    AddCollider(collision);

    //�G�t�F�N�g�̃|�C���^�Ƃ���̂܂Ƃ�
    pParticle_ = Instantiate<Particle>(this);

    //�X�e�[�W�I�u�W�F�N�g��T��
    //�����ƃp�[�c�������Ă��邩 (����ȍ~�̓|�C���^�������ăp�[�c�����邱�Ƃ�O��ɂ���)
    //pGround_ = (Ground*)FindObject("Ground");
    //assert(pGround_ != nullptr);
    //assert(!(pGround_->GetCircuitUnion() == nullptr));
    //assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    //�ăX�^�[�g�n�_
    restartTransform_ = startTransform_;

    //UI������
    PlayerUI_Initialize();

    //�|�����C��������
    pPoryLine_ = new PoryLine;
    pPoryLine_->Load("image\\PaticleAssets\\tex.png");
}

//�X�V
void Vehicle::Update()
{
    //�|�[�Y
    if (pauseFlag_)
        return;

    Debug::TimerLogStart("vehicle�ŏ�");

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

    //���̓s�x�����x����]������񂶂�Ȃ��čŌ�ɉ�]�����ق�����������

    Debug::TimerLogEnd("vehicle�ŏ�");

    //�S�[��  
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

    //�O�����ɐi��ł邩������������
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
        accZDirection_ = -1;

    Debug::TimerLogStart("vehicle����󂯂�");

    InputReceive(vecX, vecZ);

    Debug::TimerLogEnd("vehicle����󂯂�");

    //�n�ʂ̎�ނɂ����
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
            acceleration_ *= {0.0f, 0.0f, 0.0f, 0.0f};
            transform_.position_ = restartTransform_.position_;
            transform_.rotate_   = restartTransform_.rotate_;
            //�������J�����ړ�
            if (pViewer_ != nullptr)
                pViewer_->WatchPresentPosition();
        }
    }

    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //�n��Ȃ�n���h���ɍ��킹�ĉ�]
    if (landingFlag_)
    {
        //�O��ɓ����Ă���Ȃ�
        transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_ * accZDirection_;
        handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
    }
    //�n���h����߂�
    if (!handleFlag_)
        handleRotate_ *= handleAdjust_;

    //�^�C���̊p�x�ƌ���
    //���ʂ�^�C���̕����ւ͌������Ȃ����A�^�C���̕����ƕ��s�̕����ւ͌�������
    //�^�C���̕����ƕ��s�Ȃ牽�����Ȃ����A�����ɋ߂��قǃ^�C���̕����Ƀx�N�g���𔭐�������
    //�����F����̂����ŏd���\�������邩��ς���ׂ���������Ȃ�
    Debug::TimerLogStart("vehicle�^�C��������");
    TurnWheel();
    Debug::TimerLogEnd("vehicle�^�C��������");

    //�ʒu�@�{�@�x�N�g��
    XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));

    Debug::TimerLogStart("vehicle�Ǐ��Փ�");
    //�ڒn�A�ǏՓ�
    VehicleCollide();
    Debug::TimerLogEnd("vehicle�Ǐ��Փ�");

    //�^�C���̒l�Z�b�g
    pWheels_->SetWheelSpeedRotate(*XMVector3LengthEst(acceleration_).m128_f32
        * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //�J�����X�V
    PlayerCamera_Update();

    //�X�s�[�h���[�^�[
    PlayerUI_Update();

    //�G�t�F�N�g
    //PlayerParticle();
    VehicleParticle();

    //�p�x���K��
    transform_.rotate_.x = Calculator::AngleNormalize(transform_.rotate_.x);
    transform_.rotate_.y = Calculator::AngleNormalize(transform_.rotate_.y);
    transform_.rotate_.z = Calculator::AngleNormalize(transform_.rotate_.z);

    //�|�����C���Ɍ��݂̈ʒu��`����
    pPoryLine_->AddPosition(transform_.position_);
}

//�����ɓ�������
void Vehicle::OnCollision(GameObject* pTarget)
{
    //�|�[�Y
    if (pauseFlag_)
        return;

    Debug::TimerLogStart("vehicle�`�F�b�N�|�C���g");

    //���������Ƃ��̏���
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() == pointCount_)
            pointCount_++;

        if (pCP->GetNumber() <= 0 && pointCount_ >= pointCountMax_)
        {
            pointCount_ = 0;
            lapCount_++;
        }
    }

    Debug::TimerLogEnd("vehicle�`�F�b�N�|�C���g");
}

//�`��
void Vehicle::Draw()
{
    Debug::TimerLogStart("vehicle�`��");

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    PlayerUI_Draw();

    //�|�����C����`��
    //pPoryLine_->Draw();

    Debug::TimerLogEnd("vehicle�`��");
}

//�J��
void Vehicle::Release()
{
    //�|�����C�����
    pPoryLine_->Release();
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
    acceleration_ +=  *XMVector3LengthEst(acceleration_).m128_f32
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
    for(auto& itr : Circuit::GetChosenCircuit()->parts_)
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

    //���� �����ɂ���Ƃ��܂�����
    if (!landingFlag_)
    {
        acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
    }
}

//�ڒn 
void Vehicle::Landing(int hModel,int type)
{
    //���C�̔��ˈʒu
    RayCastData data;
    data.start = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    data.start = Transform::Float3Add(data.start, upF);

    data.dir = { 0.0f, -1.0f, 0.0f };   //���C�̕���
    Model::RayCast(hModel, &data);      //���C�𔭎�

    //���C������������
    if (data.hit)
    {
        //NPC�p
        SetRayCastHit(RayCastHit::Number::down, data);

        if (-data.dist > XMVectorGetY(acceleration_) - Size.toWheelBottom_)
        {
            landingType_ = type;    //�n�ʂ̃^�C�v

            //�������̉����x���傫���Ȃ�@�n�ʂɃ��[�v�@�������x���O
            if (!landingFlag_)
            {
                transform_.position_.y -= data.dist - Size.toWheelBottom_;
                acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            }

            landingFlag_ = true;
        }
        else
            landingFlag_ = false;

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
    if (vehicleData.hit && vehicleData.dist < Size.toWheelBottom_ + Size.wheelRemainder_)
    {
        landingType_ = type;    //�n�ʂ̃^�C�v

        XMFLOAT3 wheelFlo;
        XMStoreFloat3(&wheelFlo, -vehicleVector_.y * (vehicleData.dist - Size.toWheelBottom_));
        transform_.position_ = Transform::Float3Add(transform_.position_, wheelFlo);
        landingFlag_ = true;
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
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};  //�~�߂�
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

        //���C�L���X�g�̎��p
        enum Direction
        {
            front = 0,
            right = 1,
            rear = 2,
            left = 3,
        };

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

                    switch (i){
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

        enum Oblique
        {
            frontLeft = 0,
            frontRight,
            rearLeft,
            rearRight
        };

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

                    //�����x���O�ɂ�����͂��Ȃ�
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
    Size.toRight_ = *XMVector3Length(XMLoadFloat3(&vehicleRight)    - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toLeft_    = *XMVector3Length(XMLoadFloat3(&vehicleRight)  - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toFront_   = *XMVector3Length(XMLoadFloat3(&vehicleFront)  - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toRear_    = *XMVector3Length(XMLoadFloat3(&vehicleRear)   - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toTop_     = *XMVector3Length(XMLoadFloat3(&vehicleTop)    - XMLoadFloat3(&vehicleCenter)).m128_f32;
    Size.toBottom_  = *XMVector3Length(XMLoadFloat3(&vehicleBottom) - XMLoadFloat3(&vehicleCenter)).m128_f32;

    Size.rightToLeft_   = Size.toRight_ + Size.toLeft_;
    Size.topToBottom_   = Size.toBottom_ + Size.toTop_;
    Size.frontToRear_   = Size.toFront_ + Size.toRear_;
    Size.toFrontRight_  = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toRight_ * Size.toRight_));
    Size.toFrontLeft_   = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toLeft_ * Size.toLeft_));
    Size.toRearRight_   = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toRight_ * Size.toRight_));
    Size.toRearLeft_    = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toLeft_ * Size.toLeft_));

    Size.centerRightToLeft_ = Size.rightToLeft_ * 0.5f;
    //Size.centerTopToBottom_ = (Size.topToBottom_ + Size.wheelRemainder_) * 0.5f;
    Size.centerFrontToRear_ = Size.frontToRear_ * 0.5f;

    //�p�x
    Size.angleFrontLeft_    = XMConvertToDegrees(        acos(Size.toFront_ / Size.toFrontLeft_));
    Size.angleFrontRight_   = XMConvertToDegrees(XM_PI - acos(Size.toFront_ / Size.toFrontRight_));
    Size.angleRearLeft_     = XMConvertToDegrees(         acos(Size.toRear_  / Size.toFrontLeft_) + XM_PI);
    Size.angleRearRight_    = XMConvertToDegrees(XM_2PI - acos(Size.toRear_  / Size.toFrontRight_));
}

//�n���h���̑���
void Vehicle::HandleTurnLR(int LR)
{
    //��₱�������n���h����]�̂��߂ɂ������Ă���
    if((LR < 0 && handleRotate_ < 0) || (LR > 0 && handleRotate_ > 0))
        handleFlag_ = true;

    //�����Ă�ƋȂ���₷��
    if (slideFlag_)
        handleRotate_ += rotateSPD_ * slideHandleRotateAdd_ * LR;
    else
        handleRotate_ += rotateSPD_ * LR;
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

    //acceleration_ += posToOpp * *XMVector3LengthEst(acceleration_).m128_f32;
    //acceleration_ += pVehicle->GetAcceleration();
    //acceleration_ += posToOpp * ((*XMVector3LengthEst(pVehicle->GetAcceleration()).m128_f32
    //    + *XMVector3LengthEst(acceleration_).m128_f32) * 0.5f);
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
    {
        return;
    }

    //�n���h���̑���
    if (operation_.inputNow[operation_.inputName::handleLeft])
    {
        HandleTurnLR(handleLeft_);
    }
    if (operation_.inputNow[operation_.inputName::handleRight])
    {
        HandleTurnLR(handleRight_);
    }

    //�n���h���p�x����
    //�Ȃ���₷��
    if (slideFlag_)
        AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
    else
        AngleLimit(handleRotate_, handleRotateMax_);

    if (landingFlag_)
    {
        //�S�g���
        if (operation_.inputNow[operation_.inputName::moveFront])
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (operation_.inputNow[operation_.inputName::moveRear])
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //�u�[�X�g
    if (operation_.inputNow[operation_.inputName::boost])
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
        {
            slideFlag_ = false;
        }
    }
    else
    {
        slideFlag_ = false;

        if (landingFlag_)
        {
            boostCapacity_ += boostIncrease_;   //������
        }

        //���ӂ��
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }

#ifdef _DEBUG
    //���E
    if (operation_.inputNow[operation_.inputName::turnLeft])
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (operation_.inputNow[operation_.inputName::turnRight])
    {
        transform_.rotate_.y += rotateSPD_;
    }

    //�W�����v
    if (operation_.IsDown(operation_.inputName::jump))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (operation_.inputNow[operation_.inputName::moveLeft])
    {
        acceleration_ -= vecX;
    }
    if (operation_.inputNow[operation_.inputName::moveRight])
    {
        acceleration_ += vecX;
    }
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
    float accLength = *XMVector3Length(acceleration_).m128_f32;

    //�S�[��������
    if (goalFlag_)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::gold
            , transform_.position_);
    }

    //ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::gold
    //    , transform_.position_);

    //ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::spark
    //   , Model::GetBonePosition(hModel_, "rear"), acceleration_);

    //�u�[�X�g
    if (slideFlag_)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::boost
            , Model::GetBonePosition(hModel_, "rear"), acceleration_);
    }

    //���s���̃^�C���̋O��
    if (wheelParticleLength_ < accLength
        && wheelParticleLengthMax_ > accLength)
    {
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::smoke
            , Model::GetBonePosition(hModel_, "wheelRL"));
    }

    if (landingFlag_ && wheelParticleLength_ < accLength)
    {
        switch (landingType_)
        {
        default:
            break;
        case Circuit::circuitType::turf://���n���グ
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::grass
                , transform_.position_);
            break;
        case Circuit::circuitType::dirt://���n
            ParticlePackage::ActParticle(pParticle_, ParticlePackage::ParticleName::dirt
                , transform_.position_);
            break;
        }
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
    //�G�t�F�N�g��\��
    void Vehicle::PlayerParticle() {};
    //�J�����X�V
    void Vehicle::PlayerCamera_Update() {};

#if 0
�������Z�Ɋւ��郁��

�E�^���̖@��
�����x[a]�͉�������[F]�ɔ�Ⴕ�A����[m]�ɔ���Ⴗ��B[k]�͔��萔
a = k * (F / m)     �����āA
F = m * a
�E�����ɂ���Ĕ������錩�����̗͂́@ - a * m

�E���S��
���Ԃ񊵐��̈ꕔ�B���S�͂ƒނ荇����
���ar[m]�̉~����A����m[kg]�E���xv[m / s]�œ����~�^�����Ă��镨�̂ōl����ƁA[��]�͊p���x
���S�� = mr��2 = m * (v ^ 2 / r)

�E�X���b�v��
�^�C���ƘH�ʊԂ̖��C�������Ȃ�Ƃ�
�X���b�v�� = (�ԑ̑��x - �ԗւ̉�]���x) / �ԑ̑��x


�^���l�@
�E�Ԃ̎��l
�E�G���W����]���F�^�R���[�^�[�ŕ\���@�N���b�`���q�����Ă�Ȃ珙�X�ɒl���^�C���Ɉڂ��@W, S�L�[�ő���
�E�^�C����]���F�ڒn���Ă�Ȃ�O�^�C���̕����։�]���̕��A�����x�����Z����
�E�^�C���p�x�F���̎��Ɋp�x�̕������x�N�g������]������B����(���S��)�̂��߂Ɋp�x���傫���Ɖ�]������Ȃ����Ƃɂ��邩
A, D�L�[�ő���

/*
XMFLOAT3 �� XMVECTOR

XMFLOAT3 f;	//���������Ă�Ƃ���
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 �� XMVECTOR

XMVECTOR v; 	//���������Ă�Ƃ���
XMFLOAT3 f;
XMStoreFloat3(&f, v);

std::string str;
        str = std::to_string(transform_.position_.x);
        str += "\n" + std::to_string(transform_.position_.y);
        str += "\n" + std::to_string(transform_.position_.z);
        MessageBox(NULL, (LPCWSTR)str.c_str(), L" puzzleList_.size() ", MB_OK);
*/
#endif
