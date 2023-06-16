#pragma once
#include "Engine/GameObject.h"
#include <chrono>

class Particle;
class Ground;
class VehicleWheel;

//車両を管理するクラス
class Vehicle : public GameObject
{
protected:
    //モデル番号系
        int hModel_;    //モデル番号
        int hGroundModel_;  //地面のモデル番号
        int hWheelModel_;

    //重要ベクトル系
        //加速度
        XMVECTOR acceleration_;
        //それぞれの軸の単位ベクトル　定数
        const XMVECTOR VectorX_;    //X軸単位ベクトル
        const XMVECTOR VectorY_;    //Y軸単位ベクトル
        const XMVECTOR VectorZ_;    //Z軸単位ベクトル
        //車両から見た各軸の単位ベクトル
        XMVECTOR vehicleVectorX_;   //車両から見たX軸単位ベクトル
        XMVECTOR vehicleVectorY_;   //車両から見たY軸単位ベクトル
        XMVECTOR vehicleVectorZ_;   //車両から見たZ軸単位ベクトル

    //移動、回転の速さ
    float moveSPD_;
    float rotateSPD_;
    float jumpForce_;

    const float gravity_;  //重力 定数
    float speedLimit_;  //加速度の制限

    //ハンドル関係
        float handleRotate_;    //ハンドルの回転（単位：度）90゜〜 -90゜くらい？
        float handleRotateMax_; //ハンドルの限界
        bool handleFlag_;       //ハンドルを動かしていたら true
        //回転するときの調整
        float turnAdjust_;
        float driveAdjust_;
        float handleAdjust_;

        float slideHandleRotateAdd_; //滑るときのハンドル速度の追加
        float slideHandleAngleLimitAdd_; //滑るときのハンドル角度の追加

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

    //レース系
        unsigned long long time_; //経過タイム
        int pointCount_;    //チェックポイント経過数
        int pointCountMax_; //必要なチェックポイント経過数
        int lapCount_;      //周回数
        int lapMax_;        //必要な周回数
        bool goalFlag_;     //trueならゴール状態
        int ranking_;       //自分の順位
        int population_;    //レースに参加している人数

    float mass_;     //車の重量
    float engineRotate_;    //エンジン回転数

    XMVECTOR frontVec_;//正面のベクトル

    int landingType_; //地面のタイプ

    Particle* pParticle_;//エフェクト用

    Ground* pGround_;//地面のポインター

    VehicleWheel* pWheels_;//タイヤのポインター

    float wheelSpeedAdd_; //加速度に加算するタイヤの回転

    short accZDirection_; //前向きに進んでるか後ろ向きか。前：+1, 後：-1

    float wheelParticleLength_;//タイヤのエフェクトが発生するベクトルの長さ

    XMFLOAT3 startPosition_;    //開始時の位置
    XMFLOAT3 startRotate_;      //開始時の回転
    std::string vehicleModelName_;  //車両モデルの名前
    std::string wheelModelName_;    //タイヤモデルの名前

    Transform startTransform_;  //スタート状態の位置と回転

    //車両の各サイズ
    struct
    {
        float toRight_;   //右端　までの距離
        float toLeft_;    //左端　までの距離
        float toFront_;   //前方　までの距離
        float toRear_;    //後方　までの距離
        float toTop_;     //上端　までの距離
        float toCenter_;  //上端までの距離の１／２
        float rightToLeft_;     //右端　から　左端　までの距離
        float frontToRear_;     //前方　から　後方　までの距離
        float toFrontRight_;  //右斜め前　までの距離
        float toFrontLeft_;   //左斜め前　までの距離
        float toRearRight_;   //右斜め後ろ　までの距離
        float toRearLeft_;    //左斜め後ろ　までの距離
        XMFLOAT3 wheelFR_;  //右斜め前　タイヤの位置
        XMFLOAT3 wheelFL_;  //左斜め前　タイヤの位置
        XMFLOAT3 wheelRR_;  //右斜め後ろ　タイヤの位置
        XMFLOAT3 wheelRL_;  //左斜め後ろ　タイヤの位置
        float wheelHeight_; //タイヤの高さ
    }Size;

    const short handleRight_;
    const short handleLeft_;

    //レイキャストの時つかう
    enum Direction
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    };

public:
    //コンストラクタ
    Vehicle(GameObject* parent);

    //継承用
    Vehicle(GameObject* parent, const std::string& name);

    //デストラクタ
    virtual ~Vehicle(); //ヴァーチャル

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

    //壁や床との接触
    void VehicleCollide();

    /// <summary>
    /// 床があるか調べ、落下や接地
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    /// <param name="type">モデルの種類</param>
    /// <returns>なにかに当たった場合true</returns>
    bool Landing(int hModel , int type);

    /// <summary>
    /// 壁との接触
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    void CollideWall(int hModel, int type);

    /// <summary>
    /// タイヤを用意
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    void MakeWheels(int hModel);

    //タイヤの高さセッター
    void SetWheelHeight(float height) { Size.wheelHeight_ = height; }
    //モデルの大きさセッター
    void SetVehicleSize(int hModel);

    //車両の操作、入力の受付
    virtual void InputOperate();



    //ハンドルの操作
    void HandleTurnLR(int LR);

    //スタートの位置
    void SetStartTransform(Transform value) { startTransform_ = value; }

    //順位判定系セッター・ゲッター
        //チェックポイント通過数を取得
        int GetPointCount() { return pointCount_; }
        //必要なチェックポイント経過数をセット
        void SetPointCountMax(int value) { pointCountMax_ = value; }
        //周回数を取得
        int GetLapCount() { return lapCount_; }
        //必要な周回数を取得
        void SetLapMax(int value) { lapMax_ = value; }
        //次のチェックポイントの位置を取得　ただし配列をオーバーしてたら０の位置を返す
        XMFLOAT3* GetNextCheckPosition();
        //次のチェックポイントまでの距離を取得
        float GetNextCheckDistance();
        //順位をセット
        void SetRanking(int value) { ranking_ = value; }
        //レース参加人数をセット
        void SetPopulation(int value) { population_ = value; }

    //プレイヤー限定で実行する関数  
        //UIの初期化
        virtual void PlayerUI_Initialize();
        //UIの表示
        virtual void PlayerUI_Draw();
        //UIの情報更新
        virtual void PlayerUI_Update();
        //カメラの用意
        virtual void PlayerCamera_Initialize();
        //エフェクトを表示(重くなるのでNPCはやらない)
        virtual void PlayerParticle();
};