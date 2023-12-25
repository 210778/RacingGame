#include <tuple>
#include <algorithm>
#include <directxcollision.h>

#include "Engine/SceneManager.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"

#include "PlayScene.h"
#include "Vehicle.h"
#include "VehiclePlayer.h"
#include "VehicleOpponent.h"
#include "ParticlePackage.h"
#include "Music.h"
#include "Circuit.h"
#include "VehicleGlobal.h"
#include "VehicleInput.h"

using std::tuple;
using std::sort;
using std::vector;
using std::string;

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
	, pVehiclePlayer_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(6)
	, startFlag_(false), pauseFlag_(false), playerGoalFlag_(false)
	, playMusic_(Music::MusicName::se_se_selectError)
{
}

//������
void PlayScene::Initialize()
{
	Circuit::ResetCircuitModelHandle();	//�R�[�X������
	Circuit::CreateChosenCircuit(this); //�R�[�X����

	int population = VehicleGlobal::GetChosenPopulation();	//�l��
	int playerNumber = population - 1;	//�v���C���[�͈�Ԍ��

	//�v���C���[��NPC���Z�b�g
	SetPlayerNPCVector(population, playerNumber);

	//�Q���l�����Z�b�g
	for (auto& i : vehicles_)
	{
		(*i).SetPopulation(vehicles_.size());
	}

	//�����Ȃ���Ԃɂ���
	for (auto& i : vehicles_)
	{
		(*i).SetOperationInvalid(true);
	}

	//�ҋ@����
	standbyTime_ = standbySeconds_ * Global::GetStandardFPS();

	//�G�t�F�N�g������
	ParticlePackage::Initialize();
}

//�X�V
void PlayScene::Update()
{
	//���y
	//�X�^�[�g��̋�
	if (startFlag_ && !playerGoalFlag_)
	{
		switch (Circuit::GetChosenCircuitIndex())
		{
		default:
		case 0:
			playMusic_ = Music::MusicName::bgm_1;
			break;
		case 1:
			playMusic_ = Music::MusicName::bgm_2;
			break;
		case 2:
			playMusic_ = Music::MusicName::bgm_3;
			break;
		}

		Music::Play(playMusic_);
	}
	//�S�[��������
	if (!playerGoalFlag_ && pVehiclePlayer_->GetGoalFlag())
	{
		Music::Stop(playMusic_);
		playerGoalFlag_ = true;
	}
	//�S�[����
	if (playerGoalFlag_)
	{
		Music::Play(Music::MusicName::bgm_goal);
	}

	Music::Update();

	//����
	VehicleInput::Update();

	//�|�[�Y
	PlayPause();

	//�^�C�g�����
	if (pauseFlag_ && VehicleInput::GetInput(VehicleInput::Button::selectDecide))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_START);
	}

	//�|�[�Y���Ă�Ȃ��~
	if (pauseFlag_)
	{
		return;
	}

	//����
	CountUniversalTime();

	//�v���C���[�܂ł̋���
	SetNPCToPlayer();

	//����
	CalculateRanking();

	//�Փ�
	CollideVehicle();
}

//�`��
void PlayScene::Draw()
{
	Circuit::Draw();
}

//�J��
void PlayScene::Release()
{
}

//�ԗ��N���X�̂��߂̏�����
template <class V>
V* PlayScene::VehicleInstantiate(GameObject* pParent, string vehicleName, string wheelName)
{
	V* pNewObject = new V(pParent, vehicleName, wheelName);

	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();

	return pNewObject;
}

//�ԗ������������ĕK�v�Ȓl���Z�b�g
template <class V>
void PlayScene::SetVehicle(Vehicle* pVehicle, string vehicleName, string wheelName, int number)
{
	auto* pCircuit = Circuit::GetChosenCircuit();

	pVehicle = VehicleInstantiate<V>(this, vehicleName, wheelName);
	Transform setTra = pCircuit->startTransform_[number];

	pVehicle->SetPosition(pCircuit->startTransform_[number].position_);
	pVehicle->SetRotate(pCircuit->startTransform_[number].rotate_);
	pVehicle->SetStartTransform(pCircuit->startTransform_[number]);
	pVehicle->SetPointCountMax(pCircuit->check_.size());
	pVehicle->SetLapMax(pCircuit->maxLap_);

	vehicles_[number] = pVehicle;
}

//�ԗ����Փ˂��Ă邩���ׂ�
void PlayScene::CollideVehicle()
{
	Debug::TimerLogStart("�Փ�");
	//�d���Ȃ�
	for (int one = 0; one < vehicles_.size(); one++)
	{
		for (int two = one + 1; two < vehicles_.size(); two++)
		{
			BoundingOrientedBox BOB1;
			BOB1.Center = vehicles_[one]->GetBoundingBoxCenter();
			BOB1.Extents = vehicles_[one]->GetBoundingBoxExtents();
			XMFLOAT3 rotate1 = vehicles_[one]->GetRotate();
			XMStoreFloat4(&BOB1.Orientation, XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(rotate1.x), XMConvertToRadians(rotate1.y), XMConvertToRadians(rotate1.z)));

			BoundingOrientedBox BOB2;
			BOB2.Center = vehicles_[two]->GetBoundingBoxCenter();
			BOB2.Extents = vehicles_[two]->GetBoundingBoxExtents();
			XMFLOAT3 rotate2 = vehicles_[two]->GetRotate();
			XMStoreFloat4(&BOB2.Orientation, XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(rotate2.x), XMConvertToRadians(rotate2.y), XMConvertToRadians(rotate2.z)));

			//��������A���邢�͊܂܂��
			if (BOB1.Contains(BOB2) != 0)
			{
				//�������N��
				vehicles_[one]->CollideBoundingBox(vehicles_[two]);
				vehicles_[two]->CollideBoundingBox(vehicles_[one]);
			}

		}
	}
	Debug::TimerLogEnd("�Փ�");

}

//���ʂ𒲂ׂĎԗ��ɋ�����
void PlayScene::CalculateRanking()
{
	//�ԗ��̏��ʂ��v�Z
	//�^��p��
	vector<tuple<int, int, int, float, Vehicle*>> ranking(vehicles_.size());
	for (int i = 0; i < ranking.size(); i++)
	{
		//�S�[�����Ă邩�H
		int goalRanking = vehicles_.size();
		vehicles_[i]->GetIsGoalRanking(&goalRanking);

		std::get<RankName::goalRank>(ranking[i]) = goalRanking;
		std::get<RankName::lap>(ranking[i]) = -vehicles_[i]->GetLapCount();
		std::get<RankName::check>(ranking[i]) = -vehicles_[i]->GetPointCount();
		std::get<RankName::distance>(ranking[i]) = vehicles_[i]->GetNextCheckDistance();
		std::get<RankName::pointer>(ranking[i]) = vehicles_[i];
	}

	//�\�[�g
	sort(ranking.begin(), ranking.end());

	//���ʂ�������
	for (int i = 0; i < ranking.size(); i++)
	{
		std::get<RankName::pointer>(ranking[i])->SetRanking(i + 1);
	}
}

//���Ԃ����Z���Ďԗ��ɋ�����
void PlayScene::CountUniversalTime()
{
	if(standbyTime_ > 0)
	{
		standbyTime_--;
		for (auto& itr : vehicles_)
		{
			itr->SetStandbyTime(standbyTime_);
		}
	}
	else
	{
		//���Z �I�[�o�[�t���[�͍l���Ȃ�
		universalTime_++;
		for (auto& itr : vehicles_)
		{
			itr->SetTime(universalTime_);
		}
	}

	//�ԗ��𑀍�\�ɂ���
	if (standbyTime_ <= 0 && startFlag_ == false)
	{
		startFlag_ = true;
		for (auto& i : vehicles_)
		{
			(*i).SetOperationInvalid(false);
		}
	}
}

//�|�[�Y
void PlayScene::PlayPause()
{
	if (VehicleInput::GetInput(VehicleInput::Button::pause))
	{
		Music::Play(Music::MusicName::se_select2);	//����

		if (pauseFlag_)
			pauseFlag_ = false;
		else
			pauseFlag_ = true;
	}

	for (auto& i : vehicles_)
	{
		(*i).SetPauseFlag(pauseFlag_);
	}
}

//NPC�Ƀv���C���[�܂ł̋�����������
void PlayScene::SetNPCToPlayer()
{
	if (pVehiclePlayer_ == nullptr)
		return;

	for (auto& itr : vehicles_)
	{
		if (itr == pVehiclePlayer_)
			continue;	//�v���C���[�͖���

		XMFLOAT3 pPos = pVehiclePlayer_->GetPosition();
		XMFLOAT3 nPos = itr->GetPosition();
		float leng = *XMVector3Length(XMLoadFloat3(&pPos) - XMLoadFloat3(&nPos)).m128_f32;

		itr->SetToPlayerVehicleLength(*XMVector3Length(XMLoadFloat3(&pPos) - XMLoadFloat3(&nPos)).m128_f32);
	}
}

void PlayScene::SetPlayerNPCVector(int population, int playerNumber)
{
	//�l��
	int people = min(max(population, 1), Circuit::GetChosenCircuit()->startTransform_.size());
	//����
	int pNum = min(max(playerNumber, 0), population - 1);

	//�z��
	vehicles_.clear();
	vehicles_.resize(population);

	//�v���C���[�̃��f��
	string playerVehicleModelName = VehicleGlobal::GetChosenVehicleName();

	//���f�����������Ă�z������
	//�ԗ�
	vector<string> vehicleModelVector;
	for (auto& itr : *VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle))
	{
		vehicleModelVector.push_back(itr.second);
	}
	//�^�C��
	vector<string> wheelModelVector;
	for (auto& itr : *VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel))
	{
		wheelModelVector.push_back(itr.second);
	}

	//�z��̃T�C�Y���P�ȏ�Ȃ�v���C���[�Ɣ���Ă郂�f�����͏����@�^�C���͔���Ă����Ȃ�
	if (vehicleModelVector.size() >= 1)
	{
		// �C�e���[�g���ɗv�f�폜������悤�ȏꍇ�ɂ͔͈�for���͎g�p�ł��Ȃ�
		for (auto itr = vehicleModelVector.begin(); itr != vehicleModelVector.end();)
		{
			// ������v�����v�f���폜����
			if (*itr == playerVehicleModelName)
				itr = vehicleModelVector.erase(itr);// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
			else
				++itr;// �v�f�폜�����Ȃ��ꍇ�ɁA�C�e���[�^��i�߂�
		}
	}

	//�ԗ����Z�b�g
	for (int i = 0; i < people; i++)
	{
		if (i == pNum)
			continue;	//�v���C���[�łȂ�

		//�����_���ɑI��
		int vehicleIndex = rand() % vehicleModelVector.size();
		int wheelIndex   = rand() % wheelModelVector.size();
		//�͈͓��Ɏ��߂�
		vehicleIndex = min(max(vehicleIndex, 0), vehicleModelVector.size() - 1);
		wheelIndex   = min(max(wheelIndex  , 0), wheelModelVector.size() - 1);
		
		VehicleOpponent* pVehicleOpponent = nullptr;
		SetVehicle<VehicleOpponent>(pVehicleOpponent
			, vehicleModelVector[vehicleIndex].c_str()
			, wheelModelVector[wheelIndex].c_str(), i);
	}

	//�v���C���[�͍Ō�
	VehiclePlayer* pVehiclePlayer = nullptr;
	SetVehicle<VehiclePlayer>(pVehiclePlayer, playerVehicleModelName
		, VehicleGlobal::GetChosenWheelName(), pNum);

	//�|�C���^���L��
	pVehiclePlayer_ = vehicles_[pNum];
}
