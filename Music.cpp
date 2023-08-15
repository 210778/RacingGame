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
	};

	std::unordered_map<string, MusicData> musicPackage_;

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
	}

};


//初期化
void Music::Initialize()
{
	SetMusic("BGM_1", "music\\loop100201.wav", true);
}