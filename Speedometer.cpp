#include "Speedometer.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

using std::string;
using std::vector;

//�R���X�g���N�^
Speedometer::Speedometer(GameObject* parent)
    :GameObject(parent, "Speedometer")
    , speed_(0.0f)
{
    hImage_.clear();
}

//�f�X�g���N�^
Speedometer::~Speedometer()
{
}

//������
void Speedometer::Initialize()
{
    //�Q�[�W�̃T�C�Y�i�摜�T�C�Y����j
    transform_.scale_ = XMFLOAT3(0.25f, 0.25f, 1.0f);

    //�Q�[�W�̈ʒu�i��ʒ��S����j
    transform_.position_ = XMFLOAT3(-0.85f, -0.75f, 0);

    //�摜���[�h
    string header = "image\\";
    vector<string>imageName = { "mator4.png","matorNeedle.png"};
    for (int i = 0; i < imageName.size(); i++)
    {
        hImage_.push_back(Image::Load(header + imageName[i]));
        assert(hImage_[i] >= 0);
    }

}

//�X�V
void Speedometer::Update()
{
    speed_ += 1;
}

//�`��
void Speedometer::Draw()
{
    Transform meterTrans;

    //�g
    meterTrans = transform_;

    Image::SetAlpha(hImage_[IMAGE_FRAME], 255);
    //Image::SetRect(hImage_[IMAGE_FRAME], 2, 50, 3, 20);

    Image::SetTransform(hImage_[IMAGE_FRAME], meterTrans);
    Image::Draw(hImage_[IMAGE_FRAME]);

    //�j
    Transform needleTrans = transform_;
    needleTrans.rotate_.z = -speed_;

    Image::SetTransform(hImage_[IMAGE_NEEDLE], needleTrans);
    Image::Draw(hImage_[IMAGE_NEEDLE]);
}

//�J��
void Speedometer::Release()
{
}