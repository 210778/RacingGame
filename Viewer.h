#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//◆◆◆を管理するクラス
class Viewer : public GameObject
{
    Vehicle* pVehicle;

    float cameraLength_;//モデルの表示用
    //移動、回転の速さ
    float rotateSPD_;
    //カメラが1回転しないための
    float upLim_;
    float downLim_;
    //ズームアップ、ダウンの限界と速さ
    float nearLim_;
    float farLim_;
    float zoomUp_;
    float zoomOut_;
    //カメラ用
    XMFLOAT3 camFlo_;
    float camX_;

    //カメラの慣性用
    float toFutureLength_;     //現在位置から未来位置へ移動する速度(割合)

    XMVECTOR camTarPlusVec_;    //カメラの焦点に追加するベクトル

    Transform viewTransform_;   //対象のトランスフォーム
    XMVECTOR  viewAcceleration_; //対象の加速度
    float     viewRotate_;       //対象の追加回転値
    float     viewRotateAdd_;    //↑にかける値

public:
    //コンストラクタ
    Viewer(GameObject* parent);

    //デストラクタ
    ~Viewer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    /// <summary>
    /// 視点情報をセット
    /// </summary>
    /// <param name="transform">見るもののトランスフォーム</param>
    /// <param name="speed">加速度</param>
    /// <param name="handle">追加で回転したい値</param>
    void SetViewValue(const Transform& transform,const XMVECTOR& speed, const float handle);

    void SetViewPosition();
};