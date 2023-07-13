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
    , km_hAdd(120.0f)
    , pSample_(nullptr)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
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
}

//UI�̕\��
void VehiclePlayer::PlayerUI_Draw()
{
    //�����\��
    //���ʂ̃x�N�g���̒���
    XMVECTOR speedVec = acceleration_ * XMVECTOR{ 1,0,1,1 };
    string speedStr = to_string((int)(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd));
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
    pTextTime_->Draw(30, 70, timeStr.c_str());

    //���񐔕\��
    string lapStr = "Lap:";
    lapStr += to_string(lapCount_) + "/" + to_string(lapMax_);
    pTextLap_->Draw(30, 110, lapStr.c_str());

    //���ʕ\��
    string rank = to_string(ranking_) + "/" + to_string(population_);
    pTextRanking_->Draw(30, 150, rank.c_str());
#if 0
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
    pViewer->SetPosition(transform_.position_);
}

//�J�����X�V
void VehiclePlayer::PlayerCamera_Update()
{
    pViewer->SetViewValue(transform_, acceleration_, handleRotate_);
}

void VehiclePlayer::PlayerParticle()
{
    //�S�[��������
    if (goalFlag_)
    {
        ParticlePackage::ActRainbowFire(pParticle_, transform_.position_);
    }

    //���s���̃^�C���̋O��
    if (landingType_ != Ground::turf
        && wheelParticleLength_ < *XMVector3Length(acceleration_).m128_f32)
    {
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRR"));
        ParticlePackage::ActSmokeCloud(pParticle_, Model::GetBonePosition(hModel_, "wheelRL"));
    }

    //���n���グ
    if (landingType_ == Ground::turf && landingFlag_
        && wheelParticleLength_ < *XMVector3Length(acceleration_).m128_f32)
    {
        ParticlePackage::ActLandingGrass(pParticle_, transform_.position_);
    }
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