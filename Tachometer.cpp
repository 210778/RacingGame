#include "Tachometer.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

using std::string;
using std::vector;

//コンストラクタ
Tachometer::Tachometer(GameObject* parent)
    :GameObject(parent, "Tachometer"), hModel_(-1), hImage_(-1)
    , hPict_{ -1,-1,-1 }
    ,stopRatio_((float)nowHp_ / (float)maxHp_), moveRatio_(stopRatio_)
    ,nowHp_(180)      //初期HP （適宜変更）
    ,maxHp_(250)      //MaxHP（適宜変更）
    ,SPEED(0.005f)     //バーの移動速度（お好みで）
{
}

//デストラクタ
Tachometer::~Tachometer()
{
}

//初期化
void Tachometer::Initialize()
{
    hModel_ = Model::Load("model\\red_arrow.fbx");
    assert(hModel_ >= 0);

    //ゲージのサイズ（画像サイズが基準）
    transform_.scale_ = XMFLOAT3(1.5f, 1.5f, 1.0f);

    //ゲージの位置（画面中心が基準）
    transform_.position_ = XMFLOAT3(-0.0f, 0.88f, 0);

    //画像ロード
    string header = "image\\";
    vector<string>modelName = { "Gauge_Frame.png","Gauge_Green.png","Gauge_LightGreen.png","Gauge_Red.png" };
    for (int i = 0; i < modelName.size(); i++)
    {
        hPict_[i] = Image::Load(header + modelName[i]);
        assert(hPict_[i] >= 0);
    }

}

//更新
void Tachometer::Update()
{
    //最大HPに対する、現在HPの割合
    stopRatio_ = (float)nowHp_ / (float)maxHp_;

    //バーを動かす
    if (moveRatio_ < stopRatio_) moveRatio_ += SPEED;
    if (moveRatio_ > stopRatio_) moveRatio_ -= SPEED;
}

//描画
void Tachometer::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);


    Transform gaugeTrans;

    //枠
    //gaugeTrans = transform_;
    //Image::SetTransform(hPict_[PICT_FRAME], gaugeTrans);
    //Image::Draw(hPict_[PICT_FRAME]);

    //HP変化なし
    if (abs(moveRatio_ - stopRatio_) < SPEED)
    {
        //緑ゲージ
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }


    //回復中
    else if (moveRatio_ < stopRatio_)
    {
        //黄緑ゲージ
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_LGREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_LGREEN]);

        //緑ゲージ
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= moveRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }

    //ダメージ中
    else
    {
        //赤ゲージ
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= moveRatio_;
        Image::SetTransform(hPict_[PICT_RED], gaugeTrans);
        Image::Draw(hPict_[PICT_RED]);

        //緑ゲージ
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }


    //枠
    gaugeTrans = transform_;

    Image::SetAlpha(hPict_[PICT_FRAME], 128);
    Image::SetRect(hPict_[PICT_FRAME], 2, 50, 3, 20);

    Image::SetTransform(hPict_[PICT_FRAME], gaugeTrans);
    Image::Draw(hPict_[PICT_FRAME]);
}

//開放
void Tachometer::Release()
{
}

//ダメージ
void Tachometer::Damage(int value)
{
    nowHp_ -= value;
    if (nowHp_ < 0)    nowHp_ = 0;
}

//回復
void Tachometer::Recovery(int value)
{
    nowHp_ += value;
    if (nowHp_ > maxHp_) nowHp_ = maxHp_;
}
