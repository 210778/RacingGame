#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class Tachometer : public GameObject
{
    int hModel_;    //モデル番号
    int hImage_;

    
    int nowHp_; //現在のＨＰ
    int maxHp_; //最大ＨＰ

    float stopRatio_;       //最大HPに対する固定されたゲージの割合
    float moveRatio_;       //最大HPに対する変化してるゲージの割合
    const float SPEED;      //バーの速度

    //使用する画像の種類
    enum
    {
        PICT_FRAME,
        PICT_GREEN,
        PICT_LGREEN,
        PICT_RED,
        PICT_MAX
    };

    //画像番号
    int hPict_[PICT_MAX];

public:
    //コンストラクタ
    Tachometer(GameObject* parent);

    //デストラクタ
    ~Tachometer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //ダメージを受ける
    //引数：value  ダメージ量
    void Damage(int value);

    //回復
    //引数：value  回復量
    void Recovery(int value);
};