#include "VehiclePlayer.h"

#include "Engine/Text.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"

#include "Speedometer.h"
#include "Viewer.h"
#include "Ground.h"
#include "Bullet.h"
#include "Speedometer.h"
#include "ParticlePackage.h"
#include "Sample.h"

using std::string;
using std::to_string;

//�R���X�g���N�^
VehiclePlayer::VehiclePlayer(GameObject* parent)
    :Vehicle(parent, "VehiclePlayer")
    , pSpeedometer_(nullptr), pTextSpeed_(nullptr), pTextTime_(nullptr)
    , pTextLap_(nullptr), pTextRanking_(nullptr), pTextAcceleration_(nullptr)
    , km_hAdd(120.0f)
    , pSample_(nullptr)
{
}

//�R���X�g���N�^
VehiclePlayer::VehiclePlayer(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehiclePlayer")
    , pViewer(nullptr)
    , pSpeedometer_(nullptr), pTextSpeed_(nullptr), pTextTime_(nullptr)
    , pTextLap_(nullptr), pTextRanking_(nullptr), pTextAcceleration_(nullptr)
    , km_hAdd(120.0f), flashIntervalUI_(15), IsFlashUI_(true)
    , pSample_(nullptr)
    , imageBoostMax_(-1), imageBoost_(-1)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
    isPlayer_ = true;
}

//�f�X�g���N�^
VehiclePlayer::~VehiclePlayer()
{
}

//�J��
void VehiclePlayer::Release()
{
    pTextSpeed_->Release();
    pTextTime_->Release();
    pTextLap_->Release();
    pTextRanking_->Release();
    pTextAcceleration_->Release();
}

//UI�̏�����
void VehiclePlayer::PlayerUI_Initialize()
{
    //����
    pTextSpeed_ = new Text;
    pTextSpeed_->Initialize();

    pTextTime_ = new Text;
    pTextTime_->Initialize();

    pTextLap_ = new Text;
    pTextLap_->Initialize();

    pTextRanking_ = new Text;
    pTextRanking_->Initialize();

    pTextAcceleration_ = new Text;
    pTextAcceleration_->Initialize();

    pSample_ = Instantiate<Sample>(GetParent());

    //�X�s�[�h���[�^�[
    pSpeedometer_ = Instantiate<Speedometer>(GetParent());

    //�摜
    imageBoostMax_ = Image::Load("image\\grayBar.png");
    imageBoost_    = Image::Load("image\\redBar.png");
}

//UI�̕\��
void VehiclePlayer::PlayerUI_Draw()
{
    //�����\��
    //���ʂ̃x�N�g���̒���
    XMVECTOR speedVec = acceleration_;
    string speedStr = to_string((int)(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd));
    speedStr += "km/h";
    pTextSpeed_->Draw(30, 30, speedStr.c_str());

    unsigned long long time = time_;
    if (goalFlag_)
    {
        //�S�[�����Ă�Ȃ�
        time = goalTime_;

        //�_�ł�����
        if (time_ % flashIntervalUI_ == 0)
        {
            if (IsFlashUI_)
                IsFlashUI_ = false;
            else
                IsFlashUI_ = true;
        }
    }

    //�o�ߎ��ԕ\��
    int min = 0;
    int sec = 0;
    int mSec = 0;
    int rest = 0;
    min = time / 3600;
    rest = time % 3600;
    sec = rest / 60;
    rest = rest % 60;
    mSec = rest;
    string timeStr = "time:";    //= to_string(min) + ":" + to_string(sec) + ":" + to_string(mSec);
    if (min < 10)
        timeStr += "0";
    timeStr += to_string(min) + ":";
    if (sec < 10)
        timeStr += "0";
    timeStr += to_string(sec) + ":";
    if (mSec < 10)
        timeStr += "0";
    timeStr += to_string(mSec);
    //�_��
    if(IsFlashUI_)
    {
        pTextTime_->Draw(30, 70, timeStr.c_str());
    }

    //���񐔕\��
    string lapStr = "Lap:";
    lapStr += to_string(lapCount_) + "/" + to_string(lapMax_);
    //�_��
    if (IsFlashUI_)
    {
        pTextLap_->Draw(30, 110, lapStr.c_str());
    }

    //���ʕ\��
    string rank = to_string(ranking_);

    //�l���͑����Ă��Q�O�l���炢���Ƃ���
    switch (ranking_)
    {
    default:rank += "th"; break;
    case 1: rank += "st"; break;
    case 2: rank += "nd"; break;
    case 3: rank += "rd"; break;
    }

    rank += "/" + to_string(population_);
    //�_��
    if (IsFlashUI_)
    {
        pTextRanking_->Draw(30, 150, rank.c_str());
    }
#if 0
    //���ʕ\��
    string rank;
    //�S�[�����Ă邩�ۂ�
    if (goalFlag_)
        rank = to_string(goalRanking_);
    else
        rank = to_string(ranking_);


    //pTextLap_->Draw(30, 150, rank.c_str());
    string all = "!\"#$%&\'()*+,-./0123456789:;<=>?@\
ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    string one      = " ! \" # $ % & \' ( ) * + , - . / 0 ";
    string two      = " 1 2 3 4 5 6 7 8 9 : ; < = > ? @ ";
    string three    = " A B C D E F G H I J K L M N O P ";
    string four     = " Q R S T U V W X Y Z [ \\ ] ^ _ ` ";
    string five     = " a b c d e f g h i j k l m n o p ";
    string six      = " q r s t u v w x y z { | } ~ ";

    pTextLap_->Draw(10, 150, one.c_str());
    pTextLap_->Draw(10, 185, two.c_str());
    pTextLap_->Draw(10, 220, three.c_str());
    pTextLap_->Draw(10, 255, four.c_str());
    pTextLap_->Draw(10, 290, five.c_str());
    pTextLap_->Draw(10, 325, six.c_str());
#endif

    //�u�[�X�g
    //���^���Ȃ�\�����Ȃ�
    if (boostCapacity_ < boostCapacityMax_)
    {
        Transform boostMaxTrans, boostTrans;
        boostMaxTrans.position_ = { 0.0f,0.8f,1.0f };
        boostMaxTrans.scale_ = { 2.55,1.5,1.0f };

        //
        boostTrans = boostMaxTrans;
        boostTrans.scale_ = { boostMaxTrans.scale_.x * 0.95f * (boostCapacity_ / boostCapacityMax_)
                            , boostMaxTrans.scale_.y * 0.8f, 1.0f, };

        Image::SetTransform(imageBoostMax_, boostMaxTrans);
        Image::Draw(imageBoostMax_);
        Image::SetTransform(imageBoost_, boostTrans);
        Image::Draw(imageBoost_);
    }

#ifdef _DEBUG
    //
    pSample_->SetPosition(*GetNextCheckPosition());
    pSample_->SetScale({ 20.0,20.0,20.0 });

    //�����x�\��
    XMFLOAT3 floAcc;
    //�x�N�g��Y���ŉ�]�p�s��
    XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
    XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));
    string accStr = "X:" + to_string(floAcc.x) + " Y:" + to_string(floAcc.y) + " Z:" + to_string(floAcc.z);

    pTextAcceleration_->Draw(280, 30, accStr.c_str());

    accStr = "X:" + to_string(transform_.rotate_.x)
                 + " Y:" + to_string(transform_.rotate_.y)
                 + " Z:" + to_string(transform_.rotate_.z);
    pTextAcceleration_->Draw(280, 60, accStr.c_str());
#endif
}

//UI�̏��X�V
void VehiclePlayer::PlayerUI_Update()
{
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    pSpeedometer_->SetSpeed(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd);
}

//�J�����̗p��
void VehiclePlayer::PlayerCamera_Initialize()
{
    pViewer = Instantiate<Viewer>(this);
}

//�J�����X�V
void VehiclePlayer::PlayerCamera_Update()
{
    pViewer->SetViewValue(transform_, acceleration_, handleRotate_);
}

void VehiclePlayer::PlayerParticle()
{
    float accLength = *XMVector3Length(acceleration_).m128_f32;

    //�S�[��������
    if (goalFlag_)
    {
        ParticlePackage::ActRainbowFire(pParticle_, transform_.position_);
    }

    //���s���̃^�C���̋O��
    if (wheelParticleLength_ < accLength
        && wheelParticleLengthMax_ > accLength)
    {
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRL"));
    }

    //���n���グ
    if (landingType_ == Ground::turf
        && landingFlag_
        && wheelParticleLength_ < accLength)
    {
        ParticlePackage::ActLandingGrass(pParticle_, transform_.position_);
    }

    //���n
    if (landingType_ == Ground::dirt
        && landingFlag_
        && wheelParticleLength_ < accLength)
    {
        ParticlePackage::ActLandingDirt(pParticle_, transform_.position_);
    }

}

//����
void VehiclePlayer::InputOperate()
{
    //�n���h���̑���
        if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
        {
            Operation.inputNow[Operation.inputName::handleLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
        {
            Operation.inputNow[Operation.inputName::handleRight] = 1.0f;
        }

    //�S�g���
        if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
        {
            Operation.inputNow[Operation.inputName::moveFront] = 1.0f;
        }
        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            Operation.inputNow[Operation.inputName::moveRear] = 1.0f;
        }

    //�u�[�X�g
        if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
        {
            Operation.inputNow[Operation.inputName::boost] = 1.0f;
        }


    //���E��]
        if (Input::IsKey(DIK_Z) || Input::IsKey(DIK_Q))
        {
            Operation.inputNow[Operation.inputName::turnLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_X) || Input::IsKey(DIK_E))
        {
            Operation.inputNow[Operation.inputName::turnRight] = 1.0f;
        }

    //�W�����v
        if (Input::IsKey(DIK_M))
        {
            Operation.inputNow[Operation.inputName::jump] = 1.0f;    
        }

    //���E�ړ�
        if (Input::IsKey(DIK_V))
        {
            Operation.inputNow[Operation.inputName::moveLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_B))
        {
            Operation.inputNow[Operation.inputName::moveRight] = 1.0f;
        }


#if 0
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

    //�u�[�X�g
    if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
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
}
    else
    {
        slideFlag_ = false;

        //������ƈʒu������
        boostCapacity_ += boostIncrease_;
        //���ӂ��
        if (boostCapacity_ > boostCapacityMax_)
            boostCapacity_ = boostCapacityMax_;
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
#endif
}

#if 0

//������
void VehiclePlayer::Initialize()
{
    hModel_ = Model::Load("model\\Tri_Lines.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = { 3,3,3 };
}

//�X�V
void VehiclePlayer::Update()
{
    /*
    XMFLOAT3 �� XMVECTOR

    XMFLOAT3 f;	//���������Ă�Ƃ���
    XMVECTOR v = XMLoadFloat3(&f);

    XMFLOAT3 �� XMVECTOR

    XMVECTOR v; 	//���������Ă�Ƃ���
    XMFLOAT3 f;
    XMStoreFloat3(&f, v);
    */
}

//�����ɓ�������
void VehiclePlayer::OnCollision(GameObject* pTarget)
{
    //���������Ƃ��̏���
}

//�`��
void VehiclePlayer::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}


#endif