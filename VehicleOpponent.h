#pragma once
#include "Vehicle.h"

//◆◆◆を管理するクラス
class VehicleOpponent : public Vehicle
{
    float max_;
    float min_;

    float frontHitLength_;  //前の衝突判定の長さ
    float sideHitLength_;   //横の衝突判定の長さ

    float checkBackAngle_;  //後ろに目的地があると判断する角度(1/2)
    bool backFlag_;        //前が壁ならバックする
    unsigned int backTimeCount_;   //バックしている時間 (単位：フレーム)
    unsigned int backTimeMax_;     //バックする必要がある時間 (単位：フレーム)

    bool boostFlag_;
    unsigned int boostTimeCount_;   //ブーストしている時間 (単位：フレーム)
    unsigned int boostTimeMax_;     //ブーストする必要がある時間 (単位：フレーム)

    float randomTurnOdds_;    //ランダムにハンドルを切る確率
    float boostOdds_;         //ブーストする確率
    float boostTurnOdds_;     //曲がっているときにブーストする確率
    float booststraightOdds_; //曲がっていないときに　〃
    float straightLimit_;   //ハンドルがこの値以下なら直線とみなす
    float turnLimit_;       //この値以上なら曲がってる

public:
    //コンストラクタ
    VehicleOpponent(GameObject* parent);

    //コンストラクタ2
    VehicleOpponent(GameObject* pParent, std::string vehicleName, std::string wheelName);

    //デストラクタ
    ~VehicleOpponent();

    //操作
    void InputOperate() override;
};