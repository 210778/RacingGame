#pragma once
#include <unordered_map>
#include "Engine/GameObject.h"

//�摜���Ǘ�����N���X
class ImagePrinter : public GameObject
{
    struct ImageData
    {
        int imageHandle_;   //�摜�ԍ�
        Transform imageTransform_;  //�摜�g�����X�t�H�[��
        int imageAlpha_;  //�s�����x�@�@�����@0 ~ 255�@�s����

        int lifeTime_;   //�\�����邷�鎞��(�P�ʁF�t���[��)
        int life_;       //�\�����Ă��鎞��

        bool isPrint_;   //�\�����邩�ǂ���

        Transform changeTransform_; //�g�����X�t�H�[���̕ω�
        int changeAlpha_;   //�s�����x�̕ω�

        ImageData()
        {
            imageHandle_ = -1;
            imageAlpha_ = 255;
            lifeTime_ = 0;
            life_ = 0;
            isPrint_ = false;
            changeAlpha_ = 0;
        }
        ImageData(int handle, Transform transform,int time
            , Transform cTransform, int alpha = 255, int cAlpha = 0)
        {
            imageHandle_ = handle;
            imageTransform_ = transform;
            imageAlpha_ = alpha;
            lifeTime_ = time;
            life_ = 0;
            isPrint_ = false;
            changeTransform_ = cTransform;
            changeAlpha_ = cAlpha;
        }
    }imageData_;

    enum class ImageNumber
    {
        start,
        one,
        two,
        three,
        goal
    };

    //�摜
    std::unordered_map<ImageNumber, ImageData> imageMap_;



public:
    //�R���X�g���N�^
    ImagePrinter(GameObject* parent);

    //�f�X�g���N�^
    ~ImagePrinter();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};