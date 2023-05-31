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