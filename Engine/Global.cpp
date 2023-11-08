#include "Global.h"

namespace Global
{
	//�������t�@�C������擾�����͂��̕K�v�ȏ��
	int screenWidth_;		//�X�N���[���̕�
	int screenHeight_;		//�X�N���[���̍���
	int fpsLimit_;			//FPS�i��ʍX�V���x�j
	int isDrawFps_;			//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���

	int currentFPS_;	//���݂�FPS
	int standardFPS_ = 60;	//FPS�̐ݒ�Ɋ֌W�Ȃ������t���[���̒l
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

int Global::GetStandardFPS()
{
	return standardFPS_;
}

std::string Global::GetModelFileName()
{
	return "model\\";
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

// �p�x�̐��K��
float Calculator::AngleNormalize(float angle)
{
	float normal = fmodf(angle, 360);//angle %= 360

	if (normal < 0)
	{
		normal += 360;
	}

	return normal;
}
//�m��
bool Calculator::IsProbability(int percent)
{
	if (rand() % 100 < percent)
	{
		return true;
	}

	return false;
}
//�m��(float)
bool Calculator::IsProbability(float percent)
{
	if (((float)rand() / RAND_MAX) <= (percent / 100.0f))
	{
		return true;
	}

	return false;
}

