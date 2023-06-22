#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//車両のタイヤを管理するクラス
//タイヤの数は4個限定でまとめて管理する
class VehicleWheel : public GameObject
{
    int hModel_;    //モデル番号
    Vehicle* pVehicle_;  //自分がくっついてる車両のポインター

    float rotateSpeedX_;    //加速度に合わせて回転させるタイヤのX軸
    float handleRotateY_;   //ハンドル角度に合わせて回転させるタイヤのY軸

    struct
    {
        float wheelHeight_; //タイヤの高さf
        XMFLOAT3 posFR_;  //右斜め前　タイヤの位置
        XMFLOAT3 posFL_;  //左斜め前　タイヤの位置
        XMFLOAT3 posRR_;  //右斜め後ろ　タイヤの位置
        XMFLOAT3 posRL_;  //左斜め後ろ　タイヤの位置

        XMVECTOR cToFR_; //中央から右前　までのベクトル
        XMVECTOR cToFL_; //中央から左前　までのベクトル
        XMVECTOR cToRR_; //中央から右後ろ　までのベクトル
        XMVECTOR cToRL_; //中央から左後ろ　までのベクトル

        XMVECTOR centerToLeft_;     //モデルの中心から左までのベクトル
        XMVECTOR centerToRight_;    //モデルの中心から右までのベクトル
        XMVECTOR centerToBottom_;   //モデルの中心から底までのベクトル
    }Wheels;

public:
    //コンストラクタ
    VehicleWheel(GameObject* parent);

    //デストラクタ
    ~VehicleWheel();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    /// <summary>
    /// タイヤの情報いろいろのセッター　かつ　タイヤの高さを計算して車両に教える
    /// </summary>
    /// <param name="pVehicle">くっつく車両のポインタ</param>
    /// <param name="hModel">モデル番号</param>
    /// <param name="PosFL">左前　タイヤの位置</param>
    /// <param name="PosFR">右前　タイヤの位置</param>
    /// <param name="PosRL">左後ろ　タイヤの位置</param>
    /// <param name="PosRR">右後ろ　タイヤの位置</param>
    void SetVehicleWheel(Vehicle* pVehicle, int hModel
        , XMFLOAT3 PosFL, XMFLOAT3 PosFR, XMFLOAT3 PosRL, XMFLOAT3 PosRR);

    //タイヤの高さを計算して車両に教える
    void calculateHeight();

    /// <summary>
    /// タイヤの回転速度とハンドル角度をセット
    /// </summary>
    /// <param name="speed">タイヤのX軸回転速度（角速度）</param>
    /// <param name="rotate">タイヤのY軸回転角度（ハンドル角度に合わせる）</param>
    void SetWheelSpeedRotate(float speed, float rotate)
    {
        rotateSpeedX_ = speed;
        handleRotateY_ = rotate;
    }


};