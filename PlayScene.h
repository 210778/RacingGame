#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//�����V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
	int hModel_;

	std::vector<Vehicle*> vehicles_;
	Vehicle* pVehiclePlayer_;

	unsigned long long universalTime_;	//�ԗ��̋��ʎ���(�P�ʁF�t���[��)

	unsigned long long standbyTime_;	//���ԑO�̑ҋ@����(�P�ʁF�t���[��)
	unsigned int	   standbySeconds_;	//���b���ˑO�ɑ҂̂�(�P�ʁF�t���[�� * 60)
	bool startFlag_;	//�X�^�[�g������
	bool pauseFlag_;	//false -> �ʏ�A true -> �|�[�Y��

	enum RankName
	{
		goalRank = 0,
		lap,
		check,
		distance,
		pointer
	};

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

	//�ԗ������������ĕK�v�Ȓl���Z�b�g
	template <class V>
	void SetVehicle(Vehicle* pVehicle, std::string vehicleName, std::string wheelName,int number);

	//�ԗ����Փ˂��Ă邩���ׂ�
	void CollideVehicle();

	//���ʂ𒲂ׂĎԗ��ɋ�����
	void CalculateRanking();

	//���Ԃ����Z���Ďԗ��ɋ�����
	void CountUniversalTime();

	//�|�[�Y
	void PlayPause();

	//NPC�Ƀv���C���[�܂ł̋�����������
	void SetNPCToPlayer();
	
	/// <summary>
	/// �v���C���[��NPC���Z�b�g
	/// </summary>
	/// <param name="population">�l��</param>
	/// <param name="playerNumber">�v���C���[�̈ʒu</param>
	void SetPlayerNPCVector(int population, int playerNumber);
};