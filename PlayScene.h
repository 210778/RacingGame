#pragma once
#include "Engine/GameObject.h"

class Ground;
class Vehicle;


//�����V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
	int hImage_;
	int hModel_;

	Ground* pGround_;

	std::vector<Vehicle*> vehicleVector_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�ԗ��N���X�̂��߂̏�����
	template <class V>
	V* VehicleInstantiate(GameObject* pParent, std::string vehicleName, std::string wheelName);

};