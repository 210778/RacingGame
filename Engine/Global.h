#pragma once
#include "Direct3D.h"

//���S�Ƀ��������J�����邽�߂̃}�N��
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

namespace Global
{
	//�X�N���[���̕�
	void SetScreenWidth(int);
	//�X�N���[���̕�
	int GetScreenWidth();

	//�X�N���[���̍���
	void SetScreenHeight(int);
	//�X�N���[���̍���
	int GetScreenHeight();

	//FPS�i��ʍX�V���x�j
	void SetFpsLimit(int);
	//FPS�i��ʍX�V���x�j
	int GetFpsLimit();

	//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���
	void SetIsDrawFps(bool);
	//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���
	bool GetIsDrawFps();

	//���݂�FPS
	void SetCurrentFPS(int);
	//���݂�FPS
	int GetCurrentFPS();

};

namespace Calculator
{
	//float�ǂ����̔�r
	bool IsEqualFloat(float alfa, float bravo);

	/// <summary>
	/// ��̃x�N�g���̊p�x�����߂� (0 ~ 360)
	/// 0 ~ 180 �ł����Ȃ�XMVector3AngleBetweenVectors�Ƃ��g��
	/// ��̃x�N�g���͏���ɗp�ӂ���
	/// </summary>
	/// <param name="alfa">�x�N�g���P(���K������K�v�Ƃ��͂Ȃ�)</param>
	/// <param name="bravo">�x�N�g���Q(����)</param>
	/// <returns>0 ~ 360�̊p�x</returns>
	float AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo);
	
	/// <summary>
	/// ��̃x�N�g���̊p�x�����߂�
	/// ��̃x�N�g����ݒ�ł���
	/// </summary>
	/// <param name="alfa">�x�N�g���P(���K������K�v�Ƃ��͂Ȃ�)</param>
	/// <param name="bravo">�x�N�g���Q(����)</param>
	/// <param name="up">���E�̏�����̃x�N�g��(����)</param>
	/// <returns>0 ~ 360�̊p�x</returns>
	float AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up);

	/// <summary>
	/// ���"���K�������x�N�g��"�̊p�x�����߂�
	/// ��̃x�N�g����ݒ�ł���
	/// </summary>
	/// <param name="alfa">�x�N�g���P(���K��)</param>
	/// <param name="bravo">�x�N�g��2(���K��)</param>
	/// <param name="up">�^��̃x�N�g��(���K��)</param>
	/// <returns>0 ~ 360�̊p�x</returns>
	float AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up);

	/// <summary>
	/// ���"���K�������x�N�g��"�̊p�x�����߂�
	/// ��̃x�N�g���͏���ɗp�ӂ���
	/// </summary>
	/// <param name="alfa">�x�N�g���P(���K��)</param>
	/// <param name="bravo">�x�N�g��2(���K��)</param>
	/// <returns>0 ~ 360�̊p�x</returns>
	float AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo);
};