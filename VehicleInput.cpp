#include "Engine/Input.h"
#include "VehicleInput.h"

using std::vector;
using std::map;

namespace VehicleInput
{
    map<Button, bool> inputButton_; //ボタン系の値
    map<Value, float>  inputValue_;  //スティック系の値

    const int number_ = 0;    //プレイヤーの番号
    const float valueMax_ = 1.0f;   //1.0f
    const float valueNoInput_ = 0.0f;   //0.0f
    const float valueMin_ = -valueMax_; //-1.0f
};

//初期化
void VehicleInput::Initialize()
{
#if 0
    //すべての項目を用意しておく。
    for (int i = 0; i < (int)(Name::SizeMax); i++)
    {
        inputOperation_[(Name)i] = 0.0f;
    }
#endif
}

//更新
void VehicleInput::Update()
{
    //リセット
    for (auto& itr : inputButton_)
    {
        itr.second = false;
    }
    for (auto& itr : inputValue_)
    {
        itr.second = valueNoInput_;
    }

    //選択
    //上
    if (Input::IsKeyDown(DIK_W) || Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::up, number_))
    {
        inputButton_[Button::selectUp] = true;
    }
    //下
    if (Input::IsKeyDown(DIK_S) || Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::down, number_))
        inputButton_[Button::selectDown] = true;
    //右
    if (Input::IsKeyDown(DIK_D) || Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::right, number_))
        inputButton_[Button::selectRight] = true;
    //左
    if (Input::IsKeyDown(DIK_A) || Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::left, number_))
        inputButton_[Button::selectLeft] = true;
    //決定
    if (Input::IsKeyDown(DIK_SPACE) || Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RETURN)||
        Input::IsPadButtonDown(XINPUT_GAMEPAD_A, number_) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START, number_))
        inputButton_[Button::selectDecide] = true;
    //戻る
    if(Input::IsKeyDown(DIK_ESCAPE) || Input::IsKeyDown(DIK_LCONTROL)|| 
       Input::IsPadButtonDown(XINPUT_GAMEPAD_B, number_) || Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK, number_))
       inputButton_[Button::selectReturn] = true;


    //車両
    //前進、後退
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickL().y), valueNoInput_)))
    {
        inputValue_[Value::moveFrontRear] = Input::GetPadStickL().y;
    }
    if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, number_) ||
        Input::IsPadButton(XINPUT_GAMEPAD_A, number_))
    {
        inputValue_[Value::moveFrontRear] = valueMax_;
    }
    if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, number_) ||
        Input::IsPadButton(XINPUT_GAMEPAD_B, number_))
    {
        inputValue_[Value::moveFrontRear] = valueMin_;
    }
    //ハンドル左右
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickL().x), valueNoInput_)))
        inputValue_[Value::handleRightLeft] = Input::GetPadStickL().x;
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_RIGHT, number_))
        inputValue_[Value::handleRightLeft] = valueMax_;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_LEFT, number_))
        inputValue_[Value::handleRightLeft] = valueMin_;

    //ブースト
    if (Input::IsKey(DIK_SPACE) || Input::IsKey(DIK_LSHIFT) ||
        Input::IsPadButton(XINPUT_GAMEPAD_X, number_))
        inputButton_[Button::useBooster] = true;
    //ジャンプ
    if (Input::IsKeyDown(DIK_Z) || Input::IsPadButtonDown(XINPUT_GAMEPAD_Y, number_))
        inputButton_[Button::jumpUp] = true;
    //一時停止
    if (Input::IsKeyDown(DIK_ESCAPE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK, number_))
        inputButton_[Button::pause] = true;

    //左右移動
    if (Input::IsKey(DIK_V) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, number_))
        inputValue_[Value::moveRightLeft] = valueMin_;
    if (Input::IsKey(DIK_B) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, number_))
        inputValue_[Value::moveRightLeft] = valueMax_;

    //左右回転
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadTrrigerR()), valueNoInput_)))
        inputValue_[Value::turnRightLeft] = Input::GetPadTrrigerR();
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadTrrigerL()), valueNoInput_)))
        inputValue_[Value::turnRightLeft] = -(Input::GetPadTrrigerL());
    if (Input::IsKey(DIK_Q))
        inputValue_[Value::turnRightLeft] = valueMin_;
    if (Input::IsKey(DIK_E))
        inputValue_[Value::turnRightLeft] = valueMax_;

    //カメラ回転
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickR().y), valueNoInput_)))
        inputValue_[Value::cameraRotateUpDown] = Input::GetPadStickR().y;
    if (Input::IsKey(DIK_R))
        inputValue_[Value::cameraRotateUpDown] = valueMax_;
    if (Input::IsKey(DIK_F))
        inputValue_[Value::cameraRotateUpDown] = valueMin_;

    //カメラズーム
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickR().x), valueNoInput_)))
        inputValue_[Value::cameraZoomInOut] = Input::GetPadStickR().x;
    if (Input::IsKey(DIK_T))
        inputValue_[Value::cameraZoomInOut] = valueMax_;
    if (Input::IsKey(DIK_G))
        inputValue_[Value::cameraZoomInOut] = valueMin_;

}

//ボタン取得
bool VehicleInput::GetInput(Button b)
{
#if 0
    //おそらくなくても問題ない
    auto found = inputButton_.find(b);
    if (found != end(inputButton_)) {
        found->second = false;  //ないなら追加する
    }
#endif

    return inputButton_[b];
}
//スティック系取得
float VehicleInput::GetInput(Value v)
{
    return inputValue_[v];
}

float VehicleInput::GetMaxValue()
{
    return valueMax_;
}

float VehicleInput::GetMinValue()
{
    return valueMin_;
}

