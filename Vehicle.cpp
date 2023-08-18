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
    , slopeLimitAngle_(45.0f), wallReflectionForce_(0.99f)
    , handleRight_(1), handleLeft_(-1)
    //�u�[�X�g
    , boostCapacityMax_(2000.0), boostCapacity_(boostCapacityMax_)
    , boostSpending_(1.0f), boostIncrease_(boostSpending_ * 0.5f)
    , isPlayer_(false)
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
#if 0
    BoundingBox abc;
    abc.Center = { 1.f,1.f,1.f };
    abc.Extents = { 1.f,1.f,1.f };
    BoundingBox def;
    abc.Center = { 1.f,1.f,1.f };
    abc.Extents = { 1.f,1.f,1.f };
    int ans;
    ans = abc.Contains(def);
    ans = def.Contains(abc);
    //BoundingBox::Contains(constBoundingOrientedBox&)
    BoundingOrientedBox bob1;
    bob1.Center = transform_.position_;
    bob1.Extents = { Size.toLeft_ * 2.f, Size.toTop_ * 2.f, Size.toFront_ * 2.f };
    bob1.Orientation;
    XMVECTOR qua = XMQuaternionRotationRollPitchYaw(transform_.rotate_.x
                                                  , transform_.rotate_.y
                                                  , transform_.rotate_.z);
    XMStoreFloat4(&bob1.Orientation, qua);
    BoundingOrientedBox bob2 = bob1;
    ans = bob1.Contains(bob2);
    ans = bob2.Contains(bob1);
#endif

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

    //���Z�b�g
    Operation.Refresh();
    //�������
    InputOperate();

    //�n���h���̑���
    handleFlag_ = false;
    if (Operation.inputNow[Operation.inputName::handleLeft])
    {
        HandleTurnLR(handleLeft_);
    }
    if (Operation.inputNow[Operation.inputName::handleRight])
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
        if (Operation.inputNow[Operation.inputName::moveFront])
        {
            acceleration_ += vecZ * GroundTypeFriction_[landingType_].acceleration;
        }

        if (Operation.inputNow[Operation.inputName::moveRear])
        {
            acceleration_ -= vecZ * GroundTypeFriction_[landingType_].acceleration;
        }
    }

    //�u�[�X�g
    bool increaseFlag = false;  //�e�ʂ������邩

    if (Operation.inputNow[Operation.inputName::boost])
    {
        //�e�ʂ�����Ȃ�
        if (boostCapacity_ >= boostSpending_)
        {
            //����
            boostCapacity_ -= boostSpending_;

            slideFlag_ = true;

            acceleration_ += vecZ * 2.0f;

            XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "rear");
            ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
        }
        else
        {
            increaseFlag = true;
        }
    }
    else
    {
        slideFlag_ = false;

        increaseFlag = true;

        //���ӂ��
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
    }
    //������
    if (landingFlag_ && increaseFlag)
    {
        boostCapacity_ += boostIncrease_;
    }


#ifdef _DEBUG
    //���E
    if (Operation.inputNow[Operation.inputName::turnLeft])
    {
        transform_.rotate_.y -= rotateSPD_;
    }
    if (Operation.inputNow[Operation.inputName::turnRight])
    {
        transform_.rotate_.y += rotateSPD_;
    }

    //�W�����v
    //if (Operation.inputNow[Operation.inputName::jump])
    if(Operation.IsDown(Operation.inputName::jump))
    {
        acceleration_ += {0.0f, jumpForce_, 0.0f, 0.0f};
        landingFlag_ = false;
    }

    if (Operation.inputNow[Operation.inputName::moveLeft])
    {
        acceleration_ -= vecX;
    }
    if (Operation.inputNow[Operation.inputName::moveRight])
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

    //�J�����X�V
    PlayerCamera_Update();

    //�X�s�[�h���[�^�[
    PlayerUI_Update();

    //�G�t�F�N�g
    PlayerParticle();


    //�T�E���h�f�[�^�̃��[�h
    hSound_ = Audio::Load("music\\loop100201.wav");
    assert(hSound_ >= 0);
    if (Input::IsKeyDown(DIK_J))
    {
        Audio::Play(hSound_);
    }
    if (Input::IsKeyDown(DIK_K))
    {
        Audio::Stop(hSound_);
    }
    if (Input::IsKeyDown(DIK_L))
    {
        Audio::Pause(hSound_);
    }

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
}

//�^�C����]
void Vehicle::TurnWheel()
{
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
    
    //�ǂ���񂾂�������H
    bool isHitWall = false;

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
    //���C�̔��ˈʒu
    RayCastData data;
    data.start = transform_.position_;
    XMFLOAT3 upF;
    XMStoreFloat3(&upF, vehicleVector_.y * Size.toBottom_);
    data.start = Transform::Float3Add(data.start, upF);

    data.dir = { 0.0f, -1.0f, 0.0f };   //���C�̕���
    Model::RayCast(hModel, &data);      //���C�𔭎�

    bool isHit = false; //�����ɓ���������??

    //���C������������
    if (data.hit)
    {
        //NPC�p
        if (!isPlayer_){
            rayCastHit_[RayCastHit::Number::down].dir = data.dir;
            rayCastHit_[RayCastHit::Number::down].dist = data.dist;
            rayCastHit_[RayCastHit::Number::down].end = data.end;
            rayCastHit_[RayCastHit::Number::down].hit = data.hit;
        }

        landingType_ = type;    //�n�ʂ̃^�C�v
        isHit = true;           //�����ɓ�������

        if (-data.dist > XMVectorGetY(acceleration_) - Size.toWheelBottom_)
        {
            //�������̉����x���傫���Ȃ�@�n�ʂɃ��[�v�@�������x���O
            if (!landingFlag_)
            {
                transform_.position_.y -= data.dist - Size.toWheelBottom_;
                acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
                landingFlag_ = true;
            }
        }
        else
        {
            //���� �����ɂ���Ƃ��܂�����
            if (!landingFlag_)
            {
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
            }

            landingFlag_ = false;
        }

        //�p�x��ς���
        if (landingFlag_ || data.dist < Size.toWheelBottom_ + Size.wheelHeight_)
        {
            //��]
            if (VehicleRotateSlope(data.normal, slopeLimitAngle_) == false)
            {
                //�⓹�Ȃ痎��
                acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
            }
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
        isHit = true;           //�����ɓ�������

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
        if (!isPlayer_){
            rayCastHit_[RayCastHit::Number::up].dir = data.dir;
            rayCastHit_[RayCastHit::Number::up].dist = data.dist;
            rayCastHit_[RayCastHit::Number::up].end = data.end;
            rayCastHit_[RayCastHit::Number::up].hit = data.hit;
        }

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
            //�~�߂�
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
    }

    return isHit;
}

bool Vehicle::CollideWall(int hModel, int type)
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
            if (!isPlayer_) {
                int number = 0;
                switch (i) {
                default:
                case Direction::front: number = RayCastHit::Number::front; break;    //�O���A���̑�
                case Direction::right: number = RayCastHit::Number::right; break;    //�E
                case Direction::rear:  number = RayCastHit::Number::rear;  break;    //���
                case Direction::left:  number = RayCastHit::Number::left;  break;    //��
                }
                rayCastHit_[number].dir = wallCollideVertical[i].dir;
                rayCastHit_[number].dist = wallCollideVertical[i].dist;
                rayCastHit_[number].end = wallCollideVertical[i].end;
                rayCastHit_[number].hit = wallCollideVertical[i].hit;
            }

            //�������Ƃ̉����x
            float dirAcc;
            //�������Ƃ̎ԑ̂̒���
            float dirSize;

            //�����x����]
            XMVECTOR accRotVec = acceleration_;
            VectorRotateMatrixZXY(accRotVec);

            //�������Ƃɑ���@�Ȃ񂩓���邢���Ƃ��Ă����ȋC������
            switch (i){             
            default://�O���A���̑�
            case Direction::front:
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toFront_; break;
            case Direction::right://�E
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toRight_; break;                
            case Direction::rear://���
                dirAcc = XMVectorGetZ(accRotVec);
                dirSize = Size.toRear_; break;                
            case Direction::left://��
                dirAcc = XMVectorGetX(accRotVec);
                dirSize = Size.toLeft_; break;
            }

            if (wallCollideVertical[i].dist < abs(dirAcc) + dirSize)
            {
                //�U == �ǂɏՓ� / �^ == �⓹
                //��]
                if (VehicleRotateSlope(wallCollideVertical[i].normal, slopeLimitAngle_) == false)
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
            if (!(isPlayer_)) {
                rayCastHit_[NPC_Number].dir = wallCollideOblique[i].dir;
                rayCastHit_[NPC_Number].dist = wallCollideOblique[i].dist;
                rayCastHit_[NPC_Number].end = wallCollideOblique[i].end;
                rayCastHit_[NPC_Number].hit = wallCollideOblique[i].hit;
            }

            //�����Ɖ����x�̔���
            if (wallCollideOblique[i].dist * wallCollideOblique[i].dist <
                (dirAccX * dirAccX + dirAccZ * dirAccZ) + (vehicleLen * vehicleLen))
            {
                if (VehicleRotateSlope(wallCollideOblique[i].normal, slopeLimitAngle_) == false)
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

    return true;
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

//�x�N�g������]�s��ŉ�] (ZXY��)
void Vehicle::VectorRotateMatrixZXY(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateZ);
    vec = XMVector3TransformCoord(vec, matRotateX);
    vec = XMVector3TransformCoord(vec, matRotateY);
}

//�x�N�g������]�s���"�t"��] (ZXY��)
void Vehicle::VectorRotateMatrixZXY_R(XMVECTOR& vec)
{
    vec = XMVector3TransformCoord(vec, matRotateY_R);
    vec = XMVector3TransformCoord(vec, matRotateX_R);
    vec = XMVector3TransformCoord(vec, matRotateZ_R);
}

// �⓹�ɉ����Ďԗ�����](X�AZ��)
bool Vehicle::VehicleRotateSlope(const XMVECTOR& normal, const float limitAngle)
{
    //�@����Y���𔽓]
    XMVECTOR normalR = normal * XMVECTOR{ 1.0f,-1.0f,1.0f,1.0f };

    //�p�x���w��p�x���}���Ƃ�߂�
    //�p�x�v�Z�͓�
    float e = XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32);

    if (XMConvertToDegrees(*XMVector3AngleBetweenNormals(worldVector_.y, normalR).m128_f32) > limitAngle)
    {
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
        return false;
    }

    return true;
}

//�n�ʁA�ǂ̎ԗ��̎p���𓝍�����
void Vehicle::VehicleRotateTotal(std::vector<XMFLOAT3>* rotate)
{
    ;
}

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
    //�ԗ��̑���A���͂̎�t
    void Vehicle::InputOperate() {};
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


#endif
