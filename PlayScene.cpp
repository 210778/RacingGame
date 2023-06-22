#include <tuple>
#include <algorithm>

#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

#include "PlayScene.h"
#include "Sample.h"
#include "Vehicle.h"
#include "Ground.h"
#include "ParticlePackage.h"
#include "VehiclePlayer.h"
#include "VehicleOpponent.h"

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
				, "model\\wheel_test_1.fbx"
				, i);
		}
		else
		{
			VehicleOpponent* pVehicleOpponent = nullptr;
			SetVehicle<VehicleOpponent>(pVehicleOpponent
				, "model\\car_race_1_blue.fbx"
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


		std::get<lap>(ranking[i]) = -vehicles_[i]->GetLapCount();
		std::get<check>(ranking[i]) = -vehicles_[i]->GetPointCount();
		std::get<distance>(ranking[i]) = vehicles_[i]->GetNextCheckDistance();
		std::get<pointer>(ranking[i]) = vehicles_[i];
	}
	//�\�[�g
	sort(ranking.begin(), ranking.end());
	//���ʂ�������
	for (int i = 0; i < ranking.size(); i++)
	{
		std::get<pointer>(ranking[i])->SetRanking(i + 1);
	}
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