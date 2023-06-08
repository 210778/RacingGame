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
    :GameObject(parent, "Vehicle"), hModel_(-1), hGroundModel_(-1)
    , acceleration_({ 0, 0, 0, 0 })
    , moveSPD_(0.01f/*0.05f*/), rotateSPD_(1.0f), jumpForce_(1.0f)
    , coolTime_(0), bulletPower_(0.5), heatAdd_(10)
    , gravity_(0.03f), speedLimit_(10), wheelSpeed_(0), wheelDirection_({ 0, 0, 0, 0 })
    , handleRotate_(0), slideHandleAngleLimitAdd_(1.1f)
    , wheelFriction_(0.99f), airFriction_(0.985f), turfFriction_(wheelFriction_ * 1.0f)
    , slideFlag_(false)
    , sideFriction_(0.2f), sideSlideFriction_(0.1f)
    , rayStartHeight(1.0f)
    , turnAdjust_(0.05f), driveAdjust_(20), handleAdjust_(0.95f)
    , slideHandleRotateAdd_(2.0f)
    , handleFlag_(false), handleRotateMax_(/*70*/60)
    , landingFlag_(true)
    , pTextSpeed_(nullptr), pTextTime_(nullptr), pTextLap_(nullptr), pSpeedometer(nullptr), pTextEngine_(nullptr)
    , time_(0), goalFlag_(false), pointCount_(-1), lapCount_(0), lapMax_(1)
    , hImage_(-1)
    , mass_(1.0f), engineRotate_(0.0f)
    , frontVec_({ 0.0, 0.0, 0.0, 0.0 })
    , landingType_(Ground::road)
    , pParticle_(nullptr)
    , pGround_(nullptr)
    , pWheels_(nullptr), wheelSpeedAdd_(20.0f)
    , accZDirection_(1)
    , wheelParticleLength_(0.1f)
{
    Size.wheelHeight_ = 0.1;
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
    hModel_ = Model::Load("model\\Car1_blue.fbx");
    assert(hModel_ >= 0);

    hImage_ = Image::Load("image\\clearImage.png");
    assert(hImage_ >= 0);

    transform_.position_ = XMFLOAT3(0.0f, 0.0f, -200.0f);
    transform_.rotate_   = XMFLOAT3(0.0f, 0.0f, 0.0f);

    Viewer* pViewer = Instantiate<Viewer>(this);
    pViewer->SetPosition(transform_.position_);

    //�T�C�Y�v�Z
    SetVehicleSize(hModel_, "car1");

    //�^�C��
    int wheelModel = Model::Load("model\\wheel1.fbx");
    assert(wheelModel >= 0);
    MakeWheels(wheelModel);

    //�����蔻��
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, Size.toCenter_, 0), Size.frontToRear_ * 0.5f);
    AddCollider(collision);

    //�G�t�F�N�g�̃|�C���^�Ƃ���̂܂Ƃ�
    pParticle_ = Instantiate<Particle>(this);

    //�X�e�[�W�I�u�W�F�N�g��T��
    pGround_ = (Ground*)FindObject("Ground");
    assert(pGround_ != nullptr);
    //�����ƃp�[�c�������Ă��邩 (����ȍ~�̓|�C���^�������ăp�[�c�����邱�Ƃ�O��ɂ���)
    assert(!(pGround_->GetCircuitUnion()->parts_.empty()));

    lapMax_ = pGround_->GetCircuitUnion()->maxLap_;

    //����
    pTextSpeed_ = new Text;
    pTextSpeed_->Initialize();

    pTextTime_ = new Text;
    pTextTime_->Initialize();

    pTextLap_ = new Text;
    pTextLap_->Initialize();

    pTextEngine_ = new Text;
    pTextEngine_->Initialize();

    //�X�s�[�h���[�^�[
    pSpeedometer = Instantiate<Speedometer>(GetParent());
}

//�X�V
void Vehicle::Update()
{
    //�S�[��������
    if (goalFlag_)
    {
        ParticlePackage::ActRainbowFire(pParticle_, transform_.position_);
    }

    //���s���̃^�C���̋O��
    if (wheelParticleLength_ < *XMVector3Length(acceleration_).m128_f32)
    {
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "car1_wheelRR"));
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "car1_wheelRL"));
    }

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
    XMVECTOR vecX = { moveSPD_, 0, 0 ,0 };
    XMVECTOR vecY = { 0, moveSPD_, 0 ,0 };
    XMVECTOR vecZ = { 0, 0, moveSPD_ ,0 };

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
    if (handleRotate_ != 0)
    {
        //�Ȃ���₷��
        if (slideFlag_)
            AngleLimit(handleRotate_, handleRotateMax_ * slideHandleAngleLimitAdd_);
        else
            AngleLimit(handleRotate_, handleRotateMax_);
    }

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

        XMFLOAT3 boosterPos = Model::GetBonePosition(hModel_, "car1_rear");
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
        vecPos += vecX * 30;
    }
    if (Input::IsKey(DIK_Q))
    {
        vecPos -= vecX * 30;
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
        acceleration_ -= vecX * 2;
    }
    if (Input::IsKey(DIK_B))
    {
        acceleration_ += vecX * 2;
    }
#endif

    //�x�N�g�������x ���C��R�@�����x���ς���
    if (slideFlag_)
    {
        //acceleration_ *= {airFriction_, 1, airFriction_, 1};
        acceleration_ *= {wheelFriction_, 1, wheelFriction_, 1};
    }
    else
    {
        acceleration_ *= {wheelFriction_, 1, wheelFriction_, 1};
    }

    //�Ő����
    if (landingType_ == Ground::turf)
    {
        acceleration_ *= {turfFriction_, 1, turfFriction_, 1};
    }

    //�n���h���Ɛ���
    XMMATRIX matHandleRotate = XMMatrixRotationY(XMConvertToRadians(handleRotate_));
    //���ʂ̍s���p��
    XMVECTOR vecFront = XMVector3NormalizeEst(vecZ);
    vecFront = XMVector3NormalizeEst(vecFront);//���K��
    frontVec_ = vecFront;

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
        XMVECTOR normalAcc = XMVector3Normalize(acceleration_);//���K��
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
        XMVECTOR outerProduct = XMVector3Cross(vecFront, normalAcc);
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

    //���ʂ̃x�N�g�����n���h���̊p�x�ŉ�]
    XMMATRIX handleMatRotate = XMMatrixRotationY(XMConvertToRadians(handleRotate_));

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
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    pSpeedometer->SetSpeed(*XMVector3LengthEst(speedVec).m128_f32 * 120);

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
    bulletVec = XMVector3Normalize(vecFront);

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

    //�����\��
    //���ʂ̃x�N�g���̒���
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    string speedStr = std::to_string((float)(*XMVector3LengthEst(speedVec).m128_f32 * 120));
    speedStr += "km/h";
    pTextSpeed_->Draw(30, 30, speedStr.c_str());

    //�o�ߎ��ԕ\��
    int min = 0;
    int sec = 0;
    int mSec = 0;
    int rest = 0;
    min = time_ / 3600;
    rest = time_ % 3600;
    sec = rest / 60;
    rest = rest % 60;
    mSec = rest;
    string timeStr = std::to_string(min) + ":" + std::to_string(sec) + ":" + std::to_string(mSec);
    pTextTime_->Draw(30, 70, timeStr.c_str());

    //���񐔕\��
    string lapStr = std::to_string(lapCount_);
    lapStr += "/";
    lapStr += std::to_string(lapMax_);
    pTextLap_->Draw(30, 110, lapStr.c_str());

    //�G���W���\��
    XMFLOAT3 floAcc;
    //�x�N�g��Y���ŉ�]�p�s��
    XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
    XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));
    //string engineStr = std::to_string(engineRotate_);
    //string engineStr = std::to_string((int)transform_.rotate_.y);
    string engineStr = std::to_string(floAcc.x) + "/" + std::to_string(floAcc.y) + "/" + std::to_string(floAcc.z);

    //engineStr += "C'";//engineStr += "rpm";
    pTextEngine_->Draw(280, 30, engineStr.c_str());

    if (goalFlag_)
    {
        Transform imageTrans;
        imageTrans.position_ = XMFLOAT3(1.0f, 1.0f, 0.0f);
        imageTrans.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

        Image::SetRect(hImage_, 0, 0, 1024, 128);
        Image::SetTransform(hImage_, imageTrans);
        Image::Draw(hImage_);
    }

}

//�J��
void Vehicle::Release()
{
    pTextSpeed_->Release();
    pTextTime_->Release();
    pTextLap_->Release();
    pTextEngine_->Release();
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
    data.dir = XMFLOAT3(0, -1, 0);      //���C�̕���
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
    data.dir = XMFLOAT3(0, 1, 0);       //�^��ɔ���
    Model::RayCast(hModel, &data);  //���C�𔭎�

    if (data.hit)
    {   
        //������ƒn�ʂɖ��܂����Ƃ�
        if (data.dist < Size.toTop_)
        {
            //���̕��ʒu���グ��
            transform_.position_.y += data.dist;
            acceleration_ *= {1, 0, 1, 1};
        }
        //�V��ɂԂ������Ƃ�
        else if (data.dist < XMVectorGetY(acceleration_) + Size.toTop_)
        {
            //���̕��ʒu���グ��
            transform_.position_.y += data.dist - Size.toTop_;
            acceleration_ *= {1, 0, 1, 1};
        }
    }

    //�ڒn�\��
    if (landingFlag_)
    {
        transform_.scale_.y = 1;
    }
    else
    {
        transform_.scale_.y = 1.3f;
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

}

void Vehicle::MakeWheels(int hModel)
{
    //�^�C��
    pWheels_ = Instantiate<VehicleWheel>(this);
    pWheels_->SetVehicleWheel(this, hModel, Size.wheelFL_, Size.wheelFR_, Size.wheelRL_, Size.wheelRR_);
}

void Vehicle::SetVehicleSize(int hModel, std::string modelName)
{
    //�{�[��
    XMFLOAT3 vehicleBottom  = Model::GetBonePosition(hModel, modelName + "_bottom");
    XMFLOAT3 vehicleTop     = Model::GetBonePosition(hModel, modelName + "_top");
    XMFLOAT3 vehicleLeft    = Model::GetBonePosition(hModel, modelName + "_left");
    XMFLOAT3 vehicleRight   = Model::GetBonePosition(hModel, modelName + "_right");
    XMFLOAT3 vehicleRear    = Model::GetBonePosition(hModel, modelName + "_rear");
    XMFLOAT3 vehicleFront   = Model::GetBonePosition(hModel, modelName + "_front");
    Size.wheelFR_ = Model::GetBonePosition(hModel, modelName + "_wheelFR");
    Size.wheelFL_ = Model::GetBonePosition(hModel, modelName + "_wheelFL");
    Size.wheelRR_ = Model::GetBonePosition(hModel, modelName + "_wheelRR");
    Size.wheelRL_ = Model::GetBonePosition(hModel, modelName + "_wheelRL");
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

#if 0
//������
void TestScene::Initialize()
{
    Camera::SetPosition(XMFLOAT3(0, 10, -20));
    Camera::SetTarget(XMFLOAT3(0, 0, 0));
    hModel_ = Model::Load("model\\GroundGrid.fbx");

    pParticle__ = Instantiate<Particle>(this);

    //��
    {
        EmitterData data;

        //��
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-4, 1.5, -4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1.5, 1.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);
        pParticle__->Start(data);

        //�΂̕�
        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle__->Start(data);
    }


    //��
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(4, 1.5, 4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 150;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2, 2);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 1, 0.2);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.002);
        pParticle__->Start(data);
    }

    //��
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\Water.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.8, 0.4);
        data.scale = XMFLOAT2(1.02, 1.02);
        data.color = XMFLOAT4(1, 1, 1, 0.1);
        pParticle__->Start(data);

        //���H
        data.textureFileName = "image\\PaticleAssets\\bubleB.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.positionErr = XMFLOAT3(0.5, 0, 0);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.3, 0.3);
        data.sizeErr = XMFLOAT2(0, 0);
        data.scale = XMFLOAT2(0.98, 0.98);
        data.color = XMFLOAT4(1, 1, 1, 1);
        pParticle__->Start(data);
    }

    //1
    {
        EmitterData data;
        //wind
        data.textureFileName = "image\\PaticleAssets\\circle_B.png";
        data.position = XMFLOAT3(-8, 1, 5);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2.5, 2.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(0.5, 0.8, 0.0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle__->Start(data);

        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle__->Start(data);
    }

}

//�X�V
void TestScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE))
    {
        EmitterData data;
        data.textureFileName = "image\\PaticleAssets\\CloudC.png";
        data.position = XMFLOAT3(0, 0.05, 0);
        data.delay = 0;
        data.number = 80;
        data.lifeTime = 20;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.1f;
        data.speedErr = 0.8;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.05, 1.05);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
        pParticle__->Start(data);


        data.delay = 0;
        data.number = 80;
        data.lifeTime = 100;
        data.positionErr = XMFLOAT3(0.5, 0, 0.5);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.25f;
        data.speedErr = 1;
        data.accel = 0.93;
        data.size = XMFLOAT2(0.1, 0.1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(0.99, 0.99);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, 0);
        data.gravity = 0.003f;
        pParticle__->Start(data);
    }

    if (Input::IsKey(DIK_V))
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 2;
        data.gravity = 0;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.05f;
        data.speedErr = 0.01f;
        data.size = XMFLOAT2(1.25, 1.25);
        data.sizeErr = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle__->Start(data);

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.1, 0.1, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 30;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.002;
        data.speed = 0.2f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.1, 0.1);
        data.scale = XMFLOAT2(0.9, 0.9);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle_->Start(data);
    }

    if (Input::IsKey(DIK_B))
    {
        EmitterData data;

        //��
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-5, 1.0, -4);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0.15);
        data.delay = 0;
        data.number = 4;
        data.lifeTime = 15;
        data.gravity = 0.001f;
        data.dir = XMFLOAT3(-1, 0, 0);
        data.dirErr = XMFLOAT3(0.75, 0.75, 0.75);
        data.speed = 0.4f;
        data.speedErr = 0.01;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(1.15, 1.15);
        data.color = XMFLOAT4(1, 0.2, 0, 1);
        data.deltaColor = XMFLOAT4(0.0, 0.02, 0.01, -0.075);
        pParticle_->Start(data);
    }
}
#endif
//power oh-ra
#if 0
if (!Input::IsKey(DIK_C))
{
    EmitterData data;

    //��
    data.textureFileName = "image\\PaticleAssets\\CloudA.png";
    data.position = transform_.position_;
    data.positionErr = XMFLOAT3(0.1, 0, 0.1);
    //data.delay = 1;
    data.number = 1;
    data.lifeTime = 10;
    data.gravity = -0.002f;
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(0, 0, 0);
    data.speed = 0.75f;
    data.speedErr = 0.2;
    data.size = XMFLOAT2(5, 5);
    data.sizeErr = XMFLOAT2(0.4, 0.4);
    data.scale = XMFLOAT2(0.9, 0.9);
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.1, -0.2);
    pParticle->Start(data);

    //�΂̕�
    data.number = 1;
    data.positionErr = XMFLOAT3(0.8, 0, 0.8);
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(10, 10, 10);
    data.size = XMFLOAT2(0.5, 0.5);
    data.scale = XMFLOAT2(0.98, 0.98);
    data.lifeTime = 30;
    data.speed = 0.2f;
    data.gravity = 0.005;
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.02, -0.015);
    pParticle->Start(data);
}
#endif