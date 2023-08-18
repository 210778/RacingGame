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
	/// <param name="musicName">�炵�����T�E���h�̋Ȗ�</param>
	/// <returns>true���Ȃ��������̂ōĐ��Afalse���Ȃ��Ȃ�����</returns>
	bool Play(std::string musicName);

	//�ꎞ��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Pause(std::string musicName);

	//��~
	//�����FmusicName	��~�������T�E���h�̋Ȗ�
	void Stop(std::string musicName);
};
