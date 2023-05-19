#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class Speedometer : public GameObject
{
    std::vector<int> hImage_;

    float speed_;   //受け取ったスピード(0 ~ 360)

    //使用する画像の種類
    enum
    {
        IMAGE_FRAME,
        IMAGE_NEEDLE,
        IMAGE_MAX
    }SpeedImage_;

public:
    //コンストラクタ
    Speedometer(GameObject* parent);

    //デストラクタ
    ~Speedometer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;


    //セッター
    void SetSpeed(int value)
    {
        speed_ = value;
    }
};