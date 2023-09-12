#pragma once

#include "Engine/GameObject.h"
#include "Engine/Audio.h"

namespace Music
{
	void Initialize();	//初期化
	void Update();		//更新　　鳴らすべき音があるなら鳴らす


	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="musicName">再生したいサウンドの曲名</param>
	/// <param name="count">再生したい回数（デフォルト：１）</param>
	void Play(std::string musicName, int count = 1);

	//一時停止
	//引数：musicName	停止したいサウンドの曲名
	void Pause(std::string musicName);

	//停止
	//引数：musicName	停止したいサウンドの曲名
	void Stop(std::string musicName);
};
