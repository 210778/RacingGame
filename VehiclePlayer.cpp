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
#include "Music.h"
#include "ImagePrinter.h"
#include "Circuit.h"

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
    , pSpeedometer_(nullptr), pTextSpeed_(nullptr), pTextTime_(nullptr)
    , pTextLap_(nullptr), pTextRanking_(nullptr), pTextAcceleration_(nullptr)
    , km_hAdd(122.0f), flashIntervalUI_(15), IsFlashUI_(true)
    , pSample_(nullptr)
    , imageBoostMax_(-1), imageBoost_(-1)
    , pImagePrinter_(nullptr)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
    isPlayer_ = true;/////////////////////////
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
    pSpeedometer_->SetPosition({ -0.875f, -0.8f, 0.0f });

    //�摜
    imageBoostMax_ = Image::Load("image\\grayBar.png");
    imageBoost_    = Image::Load("image\\redBar.png");

    pImagePrinter_ = Instantiate<ImagePrinter>(GetParent());
    isPrintedMap_["start"] = false;
    isPrintedMap_["1"] = false;
    isPrintedMap_["2"] = false;
    isPrintedMap_["3"] = false;
    isPrintedMap_["goal"] = false;
}

//UI�̕\��
void VehiclePlayer::PlayerUI_Draw()
{
    //�����\��
    //���ʂ̃x�N�g���̒���
    XMVECTOR speedVec = acceleration_;
    string speedStr = to_string((int)(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd));
    speedStr += "km/h";
    pTextSpeed_->Draw(150, Global::GetScreenHeight() - 20, speedStr.c_str());

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
    int standard = Global::GetStandardFPS();
    int min = 0;
    int sec = 0;
    int mSec = 0;
    int rest = 0;
    min = time / (standard * standard);
    rest = time % (standard * standard);
    sec = rest / standard;
    rest = rest % standard;
    mSec = rest;
    string timeStr = "Time:";    //= to_string(min) + ":" + to_string(sec) + ":" + to_string(mSec);
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
        pTextTime_->Draw(30, 30, timeStr.c_str());
    }

    //���񐔕\��
    string lapStr = "Lap:";
    lapStr += to_string(lapCount_) + "/" + to_string(lapMax_);
    //�_��
    if (IsFlashUI_)
    {
        pTextLap_->Draw(30, 70, lapStr.c_str());
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
        pTextRanking_->Draw(30, 110, rank.c_str());
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

    //�J�E���g�_�E���\��
    if (time_ <= 0)
    {
        if (((float)standbyTime_ / standard) < 1 && isPrintedMap_["1"] == false)
        {
            pImagePrinter_->DrawImage(ImagePrinter::ImageNumber::one);
            isPrintedMap_["1"] = true;
            Music::Play(Music::MusicName::se_count123);
        }
        if (((float)standbyTime_ / standard) < 2 && isPrintedMap_["2"] == false)
        {
            pImagePrinter_->DrawImage(ImagePrinter::ImageNumber::two);
            isPrintedMap_["2"] = true;
            Music::Play(Music::MusicName::se_count123);
        }
        if (((float)standbyTime_ / standard) < 3 && isPrintedMap_["3"] == false)
        {
            pImagePrinter_->DrawImage(ImagePrinter::ImageNumber::three);
            isPrintedMap_["3"] = true;
            Music::Play(Music::MusicName::se_count123);
        }
    }
    else
    {
        if (isPrintedMap_["start"] == false)
        {
            pImagePrinter_->DrawImage(ImagePrinter::ImageNumber::start);
            isPrintedMap_["start"] = true;
            Music::Play(Music::MusicName::se_countStart);
        }
    }

    if (goalFlag_ && isPrintedMap_["goal"] == false)
    {
        pImagePrinter_->DrawImage(ImagePrinter::ImageNumber::goal);
        isPrintedMap_["goal"] = true;
        Music::Play(Music::MusicName::se_goal);
    }
}

//UI�̏��X�V
void VehiclePlayer::PlayerUI_Update()
{
    pSpeedometer_->SetSpeed(*XMVector3LengthEst(acceleration_).m128_f32 * km_hAdd);
}

//�J�����̗p��
void VehiclePlayer::PlayerCamera_Initialize()
{
    pViewer_ = Instantiate<Viewer>(this);
    pViewer_->WatchPresentPosition();    //�������ړ�
}

//�J�����X�V
void VehiclePlayer::PlayerCamera_Update()
{
    pViewer_->SetViewValue(transform_, acceleration_, handleRotate_);
}

//����
void VehiclePlayer::InputOperate()
{
    //�n���h���̑���
        if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
        {
            operation_.inputNow[operation_.inputName::handleLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
        {
            operation_.inputNow[operation_.inputName::handleRight] = 1.0f;
        }

    //�S�g���
        if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
        {
            operation_.inputNow[operation_.inputName::moveFront] = 1.0f;
        }
        if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
        {
            operation_.inputNow[operation_.inputName::moveRear] = 1.0f;
        }

    //�u�[�X�g
        if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_SPACE))
        {
            operation_.inputNow[operation_.inputName::boost] = 1.0f;
        }


    //���E��]
        if (Input::IsKey(DIK_Z) || Input::IsKey(DIK_Q))
        {
            operation_.inputNow[operation_.inputName::turnLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_X) || Input::IsKey(DIK_E))
        {
            operation_.inputNow[operation_.inputName::turnRight] = 1.0f;
        }

    //�W�����v
        if (Input::IsKey(DIK_M))
        {
            operation_.inputNow[operation_.inputName::jump] = 1.0f;    
        }

    //���E�ړ�
        if (Input::IsKey(DIK_V))
        {
            operation_.inputNow[operation_.inputName::moveLeft] = 1.0f;
        }
        if (Input::IsKey(DIK_B))
        {
            operation_.inputNow[operation_.inputName::moveRight] = 1.0f;
        }

}