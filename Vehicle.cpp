#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
#include "Engine/Particle.h"

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
    , ranking_(0), population_(1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
    , startPosition_({ 0.0f,0.0f,0.0f }), startRotate_({ 0.0f,0.0f,0.0f })
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
    , ranking_(0), population_(1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
    , startPosition_({ 0.0f,0.0f,0.0f }), startRotate_({ 0.0f,0.0f,0.0f })
    , vehicleModelName_(""), wheelModelName_("")
    , startTransform_()
    , handleRight_(1), handleLeft_(-1)
{
    matRotateX = XMMatrixIdentity();
    matRotateY = XMMatrixIdentity();
    matRotateZ = XMMatrixIdentity();
    matRotateX_R = XMMatrixIdentity();
    matRotateY_R = XMMatrixIdentity();
    matRotateZ_R = XMMatrixIdentity();

    worldVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });
    vehicleVector_.Set({ 1.0f,0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f,0.0f });

    Size.wheelHeight_ = 0.1f;

    //�n�ʂ̃^�C�v
    //�ʏ퓹�H
    GroundTypeFriction_[Ground::road].acceleration = 1.0f;
    GroundTypeFriction_[Ground::road].landing = 0.99f;
    GroundTypeFriction_[Ground::road].side = 0.2f;
    //���n
    GroundTypeFriction_[Ground::turf].acceleration = 0.98f;
    GroundTypeFriction_[Ground::turf].landing = 0.98f;
    GroundTypeFriction_[Ground::turf].side = 0.2f;
    //���n�@���Ɠ���
    GroundTypeFriction_[Ground::turf].acceleration = GroundTypeFriction_[Ground::turf].acceleration;
    GroundTypeFriction_[Ground::turf].landing = GroundTypeFriction_[Ground::turf].landing;
    GroundTypeFriction_[Ground::turf].side = GroundTypeFriction_[Ground::turf].side;
    //�X��
    GroundTypeFriction_[Ground::ice].acceleration = 0.5f;
    GroundTypeFriction_[Ground::ice].landing = 0.999f;
    GroundTypeFriction_[Ground::ice].side = 0.02f;
    //������
    GroundTypeFriction_[Ground::boost].acceleration = 1.0f;
    GroundTypeFriction_[Ground::boost].landing = 1.05f;
    GroundTypeFriction_[Ground::boost].side = 0.2f;
    //�ޗ�
    GroundTypeFriction_[Ground::abyss]; //��邾��
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

    PlayerCamera_Initialize();

    //�T�C�Y�v�Z
    SetVehicleSize(hModel_);

    //�^�C��
    hWheelModel_ = Model::Load(wheelModelName_);
    assert(hWheelModel_ >= 0);
    MakeWheels(hWheelModel_);

    //�����蔻��
    SphereCollider* collision = new SphereCollider(Model::GetBonePosition(hModel_, "center"), Size.frontToRear_ * 0.5f);
    AddCollider(collision);

    //�G�t�F�N�g�̃|�C���^�Ƃ���̂܂Ƃ�
    pParticle_ = Instantiate<Particle>(this);

    //�X�e�[�W�I�u�W�F�N�g��T��
    //�����ƃp�[�c�������Ă��邩 (����ȍ~�̓|�C���^�������ăp�[�c�����邱�Ƃ�O��ɂ���)
    pGround_ = (Ground*)FindObject("Ground");
    assert(pGround_ != nullptr);
    assert(!(pGround_->GetCircuitUnion() == nullptr));
    assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    //UI������
    PlayerUI_Initialize();
}

//�X�V
void Vehicle::Update()
{
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

    //�N�[���^�C��
    if (coolTime_ > 0)
        coolTime_--;

    //����
    if (!(goalFlag_))
        time_++;

    //�f�S�[��  
    if (lapCount_ >= lapMax_)
        goalFlag_ = true;

    //�O�����ɐi��ł邩������������
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
    {
        accZDirection_ = -1;
    }

    Debug::TimerLogStart("vehicle����󂯂�");
    //�n���h���̑���
    handleFlag_ = false;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
    {
        HandleTurnLR(handleLeft_);
    }
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
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
        if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //����
    if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
    {
        slideFlag_ = true;

        acceleration_ += vecZ * 2.0f;

        XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "rear");
        ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
    }
    else
    {
        slideFlag_ = false;
    }

#ifdef _DEBUG
    //���E
    if (Input::IsKey(DIK_Z))
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (Input::IsKey(DIK_X))
    {
        transform_.rotate_.y += rotateSPD_;
    }

    if (Input::IsKey(DIK_E))
    {
    }
    if (Input::IsKey(DIK_Q))
    {
    }

    //�㏸
    if (Input::IsKeyDown(DIK_M))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (Input::IsKey(DIK_V))
    {
        acceleration_ -= vecX;
    }
    if (Input::IsKey(DIK_B))
    {
        acceleration_ += vecX;
    }
#endif
    Debug::TimerLogEnd("vehicle����󂯂�");

    //�n�ʂ̎�ނɂ����
    if (landingFlag_)
    {
        acceleration_ *= { GroundTypeFriction_[landingType_].landing, 1.0f
                         , GroundTypeFriction_[landingType_].landing, 1.0f };

        if (landingType_ == Ground::abyss)
        {
            //�ޗ��ɗ���
            acceleration_ *= {0.0f, 0.0f, 0.0f, 0.0f};
            transform_.position_ = startTransform_.position_;
            transform_.rotate_   = startTransform_.rotate_;
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
    //���ʂ�^�C���̕����ւ͌������Ȃ����A�^�C���̕����ƕ��s�̂ق������ւ͌�������
    //�^�C���̕����ƕ��s�Ȃ牽�����Ȃ����A�����ɋ߂��قǃ^�C���̕����Ƀx�N�g���𔭐�������
    //�����F����̂����ŏd���\�������邩��ς���ׂ���������Ȃ�
    Debug::TimerLogStart("vehicle�^�C��������");
    if (landingFlag_)
    {
        TurnWheel();
    }
    Debug::TimerLogEnd("vehicle�^�C��������");


    //�����̒���
    //����Ȃ���
    //SpeedLimit(acceleration_, speedLimit_);

    //�ʒu�@�{�@�x�N�g��
    XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));

    Debug::TimerLogStart("vehicle�ǏՓ�");
        //�ڒn�A�ǏՓ�
        VehicleCollide();
    Debug::TimerLogEnd("vehicle�ǏՓ�");

    //�^�C���̒l�Z�b�g
    pWheels_->SetWheelSpeedRotate(*XMVector3LengthEst(acceleration_).m128_f32
        * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //�X�s�[�h���[�^�[
    PlayerUI_Update();

    //�G�t�F�N�g
    PlayerParticle();

#if 0
    //�e�X�g
    XMVECTOR bulletVec = XMVector3Normalize(vehicleVector_.z);

    //�e�𔭎�
    if (Input::IsMouseButton(0) && coolTime_ <= 0)
    {
        Bullet* pBullet = Instantiate<Bullet>(GetParent());
        XMFLOAT3 BulletPos = Model::GetBonePosition(hModel_, "center");
        pBullet->SetPosition(BulletPos);

        float power = bulletPower_;
        bulletVec *= power;
        XMFLOAT3 speed;
        XMStoreFloat3(&speed, bulletVec);
        pBullet->SetSpeed(speed);

        coolTime_ = heatAdd_;
    }
#endif
}

//�����ɓ�������
void Vehicle::OnCollision(GameObject* pTarget)
{
    Debug::TimerLogStart("vehicle�`�F�b�N�|�C���g");

    //���������Ƃ��̏���
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() == pointCount_)
        {
            pointCount_++;
        }
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

    Debug::TimerLogEnd("vehicle�`��");
}

//�J��
void Vehicle::Release()
{
}
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
void Vehicle::Accelerator()
{
#if 0
    transform_.position_.x += acceleration_.x;
    transform_.position_.y += acceleration_.y;
    transform_.position_.z += acceleration_.z;
#endif
}

void Vehicle::SpeedLimit(XMVECTOR& speed, const float limit)
{
    //�����𒲂ׂ�
    float* pLength = XMVector3LengthEst(speed).m128_f32;
    if (*pLength > limit)
    {
        *pLength = limit;
    }
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
    {
        minusFlag = true;
    }
    //�p�x�̐�Βl���ő�p�x�̐�Βl�𒴂��Ă�����ő�p�x�ɂ���
    //�y�ʉ��ł��邩
    if (fabs(angle) > fabs(limit))
    {
        angle = fabs(limit);

        //�}�C�i�X��������}�C�i�X�ɖ߂�
        if (minusFlag == true)
        {
            angle *= -1;
        }
    }
    //XMVector3ClampLength
}

//�^�C����]
void Vehicle::TurnWheel()
{
    XMVECTOR normalAcc = XMVector3Normalize(acceleration_);

    //������y���������+�A�E���Ɖ�����-
    XMVECTOR outerProduct = XMVector3Cross(vehicleVector_.z, normalAcc);

    //��]���ė��p����
    XMMATRIX MatRotateZ = XMMatrixRotationZ(XM_PIDIV2);
    //MatRotateZ = XMMatrixRotationNormal(vehicleVector_.z, XM_PIDIV2);
    outerProduct = XMVector3TransformCoord(outerProduct, MatRotateZ);

    //�O�ς���]
    outerProduct = XMVector3TransformCoord(outerProduct, matRotateZ);
    outerProduct = XMVector3TransformCoord(outerProduct, matRotateX);
    outerProduct = XMVector3TransformCoord(outerProduct, matRotateY);

    acceleration_ += (*XMVector3LengthEst(acceleration_).m128_f32)
                      * outerProduct * GroundTypeFriction_[landingType_].side;
}

//�n�ʁA�ǁA�G�Ƃ̏Փ˂��܂Ƃ߂�H�H
void Vehicle::VehicleCollide()
{
    bool isLanding = false;

    //��ނ̕�����
    for (int i = 0; i < pGround_->GetCircuitUnion()->parts_.size(); i++)
    {
        Debug::TimerLogStart("vehicle�n�ʓ����蔻��");
            //�n��
            if (!isLanding)
            { 
                isLanding = Landing(pGround_->GetCircuitUnion()->parts_[i].model_
                    , pGround_->GetCircuitUnion()->parts_[i].type_);
            }
        Debug::TimerLogEnd("vehicle�n�ʓ����蔻��");


        Debug::TimerLogStart("vehicle�Ǔ����蔻��");
            //��
            CollideWall(pGround_->GetCircuitUnion()->parts_[i].model_
                , pGround_->GetCircuitUnion()->parts_[i].type_);
        Debug::TimerLogEnd("vehicle�Ǔ����蔻��");
    }
}

//�ڒn 
bool Vehicle::Landing(int hModel,int type)
{
    XMFLOAT3 pos = transform_.position_;
    XMVECTOR up = vehicleVector_.y * Size.toBottom_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, up);
    pos.x += upF.x;
    pos.y += upF.y;
    pos.z += upF.z;

    //XMFLOAT3 bp = Model::GetBonePosition(hModel_, "center");

    RayCastData data;

    data.start = pos;      //���C�̔��ˈʒu

    data.dir = { 0.0f, -1.0f, 0.0f };     //���C�̕���
    //XMStoreFloat3(&data.dir, -vehicleVector_.y);
    Model::RayCast(hModel, &data);      //���C�𔭎�
    
    bool isHit = false;

    //���C������������
    if (data.hit)
    {
        landingType_ = type;
        isHit = true;

        if (-data.dist > XMVectorGetY(acceleration_) - gravity_ - Size.toWheelBottom_)
        {
            //�������̉����x���傫���Ȃ�@�n�ʂɃ��[�v�@�������x���O
            transform_.position_.y -= data.dist;

            XMFLOAT3 wheelFlo;
            XMStoreFloat3(&wheelFlo, vehicleVector_.y * Size.toWheelBottom_);
            //transform_.position_.x += wheelFlo.x;
            transform_.position_.y += wheelFlo.y;
            //transform_.position_.z += wheelFlo.z;

            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            landingFlag_ = true;
        }
        else
        {
            //����
            landingFlag_ = false;
            acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};           
        }

        //�d��
        //acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};

        if (landingFlag_)
        {
            //�⓹����(?)
            //acceleration_ -= data.normal * gravity_;

            //�p�x��ς���
            //��]
            XMVECTOR normalVec = XMVector3TransformCoord(data.normal, matRotateY_R);

            //X���̊p�x���擾
            transform_.rotate_.x = XMConvertToDegrees(*XMVector3AngleBetweenNormals(
                                    worldVector_.z, normalVec).m128_f32) - 90.0f;

            //Z��
            transform_.rotate_.z = -(XMConvertToDegrees(*XMVector3AngleBetweenNormals(
                                    worldVector_.x, normalVec).m128_f32) - 90.0f);
        }
    }

    //�⓹���ړ����Ă�Ƃ��ɍ⓹�ɒ���t��
    //�}������ƌ��ʂȂ�
    RayCastData vehicleData;
    //vehicleData.start = transform_.position_;
    //vehicleData.start = pos;
    XMStoreFloat3(&vehicleData.dir, -vehicleVector_.y);
    Model::RayCast(hModel, &vehicleData);      //���C�𔭎�
    if (vehicleData.hit)
    {
        //���፷���^�C���̒��a���炢�̎�
        if (vehicleData.dist > 0.0f && vehicleData.dist < Size.toWheelBottom_)
        {
            XMFLOAT3 wheelFlo;
            XMStoreFloat3(&wheelFlo, vehicleVector_.y * (vehicleData.dist - Size.toWheelBottom_));
            //transform_.position_.x -= wheelFlo.x;
            //transform_.position_.y -= wheelFlo.y;
            //transform_.position_.z -= wheelFlo.z;
        }

    }


    //�V�� �ق�Ƃ͕�������ׂ����낤���ǓV��ɓ����邱�Ƃ͒��������d���������炱���ɒu��
    XMStoreFloat3(&data.dir, vehicleVector_.y);
    Model::RayCast(hModel, &data);  //���C�𔭎�

    if (data.hit)
    {   
        //������ƒn�ʂɖ��܂����Ƃ�
        if (data.dist < Size.toTop_)
        {
            //���̕��ʒu���グ��
            XMFLOAT3 upVec;
            XMStoreFloat3(&upVec, data.normal * -(data.dist + Size.toWheelBottom_));
            transform_.position_.x += upVec.x;
            transform_.position_.y += upVec.y;
            transform_.position_.z += upVec.z;

            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
        //�V��ɂԂ������Ƃ�
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            //�~�߂�
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
    }

    return isHit;
}

void Vehicle::CollideWall(int hModel, int type)
{
    //���C�L���X�g�̎�����
    enum Direction
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    };

    //�O�㍶�E�Ǝ΂߂ŕ������邱�Ƃɂ���
    std::array<RayCastData, 4>wallCollideVertical;

    //�O�㍶�E��
    for (int i = 0; i < wallCollideVertical.size(); i++)
    {
        //�ԗ��̒��S���烌�C�𔭎� (�{�[���擾�������Ƃ��܂������Ȃ�����)
        wallCollideVertical[i].start = transform_.position_;
        XMFLOAT3 upF;
        XMStoreFloat3(&upF, vehicleVector_.y* Size.toBottom_);
        wallCollideVertical[i].start.x += upF.x;
        wallCollideVertical[i].start.y += upF.y;
        wallCollideVertical[i].start.z += upF.z;

        //90�x����]
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(90.0f * i));	//�x���ŉ�]������s��   

        XMVECTOR dirVec = worldVector_.z;
        
        dirVec = XMVector3TransformCoord(dirVec, matRot);
        dirVec = XMVector3TransformCoord(dirVec, matRotateZ);
        dirVec = XMVector3TransformCoord(dirVec, matRotateX);    
        dirVec = XMVector3TransformCoord(dirVec, matRotateY);

        XMStoreFloat3(&wallCollideVertical[i].dir, dirVec);

        if (coolTime_ <= 0 && i == 3)
        {
            
            Bullet* pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideVertical[i].start);
            pBullet->SetSpeed(wallCollideVertical[0].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideVertical[i].start);
            pBullet->SetSpeed(wallCollideVertical[1].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideVertical[i].start);
            pBullet->SetSpeed(wallCollideVertical[2].dir);

            pBullet = Instantiate<Bullet>(GetParent());
            pBullet->SetPosition(wallCollideVertical[i].start);
            pBullet->SetSpeed(wallCollideVertical[3].dir);

            coolTime_ = heatAdd_ * 0.5f;
        }

#if 0
        if(false){
            //��]
            //XMVECTOR meesureVec = XMVector3TransformCoord(XMLoadFloat3(&wallCollideVertical[i].dir), matRotateY_R);
            MeasurePole* pMp = Instantiate<MeasurePole>(GetParent());
            pMp->ViewRayCast(&wallCollideVertical[i].start, &wallCollideVertical[i].dir, 10.0f);
            pMp->SetPosition(wallCollideVertical[i].start);
        }
        if (true) {
            //MeasurePole* pMp;
            MeasurePole* pMp1 = Instantiate<MeasurePole>(GetParent());
            pMp1->ViewRayCast(&wallCollideVertical[i].start, &vehicleVector_.x, 10.0f);
            pMp1->SetPosition(wallCollideVertical[i].start);

            MeasurePole* pMp2 = Instantiate<MeasurePole>(GetParent());
            pMp2->ViewRayCast(&wallCollideVertical[i].start, &vehicleVector_.y, 10.0f);
            pMp2->SetPosition(wallCollideVertical[i].start);

            MeasurePole* pMp3 = Instantiate<MeasurePole>(GetParent());
            pMp3->ViewRayCast(&wallCollideVertical[i].start, &vehicleVector_.z, 10.0f);
            pMp3->SetPosition(wallCollideVertical[i].start);
        }
#endif

        Model::RayCast(hModel, &wallCollideVertical[i]);  //���C�𔭎�
        //����������
        if (wallCollideVertical[i].hit)
        { 
            //�������Ƃ̉����x
            float dirAcc;
            //�������Ƃ̎ԑ̂̒���
            float dirSize;
            //�������Ƃ̃v���X�}�C�i�X�F 1 �� -1
            float dirPlusMinus;
            //�������Ƃɑ���@�Ȃ񂩓���邢���Ƃ��Ă����ȋC������
            switch (i)
            {
                //�O���A���̑�
            default:
            case Direction::front:
                dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = Size.toFront_;
                dirPlusMinus = 1.0f;
                break;
                //�E
            case Direction::right:
                dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = Size.toRight_;
                dirPlusMinus = 1.0f;
                break;
                //���
            case Direction::rear:
                dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toRear_;
                dirPlusMinus = -1.0f;
                break;
                //��
            case Direction::left:
                dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toLeft_;
                dirPlusMinus = -1.0f;
                break;
            }

            if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
            {
                //�Փ˂��钼�O�Ŏ~�܂������̕ǂ܂ł̋���
                XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - (dirSize * dirPlusMinus), 0.0f };
                ajustVec = XMVector3TransformCoord(ajustVec, matRot);

                ajustVec = XMVector3TransformCoord(ajustVec, matRotateZ);
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateX);
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);

                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.y += XMVectorGetY(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);


                //���s�ړ�������
                //float accY = XMVectorGetY(acceleration_);
                //acceleration_ = wallCollideVertical[i].parallelism * *XMVector3LengthEst(acceleration_).m128_f32;
                //acceleration_ = XMVectorSetY(acceleration_, accY);

                static float reflectForce = 0.1f;

                float accLen = *XMVector3LengthEst(acceleration_).m128_f32;
                acceleration_ =
                    XMVector3NormalizeEst(wallCollideVertical[i].reflection + wallCollideVertical[i].parallelism)
                    * accLen * reflectForce;
            }
        }
    }
    //�΂�
    std::array<RayCastData, 4>wallCollideOblique;
    for (int i = 0; i < wallCollideOblique.size(); i++)
    {
        wallCollideVertical[i].start = transform_.position_;
        wallCollideVertical[i].start.y += Size.toTop_ * 0.5f;

        enum Oblique
        {
            frontLeft = 0,
            frontRight,
            rearLeft,
            rearRight
        };

        //z������̊p�x
        float theta = 0.0f;
        switch (i){
        default:
        case frontLeft:     theta = acos(Size.toFront_ / Size.toFrontLeft_);
            break;
        case frontRight:    theta = acos(Size.toFront_ / Size.toFrontRight_);
            break;
        case rearLeft:      theta = acos(Size.toRear_ / Size.toFrontLeft_);
            break;
        case rearRight:     theta = acos(Size.toRear_ / Size.toFrontRight_);
            break;
        }

        

    }

#if 0
void Vehicle::CollideWall(int hModel, int type)
{
    //�΂�***
    //z������̊p�x
    float theta = acos(vehicleSizeHalf_.z / vehicleSizeOblique_);

    XMVECTOR rotVec = vehicleVector_.z;

    //�z��ɂ���
    vector<RayCastData> rayCar(4);
    for (int i = 0; i < rayCar.size(); i++)
    {
        rayCar[i].start = transform_.position_;
        rayCar[i].start.y += vehicleSizeHalf_.y;

        //90�x����]
        XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(90 * i));	//�x���ŉ�]������s��    
        rotVec = XMVector3TransformCoord(vehicleVector_.z, mat);	//�x�N�g�����s��ŕό`

        XMStoreFloat3(&rayCar[i].dir, rotVec);

        Model::RayCast(hModel, &rayCar[i]);  //���C�𔭎�
        //���C������������
        if (rayCar[i].hit)
        {
            //����           
            //�t���[�g�ɂ��Ƃ�
            XMFLOAT3 floAcc;
            //�x�N�g��Y���ŉ�]�p�s��
            XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
            XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));

            XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));

            //���ʍs���A�ԗ��𐳂����ʒu�Ƀ��[�v�����A�����x�𐳂����O�ɂ���
            //engineRotate_ = rayCar[0].dist - vehicleSizeHalf_.z;
            if (i == 0 && rayCar[i].dist < floAcc.z + vehicleSizeHalf_.z)
            {
                //�Փ˂��钼�O�Ŏ~�܂������̕ǂ܂ł̋���
                XMVECTOR ajustVec = { 0,0,rayCar[i].dist - vehicleSizeHalf_.z,0 };
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);
                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);

                //���s�ړ�������
                //y���ɑΉ����ĂȂ��̂ł�??
                float accY = XMVectorGetY(acceleration_);
                acceleration_ = rayCar[i].parallelism * *XMVector3Length(acceleration_).m128_f32;
                acceleration_ = XMVectorSetY(acceleration_, accY);
            }

            //�ǂɏՓ�
            //�ړ����x+��b�l
            //
            float hitSpeed = *XMVector3LengthEst(acceleration_).m128_f32 + moveSPD_ * 2;
            if ((i == 0 || i == 2) && rayCar[i].dist < vehicleSizeHalf_.z)
            {
                //acceleration_ = rayCar[i].reflection * hitSpeed * -0.35f;
            }
            //�����炠����ƒᔽ��
            else if ((i == 1 || i == 3) && (rayCar[i].dist < vehicleSizeHalf_.x))
            {
                //acceleration_ += rayCar[i].normal * hitSpeed * -0.25f / sideWheelFriction_;
            }
        }
    }

    //�΂�
    //�z��ɂ���
    vector<RayCastData> rayCarOblique(4);
    for (int i = 0; i < rayCarOblique.size(); i++)
    {
        rayCarOblique[i].start = transform_.position_;
        rayCarOblique[i].start.y += vehicleSizeHalf_.y;
        //90�x(?) ����]
        float angle = theta;//�Ίp�x
        switch (i)
        {
        default:break;
        case 1:
            angle = XM_PI - theta; break;
        case 2:
            angle = XM_PI + theta; break;
        case 3:
            angle = -theta; break;
        }
        XMMATRIX mat = XMMatrixRotationY(angle);	//�x���ŉ�]������s��    
        rotVec = XMVector3TransformCoord(vehicleVector_.z, mat);	//�x�N�g�����s��ŕό`
        XMStoreFloat3(&rayCarOblique[i].dir, rotVec);

        Model::RayCast(hModel, &rayCarOblique[i]);  //���C�𔭎�
        float hitSpeed = *XMVector3LengthEst(acceleration_).m128_f32 + moveSPD_ * 4;
        if (rayCarOblique[i].hit && rayCarOblique[i].dist < vehicleSizeOblique_)
        {
            //pMarker->SetPosition(rayCarOblique[i].end);
            //acceleration_ += rayCarOblique[i].normal * hitSpeed * -0.25;
        }
    }
}
#endif
#if 0
//�O�㍶�E��
for (int i = 0; i < wallCollideVertical.size(); i++)
{
    XMFLOAT3 pos = transform_.position_;
    XMVECTOR up = vehicleVector_.y * Size.toBottom_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, up);
    pos.x += upF.x;
    pos.y += upF.y;
    pos.z += upF.z;

    //wallCollideVertical[i].start = transform_.position_;
    wallCollideVertical[i].start = pos;
    //wallCollideVertical[i].start = Model::GetBonePosition(hModel_, "center");

    //90�x����]
    XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(90.0f * i));	//�x���ŉ�]������s��    
    XMStoreFloat3(&wallCollideVertical[i].dir, XMVector3TransformCoord(vehicleVector_.z, matRot));//�x�N�g�����s��ŕό`

    Model::RayCast(hModel, &wallCollideVertical[i]);  //���C�𔭎�
    //����������
    if (wallCollideVertical[i].hit)
    {
        //�������Ƃ̉����x
        float dirAcc;
        //�������Ƃ̎ԑ̂̒���
        float dirSize;
        //�������Ƃ̃v���X�}�C�i�X�F 1 �� -1
        float dirPlusMinus;
        //�������Ƃɑ���@�Ȃ񂩓���邢���Ƃ��Ă����ȋC������
        switch (i)
        {
            //�O���A���̑�
        default:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = Size.toFront_;
            dirPlusMinus = 1.0f;
            break;
            //�E
        case Direction::right:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = Size.toRight_;
            dirPlusMinus = 1.0f;
            break;
            //���
        case Direction::rear:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = -Size.toRear_;
            dirPlusMinus = -1.0f;
            break;
            //��
        case Direction::left:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
            dirSize = -Size.toLeft_;
            dirPlusMinus = -1.0f;
            break;
        }

        if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
        {
            //�Փ˂��钼�O�Ŏ~�܂������̕ǂ܂ł̋���
            XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - (dirSize * dirPlusMinus), 0.0f };
            ajustVec = XMVector3TransformCoord(ajustVec, matRot);

            ajustVec = XMVector3TransformCoord(ajustVec, matRotateZ);
            ajustVec = XMVector3TransformCoord(ajustVec, matRotateX);
            ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);

            transform_.position_.x += XMVectorGetX(ajustVec);
            transform_.position_.z += XMVectorGetZ(ajustVec);


            //���s�ړ�������
            //float accY = XMVectorGetY(acceleration_);
            //acceleration_ = wallCollideVertical[i].parallelism * *XMVector3LengthEst(acceleration_).m128_f32;
            //acceleration_ = XMVectorSetY(acceleration_, accY);

            static float reflectForce = 0.1f;

            float accLen = *XMVector3LengthEst(acceleration_).m128_f32;
            acceleration_ =
                XMVector3NormalizeEst(wallCollideVertical[i].reflection + wallCollideVertical[i].parallelism)
                * accLen * reflectForce;
        }
    }
}
#endif
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
}

//�n���h���̑���
void Vehicle::HandleTurnLR(int LR)
{
    //handleFlag_ = true;

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
    if(pointCount_ < pointCountMax_ && pointCount_ >= 0)
    {
        XMFLOAT3 pos = pGround_->GetCircuitUnion()->checkPoint_[pointCount_]->GetPosition();
        return &pos;
    }
    else
    {
        XMFLOAT3 pos = pGround_->GetCircuitUnion()->checkPoint_[0]->GetPosition();
        return &pos;
    }
}
//���̃`�F�b�N�|�C���g�܂ł̋������擾
float Vehicle::GetNextCheckDistance()
{
    return *XMVector3LengthEst(XMLoadFloat3(GetNextCheckPosition())).m128_f32
        - *XMVector3LengthEst(XMLoadFloat3(&transform_.position_)).m128_f32;
}

//UI�̊֐��Q�@�v���C���[����ō�p����
//UI�̏�����
void Vehicle::PlayerUI_Initialize()
{
}
//UI�̕\��
void Vehicle::PlayerUI_Draw()
{
}
//UI�̏��X�V
void Vehicle::PlayerUI_Update()
{
}

//�J�����̗p��
void Vehicle::PlayerCamera_Initialize()
{
}

//�G�t�F�N�g��\��
void Vehicle::PlayerParticle()
{
}

//�ԗ��̑���A���͂̎�t
void Vehicle::InputOperate()
{
}

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


#endif
