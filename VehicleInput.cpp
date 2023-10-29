#include "Engine/Input.h"
#include "VehicleInput.h"

using std::vector;
using std::map;

namespace VehicleInput
{
    map<Button, bool> inputButton_; //�{�^���n�̒l
    map<Value, float>  inputValue_;  //�X�e�B�b�N�n�̒l

    const int number_ = 0;    //�v���C���[�̔ԍ�
    const float valueMax_ = 1.0f;   //1.0f
    const float valueNoInput_ = 0.0f;   //0.0f
    const float valueMin_ = -valueMax_; //-1.0f
};

//������
void VehicleInput::Initialize()
{
#if 0
    //���ׂĂ̍��ڂ�p�ӂ��Ă����B
    for (int i = 0; i < (int)(Name::SizeMax); i++)
    {
        inputOperation_[(Name)i] = 0.0f;
    }
#endif
}

//�X�V
void VehicleInput::Update()
{
    //���Z�b�g
    for (auto& itr : inputButton_)
    {
        itr.second = false;
    }
    for (auto& itr : inputValue_)
    {
        itr.second = valueNoInput_;
    }

    //�I��
    //��
    if (Input::IsKeyDown(DIK_W) || Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::up, number_))
    {
        inputButton_[Button::selectUp] = true;
    }
    //��
    if (Input::IsKeyDown(DIK_S) || Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::down, number_))
        inputButton_[Button::selectDown] = true;
    //�E
    if (Input::IsKeyDown(DIK_D) || Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::right, number_))
        inputButton_[Button::selectRight] = true;
    //��
    if (Input::IsKeyDown(DIK_A) || Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT, number_) ||
        Input::IsPadStickTilt(Input::StickLR::left, Input::StickDirection::left, number_))
        inputButton_[Button::selectLeft] = true;
    //����
    if (Input::IsKeyDown(DIK_SPACE) || Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RETURN)||
        Input::IsPadButtonDown(XINPUT_GAMEPAD_A, number_) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START, number_))
        inputButton_[Button::selectDecide] = true;
    //�߂�
    if(Input::IsKeyDown(DIK_ESCAPE) || Input::IsKeyDown(DIK_LCONTROL)|| 
       Input::IsPadButtonDown(XINPUT_GAMEPAD_B, number_) || Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK, number_))
       inputButton_[Button::selectReturn] = true;


    //�ԗ�
    //�O�i�A���
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
    //�n���h�����E
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickL().x), valueNoInput_)))
        inputValue_[Value::handleRightLeft] = Input::GetPadStickL().x;
    if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_RIGHT, number_))
        inputValue_[Value::handleRightLeft] = valueMax_;
    if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT) || Input::IsPadButton(XINPUT_GAMEPAD_DPAD_LEFT, number_))
        inputValue_[Value::handleRightLeft] = valueMin_;

    //�u�[�X�g
    if (Input::IsKey(DIK_SPACE) || Input::IsKey(DIK_LSHIFT) ||
        Input::IsPadButton(XINPUT_GAMEPAD_X, number_))
        inputButton_[Button::useBooster] = true;
    //�W�����v
    if (Input::IsKeyDown(DIK_Z) || Input::IsPadButtonDown(XINPUT_GAMEPAD_Y, number_))
        inputButton_[Button::jumpUp] = true;
    //�ꎞ��~
    if (Input::IsKeyDown(DIK_ESCAPE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK, number_))
        inputButton_[Button::pause] = true;

    //���E�ړ�
    if (Input::IsKey(DIK_V) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, number_))
        inputValue_[Value::moveRightLeft] = valueMin_;
    if (Input::IsKey(DIK_B) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, number_))
        inputValue_[Value::moveRightLeft] = valueMax_;

    //���E��]
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadTrrigerR()), valueNoInput_)))
        inputValue_[Value::turnRightLeft] = Input::GetPadTrrigerR();
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadTrrigerL()), valueNoInput_)))
        inputValue_[Value::turnRightLeft] = -(Input::GetPadTrrigerL());
    if (Input::IsKey(DIK_Q))
        inputValue_[Value::turnRightLeft] = valueMin_;
    if (Input::IsKey(DIK_E))
        inputValue_[Value::turnRightLeft] = valueMax_;

    //�J������]
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickR().y), valueNoInput_)))
        inputValue_[Value::cameraRotateUpDown] = Input::GetPadStickR().y;
    if (Input::IsKey(DIK_R))
        inputValue_[Value::cameraRotateUpDown] = valueMax_;
    if (Input::IsKey(DIK_F))
        inputValue_[Value::cameraRotateUpDown] = valueMin_;

    //�J�����Y�[��
    if (!(Calculator::IsEqualFloat(abs(Input::GetPadStickR().x), valueNoInput_)))
        inputValue_[Value::cameraZoomInOut] = Input::GetPadStickR().x;
    if (Input::IsKey(DIK_T))
        inputValue_[Value::cameraZoomInOut] = valueMax_;
    if (Input::IsKey(DIK_G))
        inputValue_[Value::cameraZoomInOut] = valueMin_;

}

//�{�^���擾
bool VehicleInput::GetInput(Button b)
{
#if 0
    //�����炭�Ȃ��Ă����Ȃ�
    auto found = inputButton_.find(b);
    if (found != end(inputButton_)) {
        found->second = false;  //�Ȃ��Ȃ�ǉ�����
    }
#endif

    return inputButton_[b];
}
//�X�e�B�b�N�n�擾
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

