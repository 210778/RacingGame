#include "Tachometer.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

using std::string;
using std::vector;

//�R���X�g���N�^
Tachometer::Tachometer(GameObject* parent)
    :GameObject(parent, "Tachometer"), hModel_(-1), hImage_(-1)
    , hPict_{ -1,-1,-1 }
    ,stopRatio_((float)nowHp_ / (float)maxHp_), moveRatio_(stopRatio_)
    ,nowHp_(180)      //����HP �i�K�X�ύX�j
    ,maxHp_(250)      //MaxHP�i�K�X�ύX�j
    ,SPEED(0.005f)     //�o�[�̈ړ����x�i���D�݂Łj
{
}

//�f�X�g���N�^
Tachometer::~Tachometer()
{
}

//������
void Tachometer::Initialize()
{
    hModel_ = Model::Load("model\\red_arrow.fbx");
    assert(hModel_ >= 0);

    //�Q�[�W�̃T�C�Y�i�摜�T�C�Y����j
    transform_.scale_ = XMFLOAT3(1.5f, 1.5f, 1.0f);

    //�Q�[�W�̈ʒu�i��ʒ��S����j
    transform_.position_ = XMFLOAT3(-0.0f, 0.88f, 0);

    //�摜���[�h
    string header = "image\\";
    vector<string>modelName = { "Gauge_Frame.png","Gauge_Green.png","Gauge_LightGreen.png","Gauge_Red.png" };
    for (int i = 0; i < modelName.size(); i++)
    {
        hPict_[i] = Image::Load(header + modelName[i]);
        assert(hPict_[i] >= 0);
    }

}

//�X�V
void Tachometer::Update()
{
    //�ő�HP�ɑ΂���A����HP�̊���
    stopRatio_ = (float)nowHp_ / (float)maxHp_;

    //�o�[�𓮂���
    if (moveRatio_ < stopRatio_) moveRatio_ += SPEED;
    if (moveRatio_ > stopRatio_) moveRatio_ -= SPEED;
}

//�`��
void Tachometer::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);


    Transform gaugeTrans;

    //�g
    //gaugeTrans = transform_;
    //Image::SetTransform(hPict_[PICT_FRAME], gaugeTrans);
    //Image::Draw(hPict_[PICT_FRAME]);

    //HP�ω��Ȃ�
    if (abs(moveRatio_ - stopRatio_) < SPEED)
    {
        //�΃Q�[�W
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }


    //�񕜒�
    else if (moveRatio_ < stopRatio_)
    {
        //���΃Q�[�W
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_LGREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_LGREEN]);

        //�΃Q�[�W
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= moveRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }

    //�_���[�W��
    else
    {
        //�ԃQ�[�W
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= moveRatio_;
        Image::SetTransform(hPict_[PICT_RED], gaugeTrans);
        Image::Draw(hPict_[PICT_RED]);

        //�΃Q�[�W
        gaugeTrans = transform_;
        gaugeTrans.scale_.x *= stopRatio_;
        Image::SetTransform(hPict_[PICT_GREEN], gaugeTrans);
        Image::Draw(hPict_[PICT_GREEN]);
    }


    //�g
    gaugeTrans = transform_;

    Image::SetAlpha(hPict_[PICT_FRAME], 128);
    Image::SetRect(hPict_[PICT_FRAME], 2, 50, 3, 20);

    Image::SetTransform(hPict_[PICT_FRAME], gaugeTrans);
    Image::Draw(hPict_[PICT_FRAME]);
}

//�J��
void Tachometer::Release()
{
}

//�_���[�W
void Tachometer::Damage(int value)
{
    nowHp_ -= value;
    if (nowHp_ < 0)    nowHp_ = 0;
}

//��
void Tachometer::Recovery(int value)
{
    nowHp_ += value;
    if (nowHp_ > maxHp_) nowHp_ = maxHp_;
}
