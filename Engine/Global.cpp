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

/*
* 
//X���̊p�x���擾
	//XMVECTOR eToN = data.normal - XMLoadFloat3(&data.end);
transform_.rotate_.x = XMConvertToDegrees(acos(*XMVector3Dot(worldVector_.z, normalVec).m128_f32
	/ (*XMVector3Length(worldVector_.z).m128_f32 * *XMVector3Length(normalVec).m128_f32)))
	- 90.0f;
//�O�ς��g��Ȃ���0 ~ 180�@�ɂȂ��Ă��܂�
XMVECTOR cross = XMVector3Cross(worldVector_.z, normalVec - worldVector_.z);
if (*XMVector3Dot(cross, worldVector_.y).m128_f32 < 0.0f)
	transform_.rotate_.x *= -1;

*/