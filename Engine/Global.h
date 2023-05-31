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