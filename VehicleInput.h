#pragma once
#include "Engine/GameObject.h"

namespace VehicleInput
{
    enum class Name
    {
        selectUp = 0,
        selectDown,
        selectRight,
        selectLeft,
        selectDecide,

        moveFront,
        moveRear,
        moveRight,
        moveLeft,
        turnRight,
        turnLeft,
        handleRight,
        handleLeft,
        useBooster,

        cameraZoomIn,
        cameraZoomOut,
        cameraRotateUp,
        cameraRotateDown,
        SizeMax
    };

    //������
    void Initialize();
    
    //�X�V
    void Update();
};
