#include "Engine/Image.h"
#include "Engine/Global.h"

#include "ImagePrinter.h"


//�R���X�g���N�^
ImagePrinter::ImagePrinter(GameObject* parent)
    :GameObject(parent, "ImagePrinter")

{
}

//�f�X�g���N�^
ImagePrinter::~ImagePrinter()
{
}

//������
void ImagePrinter::Initialize()
{
    Transform trans;
    Transform change;
    change.scale_ = { 0.05f, 0.05f, 0.05f };
    int cAlpha = -5;

    SetImage(ImageNumber::start, Image::Load("image\\count_start.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);

    SetImage(ImageNumber::one, Image::Load("image\\count_1.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);

    SetImage(ImageNumber::two, Image::Load("image\\count_2.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);

    SetImage(ImageNumber::three, Image::Load("image\\count_3.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);

    SetImage(ImageNumber::goal, Image::Load("image\\count_goal.png"), trans
        , Global::GetStandardFPS(), change, 255, cAlpha);
}

//�X�V
void ImagePrinter::Update()
{
    for (auto& itr : imageMap_)
    {
        //�\�����Ȃ�����
        if (itr.second.life_ >= itr.second.lifeTime_)
        {
            itr.second.isPrint_ = false;
        }
    }
}

//�`��
void ImagePrinter::Draw()
{
    for (auto& itr : imageMap_)
    {
        //�\������
        if (itr.second.isPrint_)
        {
            //�g�����X�t�H�[��
            Transform tra;
            tra.position_ = Transform::Float3Add(itr.second.imageTransform_.position_
                , Transform::Float3Times(itr.second.changeTransform_.position_, itr.second.life_));
            tra.rotate_ = Transform::Float3Add(itr.second.imageTransform_.rotate_
                , Transform::Float3Times(itr.second.changeTransform_.rotate_, itr.second.life_));
            tra.scale_ = Transform::Float3Add(itr.second.imageTransform_.scale_
                , Transform::Float3Times(itr.second.changeTransform_.scale_, itr.second.life_));
            Image::SetTransform(itr.second.imageHandle_, tra);
            //�����x
            int alpha = itr.second.imageAlpha_ + (itr.second.changeAlpha_ * itr.second.life_);
            Image::SetAlpha(itr.second.imageHandle_, alpha);

            Image::Draw(itr.second.imageHandle_);

            //���Z
            itr.second.life_++;
        }
    }
}

//�J��
void ImagePrinter::Release()
{
}

//�摜���Z�b�g
void ImagePrinter::SetImage(ImageNumber in, int handle, Transform transform
                          , int time, Transform cTransform, int alpha, int cAlpha)
{
    imageMap_[in].imageHandle_ = handle;
    imageMap_[in].imageTransform_ = transform;
    imageMap_[in].lifeTime_ = time;
    imageMap_[in].changeTransform_ = cTransform;
    imageMap_[in].imageAlpha_ = alpha;
    imageMap_[in].changeAlpha_ = cAlpha;
}

//�摜�`��
void ImagePrinter::DrawImage(ImageNumber in)
{
    imageMap_[in].isPrint_ = true;
    imageMap_[in].life_ = 0;
}