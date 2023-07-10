#include "Global.h"

namespace Global
{
	//�������t�@�C������擾�����͂��̕K�v�ȏ��
	int screenWidth_;		//�X�N���[���̕�
	int screenHeight_;		//�X�N���[���̍���
	int fpsLimit_;			//FPS�i��ʍX�V���x�j
	int isDrawFps_;			//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���

	int currentFPS_;	//���݂�FPS
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

//���݂�FPS
void Global::SetCurrentFPS(int value)
{
	currentFPS_ = value;
}
//���݂�FPS
int Global::GetCurrentFPS()
{
	return currentFPS_;
}



bool Calculator::IsEqualFloat(float alfa, float bravo)
{
	//��r
	if (fabsf(alfa - bravo) <= FLT_EPSILON * fmaxf(1.f, fmaxf(fabsf(alfa), fabsf(bravo))))
	{
		return true;
	}
	return false;
}

//��̃x�N�g���̊p�x�����߂�
float Calculator::AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	//���K��
	XMVECTOR nAlfa = XMVector3Normalize(alfa)
			, nBravo = XMVector3Normalize(bravo);
	
	return XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32
		/ (*XMVector3Length(nAlfa).m128_f32 * *XMVector3Length(nBravo).m128_f32)));
}

//��̃x�N�g���̊p�x�����߂�
float Calculator::AngleBetweelVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	//���K��
	XMVECTOR nAlfa = XMVector3Normalize(alfa)
			, nBravo = XMVector3Normalize(bravo);

	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 < 0.0f)
	{
		return XMConvertToDegrees(*XMVectorACos(XMVector3Dot(nAlfa, nBravo)).m128_f32);
	}

	return XMConvertToDegrees(-(XM_2PI - *XMVectorACos(XMVector3Dot(nAlfa, nBravo)).m128_f32));
}