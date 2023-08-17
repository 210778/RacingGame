#pragma once
#include <string>

//-----------------------------------------------------------
//�T�E���h���Ǘ�����iXAudio�g�p�j
//-----------------------------------------------------------
namespace Audio
{
	//������
	void Initialize();

	//�T�E���h�t�@�C��(.wav�j�����[�h
	//���łɓ������O�̃t�@�C�������[�h�ς݂̏ꍇ�́A�����̃f�[�^�̔ԍ���Ԃ�
	//�����FfileName	�t�@�C����
	//�����FsvNum�@		�����ɖ炷�ő吔�i�ȗ��j
	//�ߒl�F���̃f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�
	int Load(std::string fileName,int svNum = 1);

	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="ID">�炵�����T�E���h�̔ԍ�</param>
	/// <param name="count">�J��Ԃ������񐔁i�f�t�H���g�F�P��j</param>
	void Play(int ID, int count = 1);

	//�ꎞ��~
	//�����FID	��~�������T�E���h�̔ԍ�
	void Pause(int ID);

	//��~
	//�����FID	��~�������T�E���h�̔ԍ�
	void Stop(int ID);

	//���ׂĊJ��
	void Release();

	/// <summary>
	/// ��Ԃ̃Q�b�^�[
	/// </summary>
	/// <param name="ID">�擾�������T�E���h�̔ԍ�</param>
	/// <returns>�O���Đ����Ă��Ȃ��A�P�ȏと���̐������Đ�����</returns>
	int GetAudioState(int ID);
};

