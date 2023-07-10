#pragma once
#include "Direct3D.h"

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

namespace Global
{
	//スクリーンの幅
	void SetScreenWidth(int);
	//スクリーンの幅
	int GetScreenWidth();

	//スクリーンの高さ
	void SetScreenHeight(int);
	//スクリーンの高さ
	int GetScreenHeight();

	//FPS（画面更新速度）
	void SetFpsLimit(int);
	//FPS（画面更新速度）
	int GetFpsLimit();

	//キャプションに現在のFPSを表示するかどうか
	void SetIsDrawFps(bool);
	//キャプションに現在のFPSを表示するかどうか
	bool GetIsDrawFps();

	//現在のFPS
	void SetCurrentFPS(int);
	//現在のFPS
	int GetCurrentFPS();

};

namespace Calculator
{
	//floatどうしの比較
	bool IsEqualFloat(float alfa, float bravo);

	/// <summary>
	/// 二つのベクトルの角度を求める
	/// </summary>
	/// <param name="alfa">ベクトル１(正規化する必要とかはない)</param>
	/// <param name="bravo">ベクトル２(同上)</param>
	/// <returns>-90 ~ 90の角度</returns>
	float AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo);

	/// <summary>
	/// 二つのベクトルの角度を求める
	/// </summary>
	/// <param name="alfa">ベクトル１(正規化する必要とかはない)</param>
	/// <param name="bravo">ベクトル２(同上)</param>
	/// <param name="up">世界の上方向のベクトル(同上)</param>
	/// <returns>0 ~ 360の角度</returns>
	float AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up);
};