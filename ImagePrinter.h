#pragma once
#include <unordered_map>
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class ImagePrinter : public GameObject
{
    struct ImageData
    {
        int imageHandle_;
        Transform imageTransform_;
        int imageAlpha_;  //0 ~ 255

        Transform changeTransform_;
        int changeAlpha_;

        ImageData()
        {
            imageHandle_ = -1;
            imageAlpha_ = 255;
            changeAlpha_ = 0;
        }
        ImageData(int handle, Transform transform, Transform cTransform, int alpha = 255, int cAlpha = 0)
        {
            imageHandle_ = handle;
            imageTransform_ = transform;
            imageAlpha_ = alpha;
            changeTransform_ = cTransform;
            changeAlpha_ = cAlpha;
        }
    }imageData_;

    //�摜
    std::unordered_map<std::string, ImageData> imageMap_;


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