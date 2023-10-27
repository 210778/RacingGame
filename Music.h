#pragma once

#include "Engine/GameObject.h"
#include "Engine/Audio.h"

namespace Music
{
	void Initialize();	//初期化
	void Update();		//更新　　鳴らすべき音があるなら鳴らす

	//音楽の名前クラス
	enum class MusicName
	{
		bgm_1,
		bgm_2,

		se_carStop,
		se_count123,
		se_countStart,
		se_lap,
		se_goal,
		se_select,
		se_select2,
		se_se_selectError,
	};

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="musicName">再生したいサウンドの曲名</param>
	/// <param name="count">再生したい回数（デフォルト：１）</param>
	void Play(MusicName musicName, int count = 1);

	//一時停止
	//引数：musicName	停止したいサウンドの曲名
	void Pause(MusicName musicName);

	//停止
	//引数：musicName	停止したいサウンドの曲名
	void Stop(MusicName musicName);
};
