#include "Global.h"

namespace Global
{
	//�������t�@�C������擾�����͂��̕K�v�ȏ��
	int screenWidth_;		//�X�N���[���̕�
	int screenHeight_;		//�X�N���[���̍���
	int fpsLimit_;			//FPS�i��ʍX�V���x�j
	int isDrawFps_;			//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���
};

//�X�N���[���̕�
void Global::SetScreenWidth(int value)
{
	screenWidth_ = value;
}
int Global::GetScreenWidth()
{
	return screenWidth_;
}

//�X�N���[���̍���
void Global::SetScreenHeight(int value)
{
	screenHeight_ = value;
}
int Global::GetScreenHeight()
{
	return screenHeight_;
}

//FPS�i��ʍX�V���x�j
void Global::SetFpsLimit(int value)
{
	fpsLimit_ = value;
}
int Global::GetFpsLimit()
{
	return fpsLimit_;
}

//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���
void Global::SetIsDrawFps(bool value)
{
	isDrawFps_ = value;
}
bool Global::GetIsDrawFps()
{
	return isDrawFps_;
}