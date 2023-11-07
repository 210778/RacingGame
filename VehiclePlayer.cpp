#include "VehiclePlayer.h"

#include "Engine/Text.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

#include "Viewer.h"
#include "Speedometer.h"
#include "ParticlePackage.h"
#include "Sample.h"
#include "Music.h"
#include "Circuit.h"
#include "VehicleInput.h"

using std::string;
using std::to_string;

//コンストラクタ
VehiclePlayer::VehiclePlayer(GameObject* parent)
    :Vehicle(parent, "VehiclePlayer")
    , pText_(nullptr)
    , km_hAdd(120.0f)
    , pSample_(nullptr)
{
}

//コンストラクタ
VehiclePlayer::VehiclePlayer(GameObject* parent, std::string vehicleName, std::string wheelName)
    :Vehicle(parent, "VehiclePlayer")
    , pText_(nullptr), hImage_(-1)
    , km_hAdd(122.0f), flashIntervalUI_(15), IsFlashUI_(true)
    , pSample_(nullptr)
    , imageBoostMax_(-1), imageBoost_(-1)
    , hImageSpeedFrame_(-1), hImageSpeedNeedle_(-1)
    , drawTime_(0), standardTime_(60)
    , textCenter_(450), textPauseHeight_(150), textReturnHeight_(200), textLapHeight_(70)
    , textToTitleWidth_(700), textKMH_(150, 700), textLeftEnd_(30), textUpEnd_(30), textRankWidth_(110)
    , textDebugCenter_(280), textPosWidth_(60), textRotWidth_(90)
{
    vehicleModelName_ = vehicleName;
    wheelModelName_ = wheelName;
    isPlayer_ = true;/////////////////////////
}

//デストラクタ
VehiclePlayer::~VehiclePlayer()
{
}

//開放
void VehiclePlayer::Release()
{
    pText_->Release();
}

//UIの初期化
void VehiclePlayer::PlayerUI_Initialize()
{
    standardTime_ = Global::GetStandardFPS();

#ifdef _DEBUG
    //デバッグ用
    pSample_ = Instantiate<Sample>(GetParent());
#endif

    //文字
    pText_ = new Text;
    pText_->Initialize();

    imageTrans_.position_ = { 0.0f,0.5f,0.0f };
    imageTrans_.scale_ = { 0.3f,0.2f,1.0f };

    //画像
    hImage_ = Image::Load("image\\BackGround_K.jpg");
    assert(hImage_ >= 0);

    //スピードメーター
    hImageSpeedFrame_ = Image::Load("image\\mator6.png");
    hImageSpeedNeedle_ = Image::Load("image\\matorNeedle3.png");
    SpeedmeterTransform_.position_ = { -0.875f, -0.8f, 0.0f };

    //ブースト
    imageBoostMax_ = Image::Load("image\\grayBar.png");
    imageBoost_    = Image::Load("image\\redBar.png");

    boostMaxTrans_.position_ = { 0.0f,0.8f,1.0f };
    boostMaxTrans_.scale_ = { 2.55,1.5,1.0f };
    boostTrans_ = boostMaxTrans_;
    boostTrans_.scale_ = { boostMaxTrans_.scale_.x * 0.95f , boostMaxTrans_.scale_.y * 0.8f, 1.0f, };

    //カウントダウン表
    Transform trans;
    Transform change;
    change.scale_ = { 0.05f, 0.05f, 0.05f };
    int cAlpha = -5;
    SetImage(ImageData::ImageNumber::start, Image::Load("image\\count_start.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
    SetImage(ImageData::ImageNumber::one, Image::Load("image\\count_1.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
    SetImage(ImageData::ImageNumber::two, Image::Load("image\\count_2.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
    SetImage(ImageData::ImageNumber::three, Image::Load("image\\count_3.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
    SetImage(ImageData::ImageNumber::goal, Image::Load("image\\count_goal.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
}

//UIの表示
void VehiclePlayer::PlayerUI_Draw()
{
    //スピードメーター  
    DrawSpeedmeter();

    //時速表示
    DrawKmH();

    //ポーズ画面
    if (pauseFlag_)
    {
        Image::SetTransform(hImage_, imageTrans_);
        Image::Draw(hImage_);

        pText_->Draw(textCenter_, textPauseHeight_, "[Pause]");
        pText_->Draw(textCenter_, textReturnHeight_, "Return to the title?");
    }

    //点滅
    drawTime_ = time_;
    if (goalFlag_)
    {
        //ゴールしてるなら
        drawTime_ = goalTime_;

        //点滅させる
        if (time_ % flashIntervalUI_ == 0)
        {
            IsFlashUI_ = (IsFlashUI_) ? false : true;
        }
    }

    //経過時間表示
    if(IsFlashUI_)
        DrawElapsedTime();

    //周回数表示
    string lapStr = "Lap:";
    lapStr += to_string(lapCount_) + "/" + to_string(lapMax_);
    //点滅
    if (IsFlashUI_)
        pText_->Draw(textLeftEnd_, textLapHeight_, lapStr.c_str());

    //順位表示
    if (IsFlashUI_)
        DrawRanking();

    //ゴールの後に表示
    if (IsFlashUI_ && goalFlag_)
    {
        pText_->Draw(textCenter_, textToTitleWidth_, "[Pause to Title scene]");
    }

#if 0
    //順位表示
    string rank;
    //ゴールしてるか否か
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

    //ブースト
    DrawBoostGauge();

#ifdef _DEBUG
    //次のチェックポイント位置
    pSample_->SetPosition(*GetNextCheckPosition());

    //加速度表示
    DrawAccelerationRotate();
#endif

    //カウントダウン表示
    DrawStandbyCount();
}

//UIの情報更新
void VehiclePlayer::PlayerUI_Update()
{
    //カウントダウン表示
    for (auto& itr : imageMap_)
    {
        //表示しなくする
        if (itr.second.life_ >= itr.second.lifeTime_)
            itr.second.isPrint_ = false;
    }
}

//カメラの用意
void VehiclePlayer::PlayerCamera_Initialize()
{
    pViewer_ = Instantiate<Viewer>(this);
    pViewer_->WatchPresentPosition();    //今すぐ移動
}

//カメラ更新
void VehiclePlayer::PlayerCamera_Update()
{
    pViewer_->SetViewValue(transform_, acceleration_, handleRotate_);
}

//操作
void VehiclePlayer::InputOperate()
{
    //ハンドル
    operation_.ValueMap[Operation::Value::handleRightLeft] = VehicleInput::GetInput(VehicleInput::Value::handleRightLeft);

    //前進後退
    operation_.ValueMap[Operation::Value::moveFrontRear] = VehicleInput::GetInput(VehicleInput::Value::moveFrontRear);

    //ブースト
    operation_.ButtonMap[Operation::Button::boost] = VehicleInput::GetInput(VehicleInput::Button::useBooster);

    //左右回転
    operation_.ValueMap[Operation::Value::turnRightLeft] = VehicleInput::GetInput(VehicleInput::Value::turnRightLeft);

    //ジャンプ
    operation_.ButtonMap[Operation::Button::jump] = VehicleInput::GetInput(VehicleInput::Button::jumpUp);

    //左右移動
    operation_.ValueMap[Operation::Value::moveRightLeft] = VehicleInput::GetInput(VehicleInput::Value::moveRightLeft);
}

//画像をセット
void VehiclePlayer::SetImage(ImageData::ImageNumber in, int handle, Transform transform
    , int time, Transform cTransform, int alpha, int cAlpha)
{
    imageMap_[in].imageHandle_ = handle;
    imageMap_[in].imageTransform_ = transform;
    imageMap_[in].lifeTime_ = time;
    imageMap_[in].changeTransform_ = cTransform;
    imageMap_[in].imageAlpha_ = alpha;
    imageMap_[in].changeAlpha_ = cAlpha;
}
//画像描画
void VehiclePlayer::DrawImage(ImageData::ImageNumber in)
{
    imageMap_[in].isPrint_ = true;
    imageMap_[in].isAlreadyPrint_ = true;
    imageMap_[in].life_ = 0;
}

//スピードメーター描画
void VehiclePlayer::DrawSpeedmeter()
{
    //枠
    SpeedmeterTransform_.rotate_.z = 0;
    Image::SetTransform(hImageSpeedFrame_, SpeedmeterTransform_);
    Image::Draw(hImageSpeedFrame_);
    //針
    SpeedmeterTransform_.rotate_.z = -(*XMVector3LengthEst(acceleration_).m128_f32 * km_hAdd);
    Image::SetTransform(hImageSpeedNeedle_, SpeedmeterTransform_);
    Image::Draw(hImageSpeedNeedle_);
}

void VehiclePlayer::DrawKmH()
{
    //平面のベクトルの長さ
    XMVECTOR speedVec = acceleration_;
    string speedStr = to_string((int)(*XMVector3LengthEst(speedVec).m128_f32 * km_hAdd));
    speedStr += "km/h";
    pText_->Draw(textKMH_.x, textKMH_.y, speedStr.c_str());
}

void VehiclePlayer::DrawElapsedTime()
{
    const int standard = 60;
    int min = 0;
    int sec = 0;
    int mSec = 0;
    int rest = 0;
    min = drawTime_ / (standard * standard);
    rest = drawTime_ % (standard * standard);
    sec = rest / standard;
    rest = rest % standard;
    mSec = rest;
    string timeStr = "Time:";
    if (min < 10)
        timeStr += "0";
    timeStr += to_string(min) + ":";
    if (sec < 10)
        timeStr += "0";
    timeStr += to_string(sec) + ":";
    if (mSec < 10)
        timeStr += "0";
    timeStr += to_string(mSec);

    pText_->Draw(textLeftEnd_, textUpEnd_, timeStr.c_str());
}

void VehiclePlayer::DrawRanking()
{
    string rank = to_string(ranking_);

    //人数は多くても２０人ぐらいだとする
    switch (ranking_){
    default:rank += "th"; break;
    case 1: rank += "st"; break;
    case 2: rank += "nd"; break;
    case 3: rank += "rd"; break;
    }
    rank += "/" + to_string(population_);

    pText_->Draw(textLeftEnd_, textRankWidth_, rank.c_str());
}

void VehiclePlayer::DrawBoostGauge()
{
    //満タンなら表示しない
    if (boostCapacity_ < boostCapacityMax_)
    {
        Transform boostValue = boostTrans_;

        boostValue.scale_.x *= boostCapacity_ / boostCapacityMax_;

        Image::SetTransform(imageBoostMax_, boostMaxTrans_);
        Image::Draw(imageBoostMax_);
        Image::SetTransform(imageBoost_, boostValue);
        Image::Draw(imageBoost_);
    }
}

void VehiclePlayer::DrawAccelerationRotate()
{
    XMFLOAT3 floAcc;
    //ベクトルY軸で回転用行列
    XMMATRIX matRotateY_R = XMMatrixRotationY(XMConvertToRadians(-transform_.rotate_.y));
    XMStoreFloat3(&floAcc, XMVector3TransformCoord(acceleration_, matRotateY_R));
    string accStr = "X:" + to_string(floAcc.x) + " Y:" + to_string(floAcc.y) + " Z:" + to_string(floAcc.z);
    pText_->Draw(textDebugCenter_, textUpEnd_, accStr.c_str());

    accStr = "X:" + to_string(transform_.position_.x)
        + " Y:" + to_string(transform_.position_.y)
        + " Z:" + to_string(transform_.position_.z);
    pText_->Draw(textDebugCenter_, textPosWidth_, accStr.c_str());

    accStr = "X:" + to_string(transform_.rotate_.x)
        + " Y:" + to_string(transform_.rotate_.y)
        + " Z:" + to_string(transform_.rotate_.z);
    pText_->Draw(textDebugCenter_, textRotWidth_, accStr.c_str());
}

//カウントダウン描画
void VehiclePlayer::DrawStandbyCount()
{
    if (time_ <= 0)
    {
        if (((float)standbyTime_ / standardTime_) < 1
            && !(imageMap_[ImageData::ImageNumber::one].isAlreadyPrint_))
        {
            DrawImage(ImageData::ImageNumber::one);
            Music::Play(Music::MusicName::se_count123);
        }
        if (((float)standbyTime_ / standardTime_) < 2
            && !(imageMap_[ImageData::ImageNumber::two].isAlreadyPrint_))
        {
            DrawImage(ImageData::ImageNumber::two);
            Music::Play(Music::MusicName::se_count123);
        }
        if (((float)standbyTime_ / standardTime_) < 3
            && !(imageMap_[ImageData::ImageNumber::three].isAlreadyPrint_))
        {
            DrawImage(ImageData::ImageNumber::three);
            Music::Play(Music::MusicName::se_count123);
        }
    }
    else
    {
        if (!(imageMap_[ImageData::ImageNumber::start].isAlreadyPrint_))
        {
            DrawImage(ImageData::ImageNumber::start);
            Music::Play(Music::MusicName::se_countStart);
        }
    }

    //周回した時(非ゴール)の音声
    if (lapCountFlag_)
    {
        lapCountFlag_ = false;
        Music::Play(Music::MusicName::se_lap);

        if (goalFlag_ && !(imageMap_[ImageData::ImageNumber::goal].isAlreadyPrint_))
            DrawImage(ImageData::ImageNumber::goal);
    }


    for (auto& itr : imageMap_)
    {
        //表示する
        if (itr.second.isPrint_)
        {
            //トランスフォーム
            Transform tra;
            tra.position_ = Transform::Float3Add(itr.second.imageTransform_.position_
                , Transform::Float3Times(itr.second.changeTransform_.position_, itr.second.life_));
            tra.rotate_ = Transform::Float3Add(itr.second.imageTransform_.rotate_
                , Transform::Float3Times(itr.second.changeTransform_.rotate_, itr.second.life_));
            tra.scale_ = Transform::Float3Add(itr.second.imageTransform_.scale_
                , Transform::Float3Times(itr.second.changeTransform_.scale_, itr.second.life_));
            Image::SetTransform(itr.second.imageHandle_, tra);
            //透明度
            Image::SetAlpha(itr.second.imageHandle_
                , itr.second.imageAlpha_ + (itr.second.changeAlpha_ * itr.second.life_));
            //描画
            Image::Draw(itr.second.imageHandle_);
            //加算
            itr.second.life_++;
        }
    }
}