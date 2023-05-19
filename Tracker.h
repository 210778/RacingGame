#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class Tracker : public GameObject
{
    int hModel_;    //モデル番号
    
    int life_;  //寿命

public:
    //コンストラクタ
    Tracker(GameObject* parent);

    //デストラクタ
    ~Tracker();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void SetLife(int seconds)
    {
        life_ = seconds * 60;
    }
};