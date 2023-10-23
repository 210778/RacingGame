#pragma once
#include "Engine/GameObject.h"

class Text;

//�X�^�[�g��ʃV�[�����Ǘ�����N���X
class StartScene : public GameObject
{
	//����
	Text* pTextCircuit_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	StartScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};