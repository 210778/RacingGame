#pragma once
#include "Engine/GameObject.h"

class Sample;
class Tachometer;
class Course;
class Speedometer;
class Text;
//class CircuitUnion;
class Particle;

//◆◆◆を管理するクラス
class Vehicle : public GameObject
{
    int hModel_;    //モデル番号
    int hGroundModel_;  //地面のモデル番号


    //XMFLOAT3 acceleration_; //加速度
    XMVECTOR acceleration_; //加速度

    //モデルの大きさ
    XMFLOAT3 vehicleSize_;
    XMFLOAT3 vehicleSizeHalf_;
    float vehicleSizeOblique_;//斜め

    //移動、回転の速さ
    float moveSPD_;
    float rotateSPD_;
    float jumpForce_;

    float gravity_;  //重力
    float speedLimit_;  //加速度の制限

    //ハンドル関係
    float handleRotate_;    //ハンドルの回転（単位：度）90゜～ -90゜くらい？
    float handleRotateMax_; //ハンドルの限界
    bool handleFlag_;       //ハンドルを動かしていたら true
    //回転するときの調整
    float turnAdjust_;
    float driveAdjust_;
    float handleAdjust_;

    float slideHandleRotateAdd_; //滑るときのハンドル速度の追加
    float slideHandleAngleLimitAdd_; //滑るときのハンドル角度の追加

    float wheelSpeed_;  //タイヤの回転　地面につくと減少する代わりに加速度を加算　アクセルで増加か
    XMVECTOR wheelDirection_; //使うかは不明

    //float frictionForce_;   //摩擦
    //地上・空中
    bool landingFlag_;          //着地、空中
    float wheelFriction_;       //タイヤ摩擦
    float airFriction_;         //空気摩擦
    float turfFriction_;        //芝生摩擦

    bool  slideFlag_;           //走行、滑走
    float sideFriction_;        //横向きの摩擦
    float sideSlideFriction_;   //横向き空中摩擦

    //弾丸系
    int coolTime_;  //弾丸用
    float bulletPower_;
    int heatAdd_;   //クールタイムに追加

    //レイキャスト系
    float rayStartHeight;   //上にレイキャストするときの高さ

    //テスト　デバッグ用
    Sample* pMarker;
    Tachometer* pTachometer;
    Speedometer* pSpeedometer;

    //文字シリーズ
    Text* pTextSpeed_;  //時速
    Text* pTextTime_;   //経過時間
    Text* pTextLap_;    //周回数
    Text* pTextEngine_; //エンジン回転数

    unsigned long long time_; //経過タイム

    int pointCount_;
    int lapCount_;
    bool goalFlag_;
    int hImage_;

    //追加変数
    float torque_;   //トルク
    float mass_;     //車の重量
    float engineRotate_;    //エンジン回転数
    bool clutchFlag_;   //クラッチが繋がってるか

    //サーキット用
    //CircuitUnion* pCircuitUnion;

    XMVECTOR frontVec_;//正面のベクトル

    int landingType_; //地面のタイプ

    Particle* pParticle;

public:
    //コンストラクタ
    Vehicle(GameObject* parent);

    //デストラクタ
    ~Vehicle(); //未来を見越してヴぁーチャル化 ??

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

    //加速度の制限 vector
    void SpeedLimit(XMVECTOR& speed, const float limit);
    //加速度の制限 xmfloat3
    void SpeedLimit(XMFLOAT3& speed, const float limit);

    //ハンドル角度の制限のための
    //（変更したい角度、最大角度）
    //変更したい角度の絶対値が最大角度の絶対値を超えていたら丸め込む
    void AngleLimit(float& angle, const float limit);

    //まとめたほうがいいかも
    void VehicleCollide();

    //接地
    void Landing();

    //壁との接触
    void CollideWall();
};