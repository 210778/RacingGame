#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMVECTOR position);
	//float
	void SetPosition(XMFLOAT3 position);
	//���_���擾
	XMVECTOR GetPosition();
	//float
	XMFLOAT3 GetPositionFloat();

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMVECTOR target);
	//float
	void SetTarget(XMFLOAT3 target);
	//�œ_���擾
	XMVECTOR GetTarget();
	//float
	XMFLOAT3 GetTargetFloat();

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix();
};