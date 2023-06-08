#pragma once
#include "Vehicle.h"

//◆◆◆を管理するクラス
class VehicleOpponent : public Vehicle
{
public:
    //コンストラクタ
    VehicleOpponent(GameObject* parent);

    //コンストラクタ2
    VehicleOpponent(GameObject* pParent, std::string vehicleName, std::string wheelName);

    //デストラクタ
    ~VehicleOpponent();
};