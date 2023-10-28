#include "Engine/Input.h"
#include "VehicleInput.h"

using std::vector;
using std::array;
using std::map;

namespace VehicleInput
{
    map<Name, float> inputOperation_;
};

//初期化
void VehicleInput::Initialize()
{
    for (int i = 0; i < (int)(Name::SizeMax); i++)
    {
        inputOperation_[(Name)i] = 0.0f;
    }
}

//更新
void VehicleInput::Update()
{
    //選択上
    if (Input::IsKeyDown(DIK_W) || Input::IsKey(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP) ||
        Input::IsPadStickTilt(Input::StickLR::right, Input::StickDirection::up))
    {
        inputOperation_[Name::selectUp] = 1.0f;
    }

}