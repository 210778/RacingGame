#pragma once
#include <unordered_map>
#include "Engine/GameObject.h"

//画像を管理するクラス
class ImagePrinter : public GameObject
{
    struct ImageData
    {
        int imageHandle_;   //画像番号
        Transform imageTransform_;  //画像トランスフォーム
        int imageAlpha_;  //不透明度　　透明　0 ~ 255　不透明

        int lifeTime_;   //表示するする時間(単位：フレーム)
        int life_;       //表示している時間

        bool isPrint_;   //表示するかどうか

        Transform changeTransform_; //トランスフォームの変化
        int changeAlpha_;   //不透明度の変化

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

    //画像
    std::unordered_map<ImageNumber, ImageData> imageMap_;



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