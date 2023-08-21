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

using std::tuple;
using std::sort;
using std::vector;

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	,pGround_(nullptr)
{
}

//������
void PlayScene::Initialize()
{
	hImage_ = Image::Load("image\\skyField.png");
	assert(hImage_ >= 0);
	
	hModel_ = Model::Load("model\\SkyFieldSphere.fbx");
	//hModel_ = Model::Load("model\\CarRed2.fbx");
	assert(hModel_ >= 0);

	pGround_ = Instantiate<Ground>(this);
	pGround_->SetChosenCircuit(0);


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
}

//�X�V
void PlayScene::Update()
{
	//�ԗ��̏��ʂ��v�Z
	//�^��p��
	vector<tuple<int, int, float, Vehicle*>> ranking(vehicles_.size());
	for (int i = 0; i < ranking.size(); i++)
	{
		int la = vehicles_[i]->GetLapCount();
		int point = vehicles_[i]->GetPointCount();
		float dist = vehicles_[i]->GetNextCheckDistance();

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


	//�Փ�
	CollideVehicle();
}

//�`��
void PlayScene::Draw()
{
#if 1
		Transform ImaTra = transform_;
		ImaTra.scale_ = XMFLOAT3(0.25f, 0.25f, 0.25f);
		Image::SetTransform(hImage_, transform_);
		Image::Draw(hImage_);
#else
		Transform modTra = transform_;
		modTra.scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		modTra.rotate_.y = 180;
		Model::SetTransform(hModel_, modTra);
		Model::Draw(hModel_);
#endif
	
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
			////�O��t
			//answer = BOB2.Contains(BOB1);			
			//if (answer != 0)
			//{
			//	first->CollideBoundingBox(second);
			//}

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