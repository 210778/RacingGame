#include <unordered_map>
#include "Music.h"

using std::string;

namespace Music
{
	struct MusicData
	{
		int hSound;			//�n���h���̒l
		bool isLoop;		//���[�v���邩 true�Ȃ�J��Ԃ��񐔂͖��Ӗ��ɂȂ�
		int repeatCount;	//�J��Ԃ���
		int repeated;		//���ɌJ��Ԃ�����
	};

	std::unordered_map<string, MusicData> musicPackage_;	//���y�̂܂Ƃ�map

	/// <summary>
	/// ���y���Z�b�g����
	/// </summary>
	/// <param name="musicName">�Ăяo���Ƃ��̃L�[�iString�j</param>
	/// <param name="fileName">�t�@�C�����iString�j</param>
	/// <param name="loop">���[�v���邩 true�Ȃ�J��Ԃ��񐔂͖��Ӗ��ɂȂ�@�f�t�H���gfalse</param>
	/// <param name="repeat">�J��Ԃ��� �f�t�H���g�P</param>
	void SetMusic(string musicName, string fileName, bool loop = false, int repeat = 1)
	{
		int handle = -1;
		handle = Audio::Load(fileName);
		assert(handle >= 0);

		//���S
		musicPackage_[musicName].hSound = handle;
		musicPackage_[musicName].isLoop = loop;
		musicPackage_[musicName].repeatCount = repeat;
		musicPackage_[musicName].repeated = 0;
	}

};


//������
void Music::Initialize()
{
	SetMusic("BGM_1", "music\\loop100201.wav", true);
	SetMusic("BGM_2", "music\\loop100209.wav", true);

}

//�X�V
void Music::Update()
{
	for (const auto& itr : musicPackage_)
	{
		if (itr.second.isLoop && Audio::GetAudioState(itr.second.hSound) == 0)
		{
			//���[�v�Đ�
			Play(itr.first);
		}
	}
}

//�Đ�
bool Music::Play(string musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		//����ȋȖ��͂Ȃ�
		return false;
	}

	Audio::Play(musicPackage_[musicName].hSound, musicPackage_[musicName].repeatCount);
	return true;
}

//�ꎞ��~
void Music::Pause(string musicName)
{
	Audio::Pause(musicPackage_[musicName].hSound);
}

//��~
void Music::Stop(string musicName)
{
	Audio::Stop(musicPackage_[musicName].hSound);
}