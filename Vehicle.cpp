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

using std::vector;
using std::string;

//�R���X�g���N�^
Vehicle::Vehicle(GameObject* parent)
    :GameObject(parent, "Vehicle"), hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(wheelFriction_ * 1.0f)
    , slideFlag_(false)
    , sideFriction_(0.2f), sideSlideFriction_(0.1f)
    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(-1), lapCount_(0), lapMax_(1)
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
{
    Size.wheelHeight_ = 0.1f;
}

//�p���p
Vehicle::Vehicle(GameObject* parent, const std::string& name)
    :GameObject(parent, name), hModel_(-1), hGroundModel_(-1), hWheelModel_(-1)
    , acceleration_({ 0.0f, 0.0f, 0.0f, 0.0f })
    , moveSPD_(0.01f), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5f), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10.0f)
    , handleRotate_(0.0f), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(wheelFriction_ * 1.0f)
    , slideFlag_(false)
    , sideFriction_(0.2f), sideSlideFriction_(0.1f)
    , turnAdjust_(0.05f), driveAdjust_(20.0f), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(60.0f)
    , landingFlag_(true)
    , time_(0), goalFlag_(false), pointCount_(-1), lapCount_(0), lapMax_(1)
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
{
    Size.wheelHeight_ = 0.1f;
}

//�f�X�g���N�^
Vehicle::~Vehicle()
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

//������
void Vehicle::Initialize()
{
    hModel_ = Model::Load(vehicleModelName_);
    assert(hModel_ >= 0);
    //ModelInitialize("");

    transform_.position_ = { 0.0f, 0.0f, 0.0f };
    transform_.rotate_ = { 0.0f, 0.0f, 0.0f };

    PlayerCamera_Initialize();
    //Viewer* pViewer = Instantiate<Viewer>(this);
    //pViewer->SetPosition(transform_.position_);

    //�T�C�Y�v�Z
    SetVehicleSize(hModel_);

    //�^�C��
    hWheelModel_ = Model::Load(wheelModelName_);
    assert(hWheelModel_ >= 0);
    MakeWheels(hWheelModel_);

    //�����蔻��
    SphereCollider* collision = new SphereCollider({ 0.0f, Size.toCenter_, 0.0f } , Size.frontToRear_ * 0.5f);
    AddCollider(collision);

    //�G�t�F�N�g�̃|�C���^�Ƃ���̂܂Ƃ�
    pParticle_ = Instantiate<Particle>(this);

    //�X�e�[�W�I�u�W�F�N�g��T��
    pGround_ = (Ground*)FindObject("Ground");
    assert(pGround_ != nullptr);
    //�����ƃp�[�c�������Ă��邩 (����ȍ~�̓|�C���^�������ăp�[�c�����邱�Ƃ�O��ɂ���)
    assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    lapMax_ = pGround_->GetCircuitUnion()->maxLap_;

    //UI������
    PlayerUI_Initialize();
}

//�X�V
void Vehicle::Update()
{
    //�N�[���^�C��
    if (coolTime_ > 0)
        coolTime_--;

    //����
    if (!(goalFlag_))
        time_++;

    //�f�S�[��  
    if (lapCount_ >= lapMax_)
        goalFlag_ = true;

    //���ꂼ��̒l�ɍ��킹�Ď���]������s��
    XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX matRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));

    //���݈ʒu���x�N�g���ɂ��Ă���
    XMVECTOR vecPos = XMLoadFloat3(&transform_.position_);

    //�s���p��
    XMVECTOR vecX = { moveSPD_, 0.0f, 0.0f ,0.0f };
    XMVECTOR vecY = { 0.0f, moveSPD_, 0.0f ,0.0f };
    XMVECTOR vecZ = { 0.0f, 0.0f, moveSPD_ ,0.0f };

    //�x�N�g�� * �s��
    vecZ = XMVector3TransformCoord(vecZ, matRotateY);
    vecX = XMVector3TransformCoord(vecX, matRotateY);

    //�O�����ɐi��ł邩������������
    accZDirection_ = 1;
    if (0 > XMVectorGetZ(XMVector3TransformCoord(acceleration_, XMMatrixRotationY(
        XMConvertToRadians(-transform_.rotate_.y)))))
    {
        accZDirection_ = -1;
    }

    //�n���h���̑���
    handleFlag_ = false;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
    {
        HandleTurnLR(left);
    }
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
    {
        HandleTurnLR(right);
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
            acceleration_ += vecZ;
        }

        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            acceleration_ -= vecZ;
        }
    }

    //����
    if (Input::IsKeyDown(DIK_SPACE))
        slideFlag_ = true;
    if (Input::IsKeyUp(DIK_SPACE))
        slideFlag_ = false;

    //
    if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
    {
        acceleration_ += vecZ;

        XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "rear");
        boosterPos.y += Size.toCenter_;
        ParticlePackage::ActBooster(pParticle_, boosterPos, -vecZ);
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
        vecPos += vecX;
    }
    if (Input::IsKey(DIK_Q))
    {
        vecPos -= vecX;
    }

    //�㏸
    if (Input::IsKeyDown(DIK_M))
    {
        if (landingFlag_ || true)
        {
            acceleration_ += {0.0f,jumpForce_,0.0f,0.0f};
        }
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

    //�x�N�g�������x ���C��R�@�����x���ς���
    if (slideFlag_)
    {
        //acceleration_ *= {airFriction_, 1, airFriction_, 1};
        acceleration_ *= {wheelFriction_, 1.0f, wheelFriction_, 1.0f};
    }
    else
    {
        acceleration_ *= {wheelFriction_, 1.0f, wheelFriction_, 1.0f};
    }

    //�Ő����
    if (landingType_ == Ground::turf)
    {
        acceleration_ *= {turfFriction_, 1.0f, turfFriction_, 1.0f};
    }

    //���ʂ̍s���p��
    frontVec_ = XMVector3NormalizeEst(vecZ);

    float accLength = *XMVector3LengthEst(acceleration_).m128_f32;
    //�n��Ȃ�n���h���ɍ��킹�ĉ�]
    if (landingFlag_)
    {
        //�O��ɓ����Ă���Ȃ�
        transform_.rotate_.y += handleRotate_ * accLength * turnAdjust_ * accZDirection_;
        handleRotate_ *= (driveAdjust_ / (accLength + driveAdjust_));
    }
    //�n���h����߂�
    if (handleFlag_ == false)
        handleRotate_ *= handleAdjust_;

    //�^�C���̊p�x�ƌ���
    //���ʂ�^�C���̕����ւ͌������Ȃ����A�^�C���̕����ƕ��s�̂ق������ւ͌�������
    //�^�C���̕����ƕ��s�Ȃ牽�����Ȃ����A�����ɋ߂��قǃ^�C���̕����Ƀx�N�g���𔭐�������
    if (landingFlag_)
    {
        XMVECTOR normalAcc = XMVector3Normalize(acceleration_ * XMVECTOR{ 1.0f,0.0f,1.0f,1.0f });     //�ꉞ�c���𖳎�  ���K��
        //�i�s�����ƁA�ԗ��̐��ʂ́A�p�x
        //����͐��x���]���ɂ��č����炵���B�p�x��0 ~ 180 �̊ԂȂ̂łȂ�Ƃ������𔻕ʂ�����
        //XMVector3NormalizeEst �͒������O���ƃo�O����ۂ�
        /*
        https://www.hiramine.com/programming/graphics/2d_vectorangle.html
        �Q�����̂Q�̃x�N�g���i�x�N�g���`�A�x�N�g���a�j�̊O�� (OA �~ OB) �����߂��Ƃ��A�O�ς̒l���A
        ���̒l�̂Ƃ��A�x�N�g���a�́A�x�N�g���`�̐i�s�����ɑ΂��āA�����������Ă���A
        ���̒l�̂Ƃ��A�x�N�g���a�́A�x�N�g���`�̐i�s�����ɑ΂��āA�E���������Ă���A�Ƃ�������������܂��B
        */

        //������y���������+�A�E���Ɖ�����-
        XMVECTOR outerProduct = XMVector3Cross(frontVec_, normalAcc);
        //��]���ė��p����
        XMMATRIX MatRotateZ = XMMatrixRotationZ(XMConvertToRadians(90));
        outerProduct = XMVector3TransformCoord(outerProduct, MatRotateZ);
        //�O�ς���]
        outerProduct = XMVector3TransformCoord(outerProduct, matRotateY);

        /*
        * �x���Ƃ��͉��S�͂��キ�A�����Ƃ��͋����A���C�̉e�����󂯂�
        * 
        * ��肽�����ƁF�Ԃ̑O��ւ͒�R�����Ȃ����A���E�����ւ̒�R�͑����B
        *               ���E���牟���ꂽ�ꍇ(�J�[�u�Ȃ�)�͒҂܍��킹�ŉ�����Ă��锽�Ε����։����Ԃ��B
        *               ����͉�����(�Ԃ̍��E�����̗�)�������قǖ��C�������Ďア�͂ŉ����Ԃ�
        * �@���F�����؂�ւ������ɍ��E�ɓ���
        */

        float ajust = sideFriction_;
        if (slideFlag_)
            ajust = sideSlideFriction_;

        acceleration_ += (*XMVector3LengthEst(acceleration_).m128_f32) * outerProduct * ajust;
    }

    //�����̒���
    SpeedLimit(acceleration_, speedLimit_);

    //�ʒu�x�N�g���@�{�@�����x�x�N�g��
    vecPos += acceleration_;

    //�x�N�g�����ʒu�ɓ����
    XMStoreFloat3(&transform_.position_, vecPos);

    //�ڒn�A�ǏՓ�
    VehicleCollide();

    //�^�C���̒l�Z�b�g
    pWheels_->SetWheelSpeedRotate(
        *XMVector3LengthEst(acceleration_).m128_f32 * wheelSpeedAdd_ * accZDirection_, handleRotate_);

    //�X�s�[�h���[�^�[
    PlayerUI_Update();
#if 0
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    pSpeedometer->SetSpeed(*XMVector3LengthEst(speedVec).m128_f32 * 120);
#endif

#ifdef _DEBUG
    //�e�X�g ���ʂɃ}�[�J�[��ݒu
    XMVECTOR vecMark = { 0, 10, -1, 0 };    //���ɐL�т�x�N�g����p��
    vecMark = XMVector3TransformCoord(vecMark, matRotateX);        //������Ԃ̌����ɍ��킹�ĉ�]
    vecMark = XMVector3TransformCoord(vecMark, matRotateY);        //������Ԃ̌����ɍ��킹�ĉ�]2
    vecMark = XMVector3TransformCoord(vecMark, matRotateZ);        //������Ԃ̌����ɍ��킹�ĉ�]3

    XMVECTOR rearVec = vecMark + vecPos;
    XMVECTOR frontVec = { transform_.position_.x,transform_.position_.y + 10,transform_.position_.z,0 };
    XMVECTOR bulletVec = frontVec - rearVec;
    //�e�X�g
    bulletVec = XMVector3Normalize(frontVec_);

    //�e�𔭎�
    if (Input::IsMouseButton(0) && coolTime_ <= 0)
    {
        Bullet* pBullet = Instantiate<Bullet>(GetParent());
        XMFLOAT3 BulletPos = transform_.position_;
        BulletPos.y += Size.toCenter_;
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
    //���������Ƃ��̏���
    if (pTarget->GetObjectName() == "CheckPoint")
    {
        CheckPoint* pCP = (CheckPoint*)pTarget;

        if (pCP->GetNumber() - 1 == pointCount_)
        {
            pointCount_++;
        }
        if (pCP->GetNumber() == 1 && pointCount_ == pGround_->GetCircuitUnion()->checkPointPosition_.size())
        {
            pointCount_ = 0;
            lapCount_++;
        }   
    }
}

//�`��
void Vehicle::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    PlayerUI_Draw();
#if 0
    if (goalFlag_)
    {
        Transform imageTrans;
        imageTrans.position_ = XMFLOAT3(1.0f, 1.0f, 0.0f);
        imageTrans.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

        Image::SetRect(hImage_, 0, 0, 1024, 128);
        Image::SetTransform(hImage_, imageTrans);
        Image::Draw(hImage_);
    }
#endif
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


//�n�ʁA�ǁA�G�Ƃ̏Փ˂��܂Ƃ߂�H�H
void Vehicle::VehicleCollide()
{
    //��ނ̕�����
    for (int i = 0; i < pGround_->GetCircuitUnion()->parts_.size(); i++)
    {
        //�n��
        Landing(pGround_->GetCircuitUnion()->parts_[i].model_, pGround_->GetCircuitUnion()->parts_[i].type_);
        
        //��
        CollideWall(pGround_->GetCircuitUnion()->parts_[i].model_, pGround_->GetCircuitUnion()->parts_[i].type_);
    }
}

//�ڒn 
void Vehicle::Landing(int hModel,int type)
{
    RayCastData data;
    data.start = transform_.position_;  //���C�̔��ˈʒu
    data.start.y -= Size.wheelHeight_;  //�n�ʂɒ���t���Ă�Ƃ��܂������Ȃ� ���̒l�Ԃ񂾂��n�ʂ��畂���@�^�C���̍����ɂ���
    data.dir = { 0.0f, -1.0f, 0.0f };      //���C�̕���
    Model::RayCast(hModel, &data);      //���C�𔭎�
        
    //���C������������
    if (data.hit)
    {
        if (type == Ground::turf)
            landingType_ = Ground::turf;
        if (type == Ground::road)
            landingType_ = Ground::road;

        if (-data.dist > XMVectorGetY(acceleration_) - gravity_)
        {
            //�������̉����x���傫���Ȃ�@�n�ʂɃ��[�v�@�������x���O
            transform_.position_.y -= data.dist;
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
            landingFlag_ = true;
        }
        else
        {
            //�ʒu��������
            acceleration_ -= {0.0f, gravity_, 0.0f, 0.0f};
            landingFlag_ = false;
        }
    }

    //�V��
    data.dir = { 0.0f, 1.0f, 0.0f };       //�^��ɔ���
    Model::RayCast(hModel, &data);  //���C�𔭎�

    if (data.hit)
    {   
        //������ƒn�ʂɖ��܂����Ƃ�
        if (data.dist < Size.toTop_)
        {
            //���̕��ʒu���グ��
            transform_.position_.y += data.dist;
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
        //�V��ɂԂ������Ƃ�
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            //���̕��ʒu���グ��
            transform_.position_.y += data.dist - Size.toTop_;
            acceleration_ *= {1.0f, 0.0f, 1.0f, 1.0f};
        }
    }
}

void Vehicle::CollideWall(int hModel, int type)
{
    enum
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    } direction;

    //�O�㍶�E�Ǝ΂߂ŕ������邱�Ƃɂ���
    std::array<RayCastData, 4>wallCollideVertical;

    //�O�㍶�E��
    for (int i = 0; i < wallCollideVertical.size(); i++)
    {
        wallCollideVertical[i].start = transform_.position_;
        wallCollideVertical[i].start.y += Size.toCenter_;

        //90�x����]
        XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(90 * i));	//�x���ŉ�]������s��    
        XMStoreFloat3(&wallCollideVertical[i].dir, XMVector3TransformCoord(frontVec_, matRot));//�x�N�g�����s��ŕό`

        Model::RayCast(hModel, &wallCollideVertical[i]);  //���C�𔭎�
        //����������
        if (wallCollideVertical[i].hit)
        { 
            XMFLOAT3 floAcc;
            //�x�N�g��Y���ŉ�]�p�s��
            XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
            XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));

            XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));

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
            case right:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = Size.toRight_;
                dirPlusMinus = 1.0f;
                break;
                //���
            case rear:dirAcc = XMVectorGetZ(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toRear_;
                dirPlusMinus = -1.0f;
                break;
                //��
            case left:dirAcc = XMVectorGetX(XMVector3TransformCoord(acceleration_, matRotateY_R));
                dirSize = -Size.toLeft_;
                dirPlusMinus = -1.0f;
                break;
            }

            if (wallCollideVertical[i].dist < (dirAcc + dirSize) * dirPlusMinus)
            {
                //�Փ˂��钼�O�Ŏ~�܂������̕ǂ܂ł̋���
                XMVECTOR ajustVec = { 0.0f, 0.0f, wallCollideVertical[i].dist - (dirSize * dirPlusMinus), 0.0f };
                ajustVec = XMVector3TransformCoord(ajustVec, matRot);
                ajustVec = XMVector3TransformCoord(ajustVec, matRotateY);
                transform_.position_.x += XMVectorGetX(ajustVec);
                transform_.position_.z += XMVectorGetZ(ajustVec);

                //���s�ړ�������
                float accY = XMVectorGetY(acceleration_);
                acceleration_ = wallCollideVertical[i].parallelism * *XMVector3LengthEst(acceleration_).m128_f32;
                acceleration_ = XMVectorSetY(acceleration_, accY);
            }
        }
    }
#if 0
void Vehicle::CollideWall(int hModel, int type)
{
    //�΂�***
    //z������̊p�x
    float theta = acos(vehicleSizeHalf_.z / vehicleSizeOblique_);

    XMVECTOR rotVec = frontVec_;

    //�z��ɂ���
    vector<RayCastData> rayCar(4);
    for (int i = 0; i < rayCar.size(); i++)
    {
        rayCar[i].start = transform_.position_;
        rayCar[i].start.y += vehicleSizeHalf_.y;

        //90�x����]
        XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(90 * i));	//�x���ŉ�]������s��    
        rotVec = XMVector3TransformCoord(frontVec_, mat);	//�x�N�g�����s��ŕό`

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
                //acceleration_ += rayCar[i].normal * hitSpeed * -0.25f / sideFriction_;
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
        rotVec = XMVector3TransformCoord(frontVec_, mat);	//�x�N�g�����s��ŕό`
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
    XMFLOAT3 vehicleBottom  = Model::GetBonePosition(hModel, "bottom");
    XMFLOAT3 vehicleTop     = Model::GetBonePosition(hModel, "top");
    XMFLOAT3 vehicleLeft    = Model::GetBonePosition(hModel, "left");
    XMFLOAT3 vehicleRight   = Model::GetBonePosition(hModel, "right");
    XMFLOAT3 vehicleRear    = Model::GetBonePosition(hModel, "rear");
    XMFLOAT3 vehicleFront   = Model::GetBonePosition(hModel, "front");
    Size.wheelFR_ = Model::GetBonePosition(hModel, "wheelFR");
    Size.wheelFL_ = Model::GetBonePosition(hModel, "wheelFL");
    Size.wheelRR_ = Model::GetBonePosition(hModel, "wheelRR");
    Size.wheelRL_ = Model::GetBonePosition(hModel, "wheelRL");
    //�ԗ��̑傫���v�Z
    Size.toRight_   = *XMVector3Length(XMLoadFloat3(&vehicleRight)).m128_f32;
    Size.toLeft_    = *XMVector3Length(XMLoadFloat3(&vehicleLeft)).m128_f32;
    Size.toFront_   = *XMVector3Length(XMLoadFloat3(&vehicleFront)).m128_f32;
    Size.toRear_    = *XMVector3Length(XMLoadFloat3(&vehicleRear)).m128_f32;
    Size.toTop_     = *XMVector3Length(XMLoadFloat3(&vehicleTop)).m128_f32;
    Size.toCenter_  = Size.toTop_ * 0.5f;
    Size.rightToLeft_   = Size.toRight_ + Size.toLeft_;
    Size.frontToRear_   = Size.toFront_ + Size.toRear_;
    Size.toFrontRight_  = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toRight_ * Size.toRight_));
    Size.toFrontLeft_   = sqrt((Size.toFront_ * Size.toFront_)  + (Size.toLeft_ * Size.toLeft_));
    Size.toRearRight_   = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toRight_ * Size.toRight_));
    Size.toRearLeft_    = sqrt((Size.toRear_ * Size.toRear_)    + (Size.toLeft_ * Size.toLeft_));
}

//�n���h���̑���
void Vehicle::HandleTurnLR(int LR)
{
    handleFlag_ = true;

    //�����Ă�ƋȂ���₷��
    if (slideFlag_)
        handleRotate_ += rotateSPD_ * slideHandleRotateAdd_ * LR;
    else
        handleRotate_ += rotateSPD_ * LR;
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