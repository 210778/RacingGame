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
	/// <param name="musicName">鳴らしたいサウンドの曲名</param>
	/// <returns>true→曲があったので再生、false→曲がなかった</returns>
	bool Play(std::string musicName);

	//一時停止
	//引数：musicName	停止したいサウンドの曲名
	void Pause(std::string musicName);

	//停止
	//引数：musicName	停止したいサウンドの曲名
	void Stop(std::string musicName);
};
