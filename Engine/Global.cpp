#include "Global.h"
#include "Debug.h"

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
float Calculator::AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	return AngleBetweenNormalVector(XMVector3Normalize(alfa), XMVector3Normalize(bravo), { 0.0f,1.0f,0.0f,0.0f });
}

//二つのベクトルの角度を求める
float Calculator::AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	return AngleBetweenNormalVector(XMVector3Normalize(alfa), XMVector3Normalize(bravo), XMVector3Normalize(up));
}


// 二つの"正規化したベクトル"の角度を求める
float Calculator::AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	//正規化の必要なし
	if (*XMVector3Dot(XMVector3Cross(alfa, bravo - alfa), up).m128_f32 > 0.0f)
	{
		return 360.0f - XMConvertToDegrees(*XMVector3AngleBetweenNormals(alfa, bravo).m128_f32);
	}

	return XMConvertToDegrees(*XMVector3AngleBetweenNormals(alfa, bravo).m128_f32);
}

// 二つの"正規化したベクトル"の角度を求める
float Calculator::AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	return AngleBetweenNormalVector(alfa, bravo, { 0.0f,1.0f,0.0f,0.0f });
}


//残骸
#if 0
#if 0
	//正規化
XMVECTOR nAlfa = XMVector3Normalize(alfa)
, nBravo = XMVector3Normalize(bravo);

return XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32
	/ (*XMVector3Length(nAlfa).m128_f32 * *XMVector3Length(nBravo).m128_f32)));
#endif
#if 0
	//正規化
XMVECTOR nAlfa = XMVector3Normalize(alfa)
, nBravo = XMVector3Normalize(bravo);

float dot = *XMVector3Dot(nAlfa, nBravo).m128_f32;
XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
float angle = 0.0f;

angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
{
	//angle = XMConvertToDegrees(acos(dot));
	//angle = 360.0f - angle;
	angle = 360.0f - angle;
}
#endif
#if 0
Debug::TimerLogStart("自作バージョン");
{
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	float dot = *XMVector3Dot(nAlfa, nBravo).m128_f32;
	XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
	float angle = 0.0f;
	if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		angle = XMConvertToDegrees(XM_2PI - acos(dot));
	}
	else
	{
		angle = XMConvertToDegrees(acos(dot));
	}
}
Debug::TimerLogEnd("自作バージョン");

Debug::TimerLogStart("既存");
{
	//正規化
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
	float angle = 0.0f;

	angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

	if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		angle = 360.0f - angle;
	}
}
Debug::TimerLogEnd("既存");
#endif
#if 0
Debug::TimerLogStart("自作バージョン");
{
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		float angle = XMConvertToDegrees(XM_2PI - acos(*XMVector3Dot(nAlfa, nBravo).m128_f32));
	}
	else
	{
		float angle = XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32));
	}
	angle += 0.0f;
}
Debug::TimerLogEnd("自作バージョン");
#endif
#if 0
Debug::TimerLogStart("既存");
{
	//正規化
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);

	//float angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		return 360.0f - XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);
		//angle = 360.0f - angle;
	}

	return XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);
}
Debug::TimerLogEnd("既存");
#endif
#endif