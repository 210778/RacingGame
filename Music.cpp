#include <unordered_map>
#include "Music.h"

using std::string;

namespace Music
{
	struct MusicData
	{
		int hSound;			//�n���h���̒l
		int musicState;		//���y�̏��
		bool isLoop;		//���[�v���邩 true�Ȃ�J��Ԃ��񐔂͖��Ӗ��ɂȂ�
		int repeatCount;	//�J��Ԃ���

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

	std::unordered_map<MusicName, MusicData> musicPackage_;	//���y�̂܂Ƃ�map

	/// <summary>
	/// ���y���Z�b�g����
	/// </summary>
	/// <param name="musicName">�Ăяo���Ƃ��̃L�[�iString�j</param>
	/// <param name="fileName">�t�@�C�����iString�j</param>
	/// <param name="loop">���[�v���邩 true�Ȃ�J��Ԃ��񐔂͖��Ӗ��ɂȂ�(�f�t�H���g:false)</param>
	void SetMusic(MusicName musicName, string fileName, bool loop = false)
	{
		//���S
		int handle = -1;
		handle = Audio::Load(fileName);
		assert(handle >= 0);

		musicPackage_[musicName].hSound = handle;
		musicPackage_[musicName].musicState = MusicData::stop;
		musicPackage_[musicName].isLoop = loop;
		musicPackage_[musicName].repeatCount = 1;
	}

};


//������
void Music::Initialize()
{
	SetMusic(MusicName::bgm_1, "music\\loop100201.wav", true);
	SetMusic(MusicName::bgm_2, "music\\loop100209.wav", true);

	SetMusic(MusicName::se_carStop, "music\\carstop.wav", false);
	SetMusic(MusicName::se_count123, "music\\se_amb04.wav", false);
	SetMusic(MusicName::se_countStart, "music\\se_ymb04.wav", false);
	SetMusic(MusicName::se_goal, "music\\se_ymb04.wav", false);
}

//�X�V
void Music::Update()
{
	//�Đ�����
	for (auto& itr : musicPackage_)
	{
		int state = Audio::GetAudioState(itr.second.hSound);

		if (itr.second.musicState == MusicData::play)
		{
			if (itr.second.isLoop)
			{
				//���[�v�Đ�
				Audio::Play(itr.second.hSound);
			}
			else
			{
				//���[�v����Ȃ�
				if (itr.second.repeatCount > 0)
				{
					//�J�E���g�����炷
					if(Audio::GetAudioState(itr.second.hSound) == 0)
					{
						itr.second.repeatCount--;
					}

					//�Đ�
					Audio::Play(itr.second.hSound);
				}
			}
		}
	}
}

//�Đ�
void Music::Play(MusicName musicName, int count)
{
	if (musicPackage_.count(musicName) == 0)
	{
		//����ȋȖ��͂Ȃ�
		return;
	}

	musicPackage_[musicName].musicState = MusicData::play;
	musicPackage_[musicName].repeatCount = count;
}

//�ꎞ��~
void Music::Pause(MusicName musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		return;
	}

	musicPackage_[musicName].musicState = MusicData::pause;
	Audio::Pause(musicPackage_[musicName].hSound);
}

//��~
void Music::Stop(MusicName musicName)
{
	if (musicPackage_.count(musicName) == 0)
	{
		return;
	}

	musicPackage_[musicName].musicState = MusicData::stop;
	Audio::Stop(musicPackage_[musicName].hSound);
}