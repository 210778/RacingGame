#pragma once
#include "Engine/GameObject.h"

class Particle;
class VehicleWheel;
class RayCastData;
class Viewer;

//車両を管理するクラス
class Vehicle : public GameObject
{
protected:
    //モデル番号系
        int hModel_;    //モデル番号
        int hGroundModel_;  //地面のモデル番号
        int hWheelModel_;

    //重要ベクトル系
        //ベクトル３つ
        struct TriVector
        {
            XMVECTOR x, y, z;
            //ゼロで初期化コンストラクタ
            TriVector() {
                x = { 0.0f,0.0f,0.0f,0.0f };
                y = { 0.0f,0.0f,0.0f,0.0f };
                z = { 0.0f,0.0f,0.0f,0.0f };}
            //引数ありコンストラクタ
            TriVector(XMVECTOR vecX,XMVECTOR vecY,XMVECTOR vecZ) {
                x = vecX;
                y = vecX;
                z = vecX;}
            //セッター
            void Set(XMVECTOR vecX, XMVECTOR vecY, XMVECTOR vecZ) {
                x = vecX;
                y = vecY;
                z = vecZ;}
        };
        //加速度
        XMVECTOR acceleration_;
        //それぞれの軸の単位ベクトル　定数
        TriVector worldVector_;
        //車両から見た各軸の単位ベクトル
        TriVector vehicleVector_;

    //回転用行列
    XMMATRIX matRotateX;
    XMMATRIX matRotateY;
    XMMATRIX matRotateZ;

    XMMATRIX matRotateX_R;
    XMMATRIX matRotateY_R;
    XMMATRIX matRotateZ_R;


    //移動、回転の速さ
    float moveSPD_;
    float rotateSPD_;
    float jumpForce_;

    const float gravity_;  //重力 定数

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
        float iceFriction_;         //氷摩擦

        bool  slideFlag_;           //走行、滑走
        float sideWheelFriction_;   //横向きの摩擦
        float sideSlideFriction_;   //横向き空中摩擦
        float sideIceFriction_;     //横向き氷摩擦

        float landingFriction_;
        float sideFriction_;

        //地面のタイプによる摩擦のまとめ
        struct GroundTypeValue
        {
            float acceleration;   //加速力に乗算される値
            float landing;    //走行摩擦
            float side;       //カーブ中に横向きに押す力

            //引数なしコンストラクタ
            GroundTypeValue(){
                acceleration = 1.0f;
                landing = 0.99f;
                side = 0.2f;
            }
            //引数ありコンストラクタ
            GroundTypeValue(float _acceleration, float _landing, float _side) {
                acceleration = _acceleration;
                landing = _landing;
                side = _side;
            }
        };
        std::map<int, GroundTypeValue> GroundTypeFriction_;    //地面のタイプによる摩擦のまとめ

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
        int goalRanking_;   //ゴールした時の順位 ゴール前は0
        int population_;    //レースに参加している人数
        bool lapCountFlag_; //周回数が増えた時のフラグ
        unsigned long long goalTime_;//ゴールした時の経過タイム
        unsigned long long standbyTime_;    //待機する時間

    float mass_;            //車の重量

    int landingType_; //地面のタイプ

    Particle* pParticle_;//エフェクト用
    VehicleWheel* pWheels_;//タイヤのポインター

    float wheelSpeedAdd_; //加速度に加算するタイヤの回転

    short accZDirection_; //前向きに進んでるか後ろ向きか。前：+1, 後：-1

    float wheelParticleLength_;     //タイヤのエフェクトが発生するベクトルの長さ
    float wheelParticleLengthMax_;  //タイヤのエフェクトが発生するベクトルの長さの上限
    float sparkParticleLength_;     //ドリフトのエフェクトが発生するベクトルの長さ
    float sparkParticleHanlde_;     //ドリフトのエフェクトが発生するハンドルの回転
    int npcParticleRandom_;         //NPCがエフェクトを発生させない確率

    std::string vehicleModelName_;  //車両モデルの名前
    std::string wheelModelName_;    //タイヤモデルの名前

    Transform startTransform_;      //スタート状態の位置と回転
    Transform restartTransform_;    //復活した時の位置と回転

    float slopeLimitAngle_; //坂道を上るときの上限角度（水平線から見る）この値を超えるとバグりがち

    float wallReflectionForce_; //壁にぶつかったときの減速する値

    const short handleRight_;   //右を表す定数
    const short handleLeft_;    //左を表す定数

    float boostCapacityMax_;    //ブーストできる最大容量
    float boostCapacity_;   //ブーストできる現在残量
    float boostSpending_;   //ブーストする時の消費量
    float boostIncrease_;   //ブーストできる残量の追加値
    float boostValue_;      //ブーストしたときに加算する値

    bool isPlayer_;         //プレイヤーキャラかどうか
    float toPlayerVehicleLength_;   //NPCから見たプレイヤーまでの距離
    float particleLimitLength_;     //NPCがこの距離より離れてるとエフェクトを表示しない
    float collideBoxValue_;         //相手とぶつかったときの掛ける値
    bool isOperationInvalid_;       //操作を無効化する
    bool pauseFlag_;        //ポーズ中かどうか

    XMFLOAT3 farLimitPosition_; //xyzそれぞれがこの値(絶対値)を超えると位置リセット

    //カメラ
    Viewer* pViewer_;

    //車両の各サイズ
    struct VehicleSize
    {
        float toRight_ = 1.0f;  //右端　までの距離
        float toLeft_ = 1.0f;   //左端　までの距離
        float toFront_ = 1.0f;  //前方　までの距離
        float toRear_ = 1.0f;   //後方　までの距離
        float toTop_ = 1.0f;    //上端　までの距離
        float toBottom_ = 1.0f; //下端　までの距離
        float rightToLeft_ = 1.0f;  //右端　から　左端　までの距離
        float topToBottom_ = 1.0f;  //上端　から　下端　までの距離
        float frontToRear_ = 1.0f;  //前方　から　後方　までの距離
        float toFrontRight_ = 1.0f; //右斜め前　までの距離
        float toFrontLeft_ = 1.0f;  //左斜め前　までの距離
        float toRearRight_ = 1.0f;  //右斜め後ろ　までの距離
        float toRearLeft_ = 1.0f;   //左斜め後ろ　までの距離     
        float angleFrontRight_ = 0.0f; //前方から　右斜め前　までの角度
        float angleFrontLeft_ = 0.0f;  //前方から　左斜め前　までの角度
        float angleRearRight_ = 0.0f;  //前方から　右斜め後ろ　までの角度
        float angleRearLeft_ = 0.0f;   //前方から　左斜め後ろ　までの角度
        XMFLOAT3 wheelFR_;      //右斜め前　タイヤの位置
        XMFLOAT3 wheelFL_;      //左斜め前　タイヤの位置
        XMFLOAT3 wheelRR_;      //右斜め後ろ　タイヤの位置
        XMFLOAT3 wheelRL_;      //左斜め後ろ　タイヤの位置
        float wheelHeight_ = 0.1f;      //タイヤの高さ
        float wheelRemainder_ = 1.0f;   //車体とタイヤの位置の差分の高さ
        float toWheelBottom_ = 1.0f;    //中心からタイヤの底辺までの高さ
        float topToWheelBottom_ = 1.0f; //上端からタイヤ底辺までの高さ
        float centerRightToLeft_ = 1.0f;    //左右の長さの中心
        float centerTopToBottom_ = 1.0f;    //上下の長さの中心(タイヤの高さ込み)
        float centerFrontToRear_ = 1.0f;    //前後の長さの中心
        float centerPositionRemainder_ = 1.0f; //衝突ボックスのY軸の中心までの距離
    }Size;

    //操作入力
    struct Operation
    {
        enum class Button     //ボタン入力　false ~ true
        {
            boost = 0,

            jump,

            ButtonMax
        };
        enum class Value    //スティック系入力　-1.0 ~ 0.0 ~ 1.0
        {
            moveFrontRear = 0,
            handleRightLeft,

            turnRightLeft,
            moveRightLeft,

            ValueMax
        };

        std::map<Button, bool> ButtonMap;    //ボタン入力　false ~ true
        std::map<Value, float> ValueMap;     //スティック系入力　-1.0 ~ 0.0 ~ 1.0

        //現在の値を過去に渡してゼロにする
        void Refresh()
        {
            //リセット
            for (auto& itr1 : ButtonMap)
            {
                itr1.second = false;
            }
            for (auto& itr2 : ValueMap)
            {
                itr2.second = 0.0f;
            }
        }
    }operation_;

    //NPCのためのレイが当たったかどうかの情報まとめ
    struct RayCastHit
    {
        XMFLOAT3	dir;	//レイの向きfloat3
        float       dist;	//衝突点までの距離
        BOOL        hit;	//レイが当たったか
        XMFLOAT3    end;	//命中位置
        bool        road;   //レイが当たったときに壁なのか坂道なのか

        enum Number
        {
            up,
            down,
            front,
            frontLeft,
            left,
            rearLeft,
            rear,
            rearRight,
            right,
            frontRight,
        };
    };
    std::map<int, RayCastHit> rayCastHit_;

    //レイキャストの時用
    enum Direction
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    };
    enum Oblique
    {
        frontLeft = 0,
        frontRight,
        rearLeft,
        rearRight
    };

    //壁や床との接触
    void VehicleCollide();

    /// <summary>
    /// 床があるか調べ、落下や接地
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    /// <param name="type">モデルの種類</param>
    void Landing(int hModel, int type);

    /// <summary>
    /// 壁との接触
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    void CollideWall(int hModel, int type);

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

    //加速度を取得
    XMVECTOR GetAcceleration() { return acceleration_; };

    //加速度の制限 vector
    void SpeedLimit(XMVECTOR& speed, const float limit);
    //加速度の制限 xmfloat3
    void SpeedLimit(XMFLOAT3& speed, const float limit);

    //ハンドル角度の制限のための
    //（変更したい角度、最大角度）
    //変更したい角度の絶対値が最大角度の絶対値を超えていたら丸め込む
    void AngleLimit(float& angle, const float limit);

    //タイヤを回転させるときの物理
    void TurnWheel();

    /// <summary>
    /// タイヤを用意
    /// </summary>
    /// <param name="hModel">モデル番号</param>
    void MakeWheels(int hModel);

    //タイヤの高さセッター //車体との差分も計算
    void SetWheelHeight(float height);

    //モデルの大きさセッター
    void SetVehicleSize(int hModel);

    //車両の操作、入力の受付
    virtual void InputOperate();

    //地面との作用 ,奈落や範囲外に落ちるとリセット
    void LandingProcess();

    //ハンドルの操作
    void HandleTurnLR(int LR);
    //ハンドルの値に合わせて回転
    void HandleRotate();

    //スタートの位置
    void SetStartTransform(const Transform& value) { startTransform_ = value; }

    //ベクトルを回転行列で回転 (ZXY順)
    void VectorRotateMatrixZXY(XMVECTOR& vec);

    //ベクトルを回転行列で"逆"回転 (ZXY順)
    void VectorRotateMatrixZXY_R(XMVECTOR& vec);

    //ゴール判定
    void GoalJudgement();

    //周回数増加
    void LapCountAdd(int value = 1);

    /// <summary>
    /// 坂道に応じて車両を回転(X、Z軸)
    /// </summary>
    /// <param name="normal">坂道の法線（正規化済）</param>
    /// <param name="limitAngle">回転を許容する坂道の角度(水平線から見る)</param>
    /// <param name="rayCastType">NPC用のレイキャストの種類</param>
    /// <returns>限界角度以内なら回転してtrue そうでないなら何もせずfalse</returns>
    bool VehicleRotateSlope(const XMVECTOR& normal, float limitAngle, int rayCastType);

    /// <summary>
    /// NPC用レイキャストにデータセット
    /// </summary>
    /// <param name="number">mapのキー</param>
    /// <param name="rcd">レイキャストデータ</param>
    void SetRayCastHit(int number, const RayCastData& rcd);

    //NPCのレイキャスト用
    void SetRayCastSlope(int number, bool flag);

    /// <summary>
    /// BoundingOrientedBoxの衝突判定
    /// </summary>
    /// <param name="pVehicle">当たった車両クラスのポインタ</param>
    void CollideBoundingBox(Vehicle* pVehicle);

    //操作入力の反映
    void InputReceive(const XMVECTOR& vecX, const XMVECTOR& vecZ);

    //エフェクト
    void VehicleParticle();
    
    //奈落に落下 / フィールド外に出た　時に位置と状態をリセット
    void FallAbyss();

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
        //バウンディングボックスの大きさのゲッター
        XMFLOAT3 GetBoundingBoxExtents() { return XMFLOAT3{ Size.centerRightToLeft_
                                                          , Size.centerTopToBottom_
                                                          , Size.centerFrontToRear_ }; }
        //バウンディングボックスの中心の位置のゲッター
        XMFLOAT3 GetBoundingBoxCenter() {
            return XMFLOAT3{ transform_.position_.x
                            ,transform_.position_.y + Size.centerPositionRemainder_
                            ,transform_.position_.z };
        }
        //車両の質量のゲッター
        float GetMass() { return mass_; };
        //車両の質量のセッター
        void SetMass(float value) { mass_ = value; };
        //ゴールしてるかどうか
        bool GetGoalFlag() { return goalFlag_; };
        //ゴールしてるならtrueとポインタに順位を、してないならfalseを返してなにもしない
        bool GetIsGoalRanking(int* pRank)
        {
            if (goalFlag_)
            {
                *pRank = goalRanking_;
                return true;
            }
            return false;
        }
        //経過時間のセッター
        void SetTime(unsigned long long time) { time_ = time; }
        //操作を無効化するかどうかのセッター
        void SetOperationInvalid(bool value) { isOperationInvalid_ = value; }
        //待機時間のセッター
        void SetStandbyTime(unsigned long long time) { standbyTime_ = time; }
        //ポーズ状態のセッター
        void SetPauseFlag(bool flag) { pauseFlag_ = flag; }
        //サイズのゲッター
        VehicleSize* GetVehicleSize() { return &Size; }
        //NPCから見たプレイヤーまでの距離
        void SetToPlayerVehicleLength(float value) { toPlayerVehicleLength_ = value; }


    //プレイヤー限定で実行する関数  
        //UIの初期化
        virtual void PlayerUI_Initialize();
        //UIの表示
        virtual void PlayerUI_Draw();
        //UIの情報更新
        virtual void PlayerUI_Update();
        //カメラの用意
        virtual void PlayerCamera_Initialize();
        //カメラ更新
        virtual void PlayerCamera_Update();
};