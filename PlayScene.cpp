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

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	, pGround_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(5)
{
}

//初期化
void PlayScene::Initialize()
{
	//Instantiate<Background>(this);
	pGround_ = Instantiate<Ground>(this);
	pGround_->SetChosenCircuit(1);


	//エフェクト用
	ParticlePackage::Initialize();
	//音楽
	Music::Initialize();


	int population = 2;
	int playerNumber = 0;
	//車両をセット
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
	//参加人数をセット
	for (auto& i : vehicles_)
	{
		(*i).SetPopulation(vehicles_.size());
	}

	//動けない状態にする
	for (auto& i : vehicles_)
	{
		(*i).SetOperationInvalid(true);
	}

	standbyTime_ = standbySeconds_ * 60;

}

//更新
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
	

	//音楽
	//Music::Update();

	//時間
	CountUniversalTime();

	//順位
	CalculateRanking();

	//衝突
	CollideVehicle();
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
}

//車両クラスのための初期化
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

//車両を初期化して必要な値をセット
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

//車両が衝突してるか調べる
void PlayScene::CollideVehicle()
{
#if 0
	Debug::TimerLogStart("衝突（１）");

	for (auto& first : vehicles_)
	{
		for (auto& second : vehicles_)
		{
			//同一ならやめる
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
			//交差する、あるいは含まれる
			if (answer != 0)
			{
				first->CollideBoundingBox(second);
				second->CollideBoundingBox(first);
			}
		}
	}
	Debug::TimerLogEnd("衝突（１）");
#endif

	Debug::TimerLogStart("衝突");
	//重複なし
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

			//交差する、あるいは含まれる
			if (BOB1.Contains(BOB2) != 0)
			{
				//両方を起動
				vehicles_[one]->CollideBoundingBox(vehicles_[two]);
				vehicles_[two]->CollideBoundingBox(vehicles_[one]);
			}

		}
	}
	Debug::TimerLogEnd("衝突");

}

//順位を調べて車両に教える
void PlayScene::CalculateRanking()
{
	//車両の順位を計算
	//型を用意
	vector<tuple<int, int, int, float, Vehicle*>> ranking(vehicles_.size());
	for (int i = 0; i < ranking.size(); i++)
	{
		//ゴールしてるか？
		int goalRanking = vehicles_.size();
		vehicles_[i]->GetIsGoalRanking(&goalRanking);

		std::get<RankName::goalRank>(ranking[i]) = goalRanking;
		std::get<RankName::lap>(ranking[i]) = -vehicles_[i]->GetLapCount();
		std::get<RankName::check>(ranking[i]) = -vehicles_[i]->GetPointCount();
		std::get<RankName::distance>(ranking[i]) = vehicles_[i]->GetNextCheckDistance();
		std::get<RankName::pointer>(ranking[i]) = vehicles_[i];
	}

	//ソート
	sort(ranking.begin(), ranking.end());

	//順位を教える
	for (int i = 0; i < ranking.size(); i++)
	{
		std::get<RankName::pointer>(ranking[i])->SetRanking(i + 1);
	}
}

//時間を加算して車両に教える
void PlayScene::CountUniversalTime()
{
	//加算 オーバーフローは考えない
	universalTime_++;

	for (const auto& itr : vehicles_)
	{
		itr->SetTime(universalTime_);
	}
}