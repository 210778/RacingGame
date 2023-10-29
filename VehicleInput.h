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

    //������
    void Initialize();
    
    //�X�V
    void Update();

    /// <summary>
    /// �{�^���n���͂��擾
    /// </summary>
    /// <param name="n">�擾�������l�@Button::***</param>
    /// <returns>bool �����ĂȂ��Ffalse�@�����Ă�Ftrue</returns>
    bool GetInput(Button b);

    /// <summary>
    /// �X�e�B�b�N�n���͂��擾
    /// </summary>
    /// <param name="v">�擾�������l�@Value::***</param>
    /// <returns>float -1.0 ~ 0.0 ~ 1.0</returns>
    float GetInput(Value v);

    //���͂̍ő�l���擾(����1.0f)
    float GetMaxValue();
    //���͂̍ŏ��l���擾(����-1.0f)
    float GetMinValue();
};
