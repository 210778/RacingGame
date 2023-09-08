#pragma once
#include <unordered_map>
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
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

    //画像
    std::unordered_map<std::string, ImageData> imageMap_;


public:
    //コンストラクタ
    ImagePrinter(GameObject* parent);

    //デストラクタ
    ~ImagePrinter();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};