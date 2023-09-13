#include <unordered_map>
#include "Music.h"

using std::string;

namespace Music
{
	struct MusicData
	{
		int hSound;			//ハンドルの値
		int musicState;		//音楽の状態
		bool isLoop;		//ループするか trueなら繰り返す回数は無意味になる
		int repeatCount;	//繰り返す回数

		enum MusicState
		{
			stop,
			play,
			pause
		};

		MusicData()
		{
			hSound = -1;
			musicState = MusicState::stop;
			isLoop = false;
			repeatCount = 0;
		}
	};

	std::unordered_map<MusicName, MusicData> musicPackage_;	//音楽のまとめmap

	/// <summary>
	/// 音楽をセットする
	/// </summary>
	/// <param name="musicName">呼び出すときのキー（String）</param>
	/// <param name="fileName">ファイル名（String）</param>
	/// <param name="loop">ループするか trueなら繰り返す回数は無意味になる(デフォルト:false)</param>
	void SetMusic(MusicName musicName, string fileName, bool loop = false)
	{
		//安全
		int handle = -1;
		handle = Audio::Load(fileName);
		assert(handle >= 0);

		musicPackage_[musicName].hSound = handle;
		musicPackage_[musicName].musicState = MusicData::stop;
		musicPackage_[musicName].isLoop = loop;
		musicPackage_[musicName].repeatCount = 1;
	}

};


//初期化
void Music::Initialize()
{
	SetMusic(MusicName::bgm_1, "music\\loop100201.wav", true);
	SetMusic(MusicName::bgm_2, "music\\loop100209.wav", true);

	SetMusic(MusicName::se_carStop, "music\\carstop.wav", false);
	SetMusic(MusicName::se_count123, "music\\se_amb04.wav", false);
	SetMusic(MusicName::se_countStart, "music\\se_ymb04.wav", false);
	SetMusic(MusicName::se_goal, "music\\se_ymb04.wav", false);
}

//更新
void Music::Update()
{
	//再生する
	for (auto& itr : musicPackage_)
	{
		int state = Audio::GetAudioState(itr.second.hSound);

		if (itr.second.musicState == MusicData::play)
		{
			if (itr.second.isLoop)
			{
				//ループ再生
				Audio::Play(itr.second.hSound);
			}
			else
			{
				//ループじゃない
				if (itr.second.repeatCount > 0)
				{
					//カウントを減らす
					if(Audio::GetAudioState(itr.second.hSound) == 0)
					{
						itr.second.repeatCount--;
					}

					//再生
					Audio::Play(itr.second.hSound);
				}
			}
		}
	}
}

//再生
void Music::Play(MusicName musicName, int count)
{
	if (musicPackage_.count(musicName) == 0)
	{
		//そんな曲名はない
		return;
	}

	musicPackage_[musicName].musicState = MusicData::play;
	musicPackage_[musicName].repeatCount = count;
}

//一時停止
void Music::Pause(MusicName musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		return;
	}

	musicPackage_[musicName].musicState = MusicData::pause;
	Audio::Pause(musicPackage_[musicName].hSound);
}

//停止
void Music::Stop(MusicName musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		return;
	}

	musicPackage_[musicName].musicState = MusicData::stop;
	Audio::Stop(musicPackage_[musicName].hSound);
}