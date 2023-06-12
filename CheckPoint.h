#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class CheckPoint : public GameObject
{
    int number_;    //番号
public:
    //コンストラクタ
    CheckPoint(GameObject* parent);

    //デストラクタ
    ~CheckPoint();

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

    //セッターとゲッター
    void SetNumber(int value) { number_ = value; };
    int GetNumber() { return number_; };

    //当たり判定を作る
    void MakeSphereCollider(XMFLOAT3 position, float size);

};