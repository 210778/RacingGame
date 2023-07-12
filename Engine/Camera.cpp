#include "camera.h"
#include "Direct3D.h"
#include"Global.h"

namespace Camera
{
	//�ϐ�
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;	//�r���[�s��
	XMMATRIX projMatrix_;	//�v���W�F�N�V�����s��
	XMMATRIX billBoardMatrix_; //�r���{�[�h�s��
};

//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	position_ = XMVectorSet(0, 0, 0, 0);	//�J�����̈ʒu  XMVectorSet(3, 3, -10, 0);
	target_ = XMVectorSet(0, 0, 0, 0);	//�J�����̏œ_

	const float NEAR_CLIPPING = 0.1f;
	const float FAR_CLIPPING = 1600.0f;//800

	//const int WINDOW_WIDTH = 1280;				//�E�B���h�E�̕�
	//const int WINDOW_HEIGHT = 720;				//�E�B���h�E�̍���

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		(FLOAT)Global::GetScreenWidth() / (FLOAT)Global::GetScreenHeight(),
		NEAR_CLIPPING,
		FAR_CLIPPING);
	/*
	���̊֐��̈����͑O���珇�Ԃ�
		1.��p�i����p�j�FXM_PI��3.14���W�A����180�x�BDIV4�́�4�Ȃ̂�XM_PIDIV4��45�x��\���萔�B
		2.�A�X�y�N�g��F�E�B���h�E��(��, �c)�̔䗦�B�������𒼐ڏ���������ă_�T���̂ŁA��ŉ��Ƃ������B
		3.�j�A�i�߁j�N���b�s���O�ʂ܂ł̋����F�J�������炱�̒l���߂����͉̂f��Ȃ��B
		4.�t�@�[�i���j�N���b�s���O�ʂ܂ł̋����F�J�������炱�̒l��艓�����͉̂f��Ȃ��B
	*/
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update()
{
	//assert(!XMVector3Equal(EyeDirection, XMVectorZero()));
	//assert(!XMVector3IsInfinite(EyeDirection));
	//assert(!XMVector3Equal(UpDirection, XMVectorZero()));
	//assert(!XMVector3IsInfinite(UpDirection));

	//���S
	if (XMVector3Equal(position_, target_))
		return;

	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	billBoardMatrix_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_- position_, XMVectorSet(0, 1, 0, 0));
	billBoardMatrix_ = XMMatrixInverse(nullptr, billBoardMatrix_);
}

//�ʒu��ݒ�
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}
void Camera::SetPosition(XMFLOAT3 position)
{
	position_ = XMLoadFloat3(&position);
}

//�ʒu���擾
XMVECTOR Camera::GetPosition()
{
	return position_;
}
XMFLOAT3 Camera::GetPositionFloat()
{
	XMFLOAT3 answer;
	XMStoreFloat3(&answer, position_);
	return answer;
}

//�œ_��ݒ�
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}
void Camera::SetTarget(XMFLOAT3 target)
{
	target_ = XMLoadFloat3(&target);
}

//�œ_���擾
XMVECTOR Camera::GetTarget()
{
	return target_;
}
XMFLOAT3 Camera::GetTargetFloat()
{
	XMFLOAT3 answer;
	XMStoreFloat3(&answer, target_);
	return answer;
}

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix() { return viewMatrix_; }

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix() { return projMatrix_; }

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix() { return billBoardMatrix_; }
