#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//◆◆◆を管理するクラス
class Viewer : public GameObject
{
    //Vehicle* pVehicle;
    Vehicle* pVehicle;

    float cameraLength_;//モデルの表示用
    //移動、回転の速さ
    //float moveSPD_;
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
    float camX;

    //カメラの慣性用
    float toFutureLength_;     //現在位置から未来位置へ移動する速度(割合)

    XMVECTOR camTarPlusVec;    //カメラの焦点に追加するベクトル

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
};