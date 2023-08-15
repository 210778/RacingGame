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
	};

	std::unordered_map<string, MusicData> musicPackage_;

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
	}

};


//������
void Music::Initialize()
{
	SetMusic("BGM_1", "music\\loop100201.wav", true);
}