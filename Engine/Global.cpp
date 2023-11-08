#include "Global.h"

namespace Global
{
	//初期化ファイルから取得されるはずの必要な情報
	int screenWidth_;		//スクリーンの幅
	int screenHeight_;		//スクリーンの高さ
	int fpsLimit_;			//FPS（画面更新速度）
	int isDrawFps_;			//キャプションに現在のFPSを表示するかどうか

	int currentFPS_;	//現在のFPS
	int standardFPS_ = 60;	//FPSの設定に関係なくつかうフレームの値
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

int Global::GetStandardFPS()
{
	return standardFPS_;
}

std::string Global::GetModelFileName()
{
	return "model\\";
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

// 角度の正規化
float Calculator::AngleNormalize(float angle)
{
	float normal = fmodf(angle, 360);//angle %= 360

	if (normal < 0)
	{
		normal += 360;
	}

	return normal;
}
//確率
bool Calculator::IsProbability(int percent)
{
	if (rand() % 100 < percent)
	{
		return true;
	}

	return false;
}
//確率(float)
bool Calculator::IsProbability(float percent)
{
	if (((float)rand() / RAND_MAX) <= (percent / 100.0f))
	{
		return true;
	}

	return false;
}

