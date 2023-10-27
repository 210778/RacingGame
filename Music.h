#pragma once

#include "Engine/GameObject.h"
#include "Engine/Audio.h"

namespace Music
{
	void Initialize();	//������
	void Update();		//�X�V�@�@�炷�ׂ���������Ȃ�炷

	//���y�̖��O�N���X
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
	/// �Đ�
	/// </summary>
	/// <param name="musicName">�Đ��������T�E���h�̋Ȗ�</param>
	/// <param name="count">�Đ��������񐔁i�f�t�H���g�F�P�j</param>
	void Play(MusicName musicName, int count = 1);

	//�ꎞ��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Pause(MusicName musicName);

	//��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Stop(MusicName musicName);
};
