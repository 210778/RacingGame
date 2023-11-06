#include "Input.h"
#include "Global.h"

namespace Input
{
	//ウィンドウハンドル
	HWND	hWnd_;

	//DirectInputオブジェクト
	LPDIRECTINPUT8			pDInput_;

	//キーボード
	LPDIRECTINPUTDEVICE8	pKeyDevice_;	//デバイスオブジェクト
	BYTE keyState_[256];					//現在の各キーの状態
	BYTE prevKeyState_[256];				//前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8	pMouseDevice_;	//デバイスオブジェクト
	DIMOUSESTATE mouseState_;				//マウスの状態
	DIMOUSESTATE prevMouseState_;			//前フレームのマウスの状態
	POINT mousePos_;							//マウスカーソルの位置

	//コントローラー
	const int MAX_PAD_NUM = 4;
	XINPUT_STATE controllerState_[MAX_PAD_NUM];
	XINPUT_STATE prevControllerState_[MAX_PAD_NUM];
	
	//最大値
	const int maxStick = 32767;
	const int maxTrriger = 255;


	//初期化
	void Initialize(HWND hWnd)
	{
		//ウィンドウハンドル
		hWnd_ = hWnd;

		//DirectInput本体
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&pDInput_, nullptr);

		//キーボード
		pDInput_->CreateDevice(GUID_SysKeyboard, &pKeyDevice_, nullptr);
		pKeyDevice_->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice_->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		//マウス
		pDInput_->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}

	//更新
	void Update()
	{
		//キーボード
		pKeyDevice_->Acquire();
		memcpy(prevKeyState_, keyState_, sizeof(keyState_));
		pKeyDevice_->GetDeviceState(sizeof(keyState_), &keyState_);

		//マウス
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);

		//コントローラー
		for (int i = 0; i < MAX_PAD_NUM; i++)
		{
			memcpy(&prevControllerState_[i], &controllerState_[i], sizeof(controllerState_[i]));
			XInputGetState(i, &controllerState_[i]);
		}
	}

	//開放
	void Release()
	{
		SAFE_RELEASE(pMouseDevice_);
		SAFE_RELEASE(pKeyDevice_);
		SAFE_RELEASE(pDInput_);
	}

	/////////////////////////////　キーボード情報取得　//////////////////////////////////

	//キーが押されているか調べる
	bool IsKey(int keyCode)
	{
		//押してる
		if (keyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//キーを今押したか調べる（押しっぱなしは無効）
	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (IsKey(keyCode) && !(prevKeyState_[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//キーを今放したか調べる
	bool IsKeyUp(int keyCode)
	{
		//今押してなくて、前回は押してる
		if (!IsKey(keyCode) && prevKeyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	/////////////////////////////　マウス情報取得　//////////////////////////////////

	//マウスのボタンが押されているか調べる
	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今放したか調べる
	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスカーソルの位置を取得
	XMFLOAT3 GetMousePosition()
	{
		XMFLOAT3 result = XMFLOAT3((float)mousePos_.x, (float)mousePos_.y, 0);
		return result;
	}

	//マウスカーソルの位置をセット
	void SetMousePosition(int x, int y)
	{
		mousePos_.x = x;
		mousePos_.y = y;
	}

	//そのフレームでのマウスの移動量を取得
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
		return result;
	}

	/////////////////////////////　コントローラー情報取得　//////////////////////////////////

	//コントローラーのボタンが押されているか調べる
	bool IsPadButton(int buttonCode, int padID)
	{
		if (controllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true; //押してる
		}
		return false; //押してない
	}

	//コントローラーのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsPadButtonDown(int buttonCode, int padID)
	{
		//今は押してて、前回は押してない
		if (IsPadButton(buttonCode, padID) && !(prevControllerState_[padID].Gamepad.wButtons & buttonCode))
		{
			return true;
		}
		return false;
	}

	//コントローラーのボタンを今放したか調べる
	bool IsPadButtonUp(int buttonCode, int padID)
	{
		//今押してなくて、前回は押してる
		if (!IsPadButton(buttonCode, padID) && prevControllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true;
		}
		return false;
	}

	//スティックの値
	float GetAnalogValue(int raw, int max, int deadZone)
	{
		float result = (float)raw;

		if (result > 0)
		{
			//デッドゾーン
			if (result < deadZone)
				result = 0;
			else
				result = (result - deadZone) / (max - deadZone);
		}
		else
		{
			//デッドゾーン
			if (result > -deadZone)
				result = 0;
			else
				result = (result + deadZone) / (max - deadZone);
		}
		return result;
	}


	//左スティックの傾きを取得
	XMFLOAT3 GetPadStickL(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//右スティックの傾きを取得
	XMFLOAT3 GetPadStickR(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//スティックを前は傾けてなくて、今傾けたかどうか
	bool IsPadStickTilt(StickLR lr, StickDirection dir, int padID)
	{
		float nowX = 0.0f, nowY = 0.0f, preX = 0.0f, preY = 0.0f;

		switch (lr)
		{
		default:
		case Input::StickLR::right:
			nowX = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			nowY = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preX = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbRX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preY = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbRY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			break;
		case Input::StickLR::left:
			nowX = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			nowY = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			preX = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbLX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preY = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbLY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			break;
		}

		switch (dir)
		{
		default:
		case Input::StickDirection::up:
			return (nowY > 0.0f && preY <= 0.0f) ? true : false;
			break;
		case Input::StickDirection::down:
			return (nowY < 0.0f && preY >= 0.0f) ? true : false;
			break;
		case Input::StickDirection::right:
			return (nowX > 0.0f && preX <= 0.0f && !Calculator::IsEqualFloat(nowX, preX)) ? true : false;
			break;
		case Input::StickDirection::left:
			return (nowX < 0.0f && preX >= 0.0f && !Calculator::IsEqualFloat(nowX, preX)) ? true : false;
			break;
		}

		return false;
	}
	//スティックを前は傾けてて、今傾てないかどうか
	bool IsPadStickRestore(StickLR lr, StickDirection dir, int padID)
	{
		float nowX = 0.0f, nowY = 0.0f, preX = 0.0f, preY = 0.0f;

		switch (lr)
		{
		default:
		case Input::StickLR::right:
			nowX = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			nowY = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preX = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbRX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preY = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbRY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			break;
		case Input::StickLR::left:
			nowX = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			nowY = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, maxStick, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			preX = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbLX, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			preY = GetAnalogValue(prevControllerState_[padID].Gamepad.sThumbLY, maxStick, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			break;
		}

		switch (dir)
		{
		default:
		case Input::StickDirection::up:
			return (preY > 0.0f && nowY <= 0.0f) ? true : false;
			break;
		case Input::StickDirection::down:
			return (preY < 0.0f && nowY >= 0.0f) ? true : false;
			break;
		case Input::StickDirection::right:
			return (preX > 0.0f && nowX <= 0.0f) ? true : false;
			break;
		case Input::StickDirection::left:
			return (preX < 0.0f && nowX >= 0.0f) ? true : false;
			break;
		}

		return false;
	}

	//左トリガーの押し込み具合を取得
	float GetPadTrrigerL(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bLeftTrigger, maxTrriger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//右トリガーの押し込み具合を取得
	float GetPadTrrigerR(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bRightTrigger, maxTrriger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//振動させる
	void SetPadVibration(int l, int r, int padID)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = l; // 左モーターの強さ
		vibration.wRightMotorSpeed = r;// 右モーターの強さ
		XInputSetState(padID, &vibration);
	}

}