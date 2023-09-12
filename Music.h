#pragma once

#include "Engine/GameObject.h"
#include "Engine/Audio.h"

namespace Music
{
	void Initialize();	//������
	void Update();		//�X�V�@�@�炷�ׂ���������Ȃ�炷


	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="musicName">�Đ��������T�E���h�̋Ȗ�</param>
	/// <param name="count">�Đ��������񐔁i�f�t�H���g�F�P�j</param>
	void Play(std::string musicName, int count = 1);

	//�ꎞ��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Pause(std::string musicName);

	//��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Stop(std::string musicName);
};
