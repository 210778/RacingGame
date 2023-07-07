#pragma once
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class MeasurePole : public GameObject
{
    int hModel_;    //モデル番号
public:
    //コンストラクタ
    MeasurePole(GameObject* parent);

    //デストラクタ
    ~MeasurePole();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    /// <summary>
    /// レイキャストの可視化
    /// </summary>
    /// <param name="start">スタート位置</param>
    /// <param name="direction">レイの方向</param>
    /// <param name="length">判定する長さ</param>
    void ViewRayCast(const XMFLOAT3* start, const XMVECTOR* direction, const float length);
};