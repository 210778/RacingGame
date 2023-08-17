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

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="ID">鳴らしたいサウンドの番号</param>
	/// <param name="count">繰り返したい回数（デフォルト：１回）</param>
	void Play(int ID, int count = 1);

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
};

