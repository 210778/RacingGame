#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class Bullet : public GameObject
{
    int hModel_;    //モデル番号

    XMFLOAT3 acceleration_; //加速度

    int life_;//寿命

public:
    //コンストラクタ
    Bullet(GameObject* parent);

    //デストラクタ
    ~Bullet();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //何かに当たった
   //引数：pTarget 当たった相手
    void OnCollision(GameObject* pTarget) override;

    //加速度を位置に加算する。アップデートの最初においとく
    void Accelerator();

    //セッターとゲッター
    void SetSpeed(XMFLOAT3 speed)
    {
        acceleration_ = speed;
    }
    XMFLOAT3 GetSpeed()
    {
        return acceleration_;
    }
};