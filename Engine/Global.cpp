#include "Global.h"
#include "Debug.h"

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
float Calculator::AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	return AngleBetweenNormalVector(XMVector3Normalize(alfa), XMVector3Normalize(bravo), { 0.0f,1.0f,0.0f,0.0f });
}

//��̃x�N�g���̊p�x�����߂�
float Calculator::AngleBetweenVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	return AngleBetweenNormalVector(XMVector3Normalize(alfa), XMVector3Normalize(bravo), XMVector3Normalize(up));
}


// ���"���K�������x�N�g��"�̊p�x�����߂�
float Calculator::AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo, const XMVECTOR& up)
{
	//���K���̕K�v�Ȃ�
	if (*XMVector3Dot(XMVector3Cross(alfa, bravo - alfa), up).m128_f32 > 0.0f)
	{
		return 360.0f - XMConvertToDegrees(*XMVector3AngleBetweenNormals(alfa, bravo).m128_f32);
	}

	return XMConvertToDegrees(*XMVector3AngleBetweenNormals(alfa, bravo).m128_f32);
}

// ���"���K�������x�N�g��"�̊p�x�����߂�
float Calculator::AngleBetweenNormalVector(const XMVECTOR& alfa, const XMVECTOR& bravo)
{
	return AngleBetweenNormalVector(alfa, bravo, { 0.0f,1.0f,0.0f,0.0f });
}


//�c�[
#if 0
#if 0
	//���K��
XMVECTOR nAlfa = XMVector3Normalize(alfa)
, nBravo = XMVector3Normalize(bravo);

return XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32
	/ (*XMVector3Length(nAlfa).m128_f32 * *XMVector3Length(nBravo).m128_f32)));
#endif
#if 0
	//���K��
XMVECTOR nAlfa = XMVector3Normalize(alfa)
, nBravo = XMVector3Normalize(bravo);

float dot = *XMVector3Dot(nAlfa, nBravo).m128_f32;
XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
float angle = 0.0f;

angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
{
	//angle = XMConvertToDegrees(acos(dot));
	//angle = 360.0f - angle;
	angle = 360.0f - angle;
}
#endif
#if 0
Debug::TimerLogStart("����o�[�W����");
{
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	float dot = *XMVector3Dot(nAlfa, nBravo).m128_f32;
	XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
	float angle = 0.0f;
	if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		angle = XMConvertToDegrees(XM_2PI - acos(dot));
	}
	else
	{
		angle = XMConvertToDegrees(acos(dot));
	}
}
Debug::TimerLogEnd("����o�[�W����");

Debug::TimerLogStart("����");
{
	//���K��
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	XMVECTOR cross = XMVector3Cross(nAlfa, nBravo - nAlfa);
	float angle = 0.0f;

	angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

	if (*XMVector3Dot(cross, XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		angle = 360.0f - angle;
	}
}
Debug::TimerLogEnd("����");
#endif
#if 0
Debug::TimerLogStart("����o�[�W����");
{
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);
	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		float angle = XMConvertToDegrees(XM_2PI - acos(*XMVector3Dot(nAlfa, nBravo).m128_f32));
	}
	else
	{
		float angle = XMConvertToDegrees(acos(*XMVector3Dot(nAlfa, nBravo).m128_f32));
	}
	angle += 0.0f;
}
Debug::TimerLogEnd("����o�[�W����");
#endif
#if 0
Debug::TimerLogStart("����");
{
	//���K��
	XMVECTOR nAlfa = XMVector3Normalize(alfa), nBravo = XMVector3Normalize(bravo);

	//float angle = XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);

	if (*XMVector3Dot(XMVector3Cross(nAlfa, nBravo - nAlfa), XMVector3Normalize(up)).m128_f32 > 0.0f)
	{
		return 360.0f - XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);
		//angle = 360.0f - angle;
	}

	return XMConvertToDegrees(*XMVector3AngleBetweenNormals(nAlfa, nBravo).m128_f32);
}
Debug::TimerLogEnd("����");
#endif
#endif