#include "Speedometer.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

using std::string;
using std::vector;

//コンストラクタ
Speedometer::Speedometer(GameObject* parent)
    :GameObject(parent, "Speedometer")
    , speed_(0.0f)
{
    hImage_.clear();
}

//デストラクタ
Speedometer::~Speedometer()
{
}

//初期化
void Speedometer::Initialize()
{
    //ゲージのサイズ（画像サイズが基準）
    transform_.scale_ = XMFLOAT3(0.25f, 0.25f, 1.0f);

    //ゲージの位置（画面中心が基準）
    transform_.position_ = XMFLOAT3(-0.85f, -0.75f, 0);

    //画像ロード
    string header = "image\\";
    vector<string>imageName = { "mator4.png","matorNeedle.png"};
    for (int i = 0; i < imageName.size(); i++)
    {
        hImage_.push_back(Image::Load(header + imageName[i]));
        assert(hImage_[i] >= 0);
    }

}

//更新
void Speedometer::Update()
{
    speed_ += 1;
}

//描画
void Speedometer::Draw()
{
    Transform meterTrans;

    //枠
    meterTrans = transform_;

    Image::SetAlpha(hImage_[IMAGE_FRAME], 255);
    //Image::SetRect(hImage_[IMAGE_FRAME], 2, 50, 3, 20);

    Image::SetTransform(hImage_[IMAGE_FRAME], meterTrans);
    Image::Draw(hImage_[IMAGE_FRAME]);

    //針
    Transform needleTrans = transform_;
    needleTrans.rotate_.z = -speed_;

    Image::SetTransform(hImage_[IMAGE_NEEDLE], needleTrans);
    Image::Draw(hImage_[IMAGE_NEEDLE]);
}

//開放
void Speedometer::Release()
{
}