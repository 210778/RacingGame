#include "Global.h"

namespace Global
{
	//初期化ファイルから取得されるはずの必要な情報
	int screenWidth_;		//スクリーンの幅
	int screenHeight_;		//スクリーンの高さ
	int fpsLimit_;			//FPS（画面更新速度）
	int isDrawFps_;			//キャプションに現在のFPSを表示するかどうか

	int currentFPS_;	//現在のFPS
};

//スクリーンの幅
void Global::SetScreenWidth(int value)
{
	screenWidth_ = value;
}
int Global::GetScreenWidth()
{
	return screenWidth_;
}

//スクリーンの高さ
void Global::SetScreenHeight(int value)
{
	screenHeight_ = value;
}
int Global::GetScreenHeight()
{
	return screenHeight_;
}

//FPS（画面更新速度）
void Global::SetFpsLimit(int value)
{
	fpsLimit_ = value;
}
int Global::GetFpsLimit()
{
	return fpsLimit_;
}

//キャプションに現在のFPSを表示するかどうか
void Global::SetIsDrawFps(bool value)
{
	isDrawFps_ = value;
}
bool Global::GetIsDrawFps()
{
	return isDrawFps_;
}

//現在のFPS
void Global::SetCurrentFPS(int value)
{
	currentFPS_ = value;
}
//現在のFPS
int Global::GetCurrentFPS()
{
	return currentFPS_;
}



bool Calculator::IsEqualFloat(float alfa, float bravo)
{
	//比較
	if (fabsf(alfa - bravo) <= FLT_EPSILON * fmaxf(1.f, fmaxf(fabsf(alfa), fabsf(bravo))))
	{
		return true;
	}
	return false;
}

/*
* 
//X軸の角度を取得
	//XMVECTOR eToN = data.normal - XMLoadFloat3(&data.end);
transform_.rotate_.x = XMConvertToDegrees(acos(*XMVector3Dot(worldVector_.z, normalVec).m128_f32
	/ (*XMVector3Length(worldVector_.z).m128_f32 * *XMVector3Length(normalVec).m128_f32)))
	- 90.0f;
//外積を使わないと0 ~ 180　になってしまう
XMVECTOR cross = XMVector3Cross(worldVector_.z, normalVec - worldVector_.z);
if (*XMVector3Dot(cross, worldVector_.y).m128_f32 < 0.0f)
	transform_.rotate_.x *= -1;

*/