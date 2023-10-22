#pragma once
#include <string>

//-----------------------------------------------------------
//サウンドを管理する（XAudio使用）
//-----------------------------------------------------------
namespace Audio
{
	//初期化
	void Initialize();

	//サウンドファイル(.wav）をロード
	//すでに同じ名前のファイルをロード済みの場合は、既存のデータの番号を返す
	//引数：fileName	ファイル名
	//引数：svNum　		同時に鳴らす最大数（省略可）
	//戻値：そのデータに割り当てられた番号
	int Load(std::string fileName,int svNum = 1);

	//再生
	//引数：ID	停止したいサウンドの番号
	void Play(int ID);

	//一時停止
	//引数：ID	停止したいサウンドの番号
	void Pause(int ID);

	//停止
	//引数：ID	停止したいサウンドの番号
	void Stop(int ID);

	//すべて開放
	void Release();

	/// <summary>
	/// 状態のゲッター
	/// </summary>
	/// <param name="ID">取得したいサウンドの番号</param>
	/// <returns>０→再生していない、１以上→その数だけ再生する</returns>
	int GetAudioState(int ID);

	/// <summary>
	/// 安全か確かめる状態のゲッター
	/// </summary>
	/// <param name="ID">サウンドの番号</param>
	/// <param name="i">配列の番号</param>
	/// <returns>サウンドの状態　nullptrなら-1</returns>
	int GetVoiceStateBuffersQueued(int ID, int i);
};

