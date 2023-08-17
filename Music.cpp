#include <unordered_map>
#include "Music.h"

using std::string;

namespace Music
{
	struct MusicData
	{
		int hSound;			//ハンドルの値
		bool isLoop;		//ループするか trueなら繰り返す回数は無意味になる
		int repeatCount;	//繰り返す回数
		int repeated;		//既に繰り返した回数
	};

	std::unordered_map<string, MusicData> musicPackage_;	//音楽のまとめmap

	/// <summary>
	/// 音楽をセットする
	/// </summary>
	/// <param name="musicName">呼び出すときのキー（String）</param>
	/// <param name="fileName">ファイル名（String）</param>
	/// <param name="loop">ループするか trueなら繰り返す回数は無意味になる　デフォルトfalse</param>
	/// <param name="repeat">繰り返す回数 デフォルト１</param>
	void SetMusic(string musicName, string fileName, bool loop = false, int repeat = 1)
	{
		int handle = -1;
		handle = Audio::Load(fileName);
		assert(handle >= 0);

		//安全
		musicPackage_[musicName].hSound = handle;
		musicPackage_[musicName].isLoop = loop;
		musicPackage_[musicName].repeatCount = repeat;
		musicPackage_[musicName].repeated = 0;
	}

};


//初期化
void Music::Initialize()
{
	SetMusic("BGM_1", "music\\loop100201.wav", true);
	SetMusic("BGM_2", "music\\loop100209.wav", true);

}

//更新
void Music::Update()
{
	for (const auto& itr : musicPackage_)
	{
		if (itr.second.isLoop && Audio::GetAudioState(itr.second.hSound) == 0)
		{
			//ループ再生
			Play(itr.first);
		}
	}
}

//再生
bool Music::Play(string musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		//そんな曲名はない
		return false;
	}

	Audio::Play(musicPackage_[musicName].hSound, musicPackage_[musicName].repeatCount);
	return true;
}

//一時停止
void Music::Pause(string musicName)
{
	Audio::Pause(musicPackage_[musicName].hSound);
}

//停止
void Music::Stop(string musicName)
{
	Audio::Stop(musicPackage_[musicName].hSound);
}