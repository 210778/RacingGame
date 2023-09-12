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
#include "TextPrinter.h"

using std::tuple;
using std::sort;
using std::vector;

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	, pGround_(nullptr), pVehiclePlayer_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(5)
	, startFlag_(false)
{
}

//������
void PlayScene::Initialize()
{
	//Instantiate<Background>(this);
	pGround_ = Instantiate<Ground>(this);
	pGround_->CreateChosenCircuit(0);


	//���y
	Music::Initialize();

#if 0
	//�������t�@�C���isetup.ini�j����K�v�ȏ����擾
	int h = GetPrivateProfileInt("GAM", "Fp", 656565, ".\\setup.ini");
	//�^�C�g���o�[�ɕ\��������e
	std::string moji;
	char caption[MAX_PATH];
	GetPrivateProfileString("moji", "alfa", "***", caption, MAX_PATH, ".\\setup.ini");
	moji = caption;
#endif

	int population = 2;
	int playerNumber = 0;

	//�l��
	if (population < 1)
	{
		population = 1;
	}
	if (population >= pGround_->GetCircuitUnion()->startTransform_.size())
	{
		population = pGround_->GetCircuitUnion()->startTransform_.size() - 1;
	}
	//����
	if (playerNumber < 0)
	{
		playerNumber = 0;
	}
	if (playerNumber > population - 1)
	{
		playerNumber = population - 1;
	}


	//�ԗ����Z�b�g
	for (int i = 0; i < population; i++)
	{
		if (i != playerNumber)
		{
			VehicleOpponent* pVehicleOpponent = nullptr;
			SetVehicle<VehicleOpponent>(pVehicleOpponent
				, "model\\car_race_1_red.fbx"
				, "model\\wheel_industry_1_yellow.fbx"
				, i);
		}
	}
	//�v���C���[�͍Ō�
	VehiclePlayer* pVehiclePlayer = nullptr;
	SetVehicle<VehiclePlayer>(pVehiclePlayer
		, "model\\car_race_1_blue.fbx"
		, "model\\wheel_race_1_white.fbx"
		, playerNumber);
	//�|�C���^���L��
	pVehiclePlayer_ = vehicles_[playerNumber];

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


	//�G�t�F�N�g�p
	ParticlePackage::Initialize();

	//����
	//Instantiate<TextPrinter>(this);
}

//�X�V
void PlayScene::Update()
{
	//�|�[�Y
	PlayPause();

	//�|�[�Y���Ă�Ȃ��~
	if (pauseFlag_)
	{
		return;
	}

	//���y
	Music::Update();

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
	pVehicle->SetPointCountMax(pGround_->GetCircuitUnion()->check_.size());
	pVehicle->SetLapMax(pGround_->GetCircuitUnion()->maxLap_);
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
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
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