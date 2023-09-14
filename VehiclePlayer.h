#pragma once
#include "Vehicle.h"

class Speedometer;
class Text;
class Sample;
class Viewer;
class ImagePrinter;

//◆◆◆を管理するクラス
class VehiclePlayer : public Vehicle
{
    //スピードメーター
    Speedometer* pSpeedometer_;

    //文字シリーズ
    Text* pTextSpeed_;  //時速
    Text* pTextTime_;   //経過時間
    Text* pTextLap_;    //周回数
    Text* pTextRanking_;    //順位
    Text* pTextAcceleration_; //加速度表示 デバッグ用
    float km_hAdd;  //時速表示加算
    int   flashIntervalUI_;   //表示が点滅する間隔
    bool  IsFlashUI_;         //UIを表示するかどうか
    Sample* pSample_;

    //ブースト系
    int imageBoostMax_;
    int imageBoost_;

    //画像表示クラス
    ImagePrinter* pImagePrinter_;
    std::unordered_map<std::string, bool> isPrintedMap_;

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
    //エフェクト表示
    void PlayerParticle() override;
    //カメラ更新
    void PlayerCamera_Update() override;

    //操作
    void InputOperate() override;

#if 0
    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //何かに当たった
    //引数：pTarget 当たった相手
    void OnCollision(GameObject* pTarget) override;
#endif
};