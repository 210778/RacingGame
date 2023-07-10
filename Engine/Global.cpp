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

//二つのベクトルの角度を求める
float Calculator::AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	//正規化
	XMVECTOR nAlfa = XMVector3Normalize(alfa)
			, nBravo = XMVector3Normalize(bravo);
	
	return XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32
		/ (*XMVector3Length(nAlfa).m128_f32 * *XMVector3Length(nBravo).m128_f32)));
}

//二つのベクトルの角度を求める
float Calculator::AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	//正規化
	XMVECTOR nAlfa = XMVector3Normalize(alfa)
			, nBravo = XMVector3Normalize(bravo);

	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 < 0.0f)
	{
		return XMConvertToDegrees(*XMVectorACos(XMVector3Dot(nAlfa, nBravo)).m128_f32);
	}

	return XMConvertToDegrees(-(XM_2PI - *XMVectorACos(XMVector3Dot(nAlfa, nBravo)).m128_f32));
}