#include <tuple>
#include <algorithm>
#include <directxcollision.h>
#include <directxmath.h>
#include <directxpackedvector.h>

#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"

#include "PlayScene.h"
#include "Sample.h"
#include "Vehicle.h"
#include "Ground.h"
#include "ParticlePackage.h"
#include "VehiclePlayer.h"
#include "VehicleOpponent.h"
#include "Music.h"
#include "Background.h"

using std::tuple;
using std::sort;
using std::vector;

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	, pGround_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(5)
{
}

//������
void PlayScene::Initialize()
{
	//Instantiate<Background>(this);
	pGround_ = Instantiate<Ground>(this);
	pGround_->SetChosenCircuit(1);


	//�G�t�F�N�g�p
	ParticlePackage::Initialize();
	//���y
	Music::Initialize();


	int population = 2;
	int playerNumber = 0;
	//�ԗ����Z�b�g
	for (int i = 0; i < population; i++)
	{
		if (i == playerNumber)
		{
			VehiclePlayer* pVehiclePlayer = nullptr;
			SetVehicle<VehiclePlayer>(pVehiclePlayer
				, "model\\car_race_1_blue.fbx"
				, "model\\wheel_race_1_white.fbx"
				, i);
		}
		else
		{
			VehicleOpponent* pVehicleOpponent = nullptr;
			SetVehicle<VehicleOpponent>(pVehicleOpponent
				, "model\\car_race_1_red.fbx"
				, "model\\wheel_industry_1_yellow.fbx"
				, i);
		}
	}
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

	standbyTime_ = standbySeconds_ * 60;

}

//�X�V
void PlayScene::Update()
{
	if (standbyTime_ > 0)
	{
		standbyTime_--;

		//pTextUI_->Print("std::to_string(stangroieahgoirea\nreagera\nareg\nreag\neraga\naerg\ngrrg");
	}
	if (standbyTime_ == 0)
	{
		for (auto& i : vehicles_)
		{
			(*i).SetOperationInvalid(false);
		}
	}
	

	//���y
	//Music::Update();

	//����
	CountUniversalTime();

	//����
	CalculateRanking();

	//�Փ�
	CollideVehicle();
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}

//�ԗ��N���X�̂��߂̏�����
template <class V>
V* PlayScene::VehicleInstantiate(GameObject* pParent, std::string vehicleName, std::string wheelName)
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
void PlayScene::SetVehicle(Vehicle* pVehicle, std::string vehicleName, std::string wheelName, int number)
{
	pVehicle = VehicleInstantiate<V>(this, vehicleName, wheelName);
	Transform setTra = pGround_->GetCircuitUnion()->startTransform_[number];

	vehicles_.push_back(pVehicle);
	pVehicle->SetPosition(pGround_->GetCircuitUnion()->startTransform_[number].position_);
	pVehicle->SetRotate(pGround_->GetCircuitUnion()->startTransform_[number].rotate_);
	pVehicle->SetStartTransform(pGround_->GetCircuitUnion()->startTransform_[number]);
	pVehicle->SetPointCountMax(pGround_->GetCircuitUnion()->checkPoint_.size());
	pVehicle->SetLapMax(pGround_->GetCircuitUnion()->maxLap_);
}

//�ԗ����Փ˂��Ă邩���ׂ�
void PlayScene::CollideVehicle()
{
#if 0
	Debug::TimerLogStart("�Փˁi�P�j");

	for (auto& first : vehicles_)
	{
		for (auto& second : vehicles_)
		{
			//����Ȃ��߂�
			if (first == second)
			{
				continue;
			}

			BoundingOrientedBox BOB1;
			BOB1.Center = first->GetBoundingBoxCenter();
			BOB1.Extents = first->GetBoundingBoxExtents();
			XMFLOAT3 rotate1 = first->GetRotate();
			XMStoreFloat4(&BOB1.Orientation, XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(rotate1.x), XMConvertToRadians(rotate1.y), XMConvertToRadians(rotate1.z)));
			
			BoundingOrientedBox BOB2;
			BOB2.Center = second->GetBoundingBoxCenter();
			BOB2.Extents = second->GetBoundingBoxExtents();
			XMFLOAT3 rotate2 = second->GetRotate();
			XMStoreFloat4(&BOB2.Orientation, XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(rotate2.x), XMConvertToRadians(rotate2.y), XMConvertToRadians(rotate2.z)));

			int answer = BOB1.Contains(BOB2);
			//��������A���邢�͊܂܂��
			if (answer != 0)
			{
				first->CollideBoundingBox(second);
				second->CollideBoundingBox(first);
			}
		}
	}
	Debug::TimerLogEnd("�Փˁi�P�j");
#endif

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
	//���Z �I�[�o�[�t���[�͍l���Ȃ�
	universalTime_++;

	for (const auto& itr : vehicles_)
	{
		itr->SetTime(universalTime_);
	}
}