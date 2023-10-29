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

        jumpUp,
        useBooster,
        pause,

        cameraZoomIn,
        cameraZoomOut,
        cameraRotateUp,
        cameraRotateDown,
        ButtonMax
    };

    enum class Value
    {
        moveFrontRear = 0,
        moveRightLeft,
        turnRightLeft,
        handleRightLeft,
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
};
