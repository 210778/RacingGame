#pragma once
#include "Vehicle.h"

class Text;
class Sample;
class Viewer;

//◆◆◆を管理するクラス
class VehiclePlayer : public Vehicle
{
    //文字
    Text* pText_;    //文字

    float km_hAdd;  //時速表示加算
    int   flashIntervalUI_;   //表示が点滅する間隔
    bool  IsFlashUI_;         //UIを表示するかどうか
    Sample* pSample_;

    //ブースト系
    int imageBoostMax_;
    int imageBoost_;
    Transform boostMaxTrans_;
    Transform boostTrans_;

    //スピードメーター
    int hImageSpeedFrame_;
    int hImageSpeedNeedle_;
    Transform SpeedmeterTransform_;

    unsigned long long drawTime_;//描画に使う時間の値
    int standardTime_;  //描画に使うFPSの値

    //カウントダウン表示
    struct ImageData
    {
        int imageHandle_;   //画像番号
        Transform imageTransform_;  //画像トランスフォーム
        int imageAlpha_;  //不透明度　　透明　0 ~ 255　不透明

        int lifeTime_;   //表示するする時間(単位：フレーム)
        int life_;       //表示している時間

        bool isPrint_;   //表示するかどうか
        bool isAlreadyPrint_;   //既に表示したかどうか

        Transform changeTransform_; //トランスフォームの変化
        int changeAlpha_;   //不透明度の変化

        enum class ImageNumber
        {
            start,
            one,
            two,
            three,
            goal
        };
        ImageData()
        {
            imageHandle_ = -1;
            imageAlpha_ = 255;
            lifeTime_ = 0;
            life_ = 0;
            isPrint_ = false;
            isAlreadyPrint_ = false;
            changeAlpha_ = 0;
        }
        ImageData(int handle, Transform transform, int time
            , Transform cTransform, int alpha = 255, int cAlpha = 0)
        {
            imageHandle_ = handle;
            imageTransform_ = transform;
            imageAlpha_ = alpha;
            lifeTime_ = time;
            life_ = 0;
            isPrint_ = false;
            isAlreadyPrint_ = false;
            changeTransform_ = cTransform;
            changeAlpha_ = cAlpha;
        }
    }imageData_;
    //画像をセット
    void SetImage(ImageData::ImageNumber in, int handle, Transform transform, int time
        , Transform cTransform, int alpha = 255, int cAlpha = 0);
    //画像描画
    void DrawImage(ImageData::ImageNumber in);
    //画像まとめ
    std::map<ImageData::ImageNumber, ImageData> imageMap_;

public:
    //コンストラクタ
    VehiclePlayer(GameObject* parent);

    //コンストラクタ2
    VehiclePlayer(GameObject* pParent, std::string vehicleName, std::string wheelName);

    //デストラクタ
    ~VehiclePlayer();

    //開放
    void Release() override;

    //プレイヤー限定で実行する関数  
    //UIの初期化
    void PlayerUI_Initialize() override;
    //UIの表示
    void PlayerUI_Draw() override;
    //UIの情報更新
    void PlayerUI_Update() override;
    //カメラの用意
    void PlayerCamera_Initialize() override;
    //カメラ更新
    void PlayerCamera_Update() override;

    //操作
    void InputOperate() override;

    //スピードメーター描画
    void DrawSpeedmeter();
    //時速描画
    void DrawKmH();
    //経過時間描画
    void DrawElapsedTime();
    //順位表示
    void DrawRanking();
    //ブースト表示
    void DrawBoostGauge();
    //デバッグ：加速度と回転表示
    void DrawAccelerationRotate();
    //カウントダウン描画
    void DrawStandbyCount();
};