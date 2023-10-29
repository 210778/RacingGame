#include "Engine/Input.h"
#include "VehicleInput.h"

using std::vector;
using std::array;
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
    //入力があるならスティックを優先
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickL().y), valueNoInput_)))
    {
        inputValue_[Value::moveFrontRear] = Input::GetPadStickL().y;
    }
    else if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, number_))
    {
        inputValue_[Value::moveFrontRear] = valueMax_;
    }
    else if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, number_))
    {
        inputValue_[Value::moveFrontRear] = valueMin_;
    }

    //ハンドル左右
    //入力があるならスティックを優先
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickL().x), valueNoInput_)))
        inputValue_[Value::handleRightLeft] = Input::GetPadStickL().x;
    else if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_RIGHT, number_))
        inputValue_[Value::handleRightLeft] = valueMax_;
    else if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_LEFT, number_))
        inputValue_[Value::handleRightLeft] = valueMin_;

    //一時停止
    if (Input::IsKeyDown(DIK_ESCAPE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK, number_))
        inputButton_[Button::pause] = true;

    
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

