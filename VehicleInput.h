#pragma once
#include "Engine/GameObject.h"

namespace VehicleInput
{
    enum class Button
    {
        selectUp = 0,
        selectDown,
        selectRight,
        selectLeft,
        selectDecide,
        selectReturn,

        useBooster,
        jumpUp,
        pause,

        ButtonMax
    };

    enum class Value
    {
        moveFrontRear = 0,
        moveRightLeft,
        turnRightLeft,
        handleRightLeft,

        cameraZoomInOut,
        cameraRotateUpDown,

        ValueMax
    };

    //初期化
    void Initialize();
    
    //更新
    void Update();

    /// <summary>
    /// ボタン系入力を取得
    /// </summary>
    /// <param name="n">取得したい値　Button::***</param>
    /// <returns>bool 押してない：false　押してる：true</returns>
    bool GetInput(Button b);

    /// <summary>
    /// スティック系入力を取得
    /// </summary>
    /// <param name="v">取得したい値　Value::***</param>
    /// <returns>float -1.0 ~ 0.0 ~ 1.0</returns>
    float GetInput(Value v);

    //入力の最大値を取得(今は1.0f)
    float GetMaxValue();
    //入力の最小値を取得(今は-1.0f)
    float GetMinValue();
};
