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
#include "Circuit.h"


using std::tuple;
using std::sort;
using std::vector;

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	, pGround_(nullptr), pVehiclePlayer_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(5)//5
	, startFlag_(false)
{
}

//初期化
void PlayScene::Initialize()
{
	Instantiate<Background>(this);

	Circuit::ResetCircuitModelHandle();
	Circuit::CreateChosenCircuit(this);
	//Circuit::SetChosenCircuit(0);

	//音楽
	//Music::Initialize();

	int population = 2;
	int playerNumber = 0;

	//人数
	if (population < 1)
	{
		population = 1;
	}
	if (population >= Circuit::GetChosenCircuit()->startTransform_.size())
	{
		population = Circuit::GetChosenCircuit()->startTransform_.size() - 1;
	}
	//順番
	if (playerNumber < 0)
	{
		playerNumber = 0;
	}
	if (playerNumber > population - 1)
	{
		playerNumber = population - 1;
	}

	//配列
	vehicles_.clear();
	vehicles_.resize(population);

	//車両をセット
	for (int i = 0; i < population; i++)
	{
		if (i == playerNumber)
			continue;	//プレイヤーでない

		VehicleOpponent* pVehicleOpponent = nullptr;
		SetVehicle<VehicleOpponent>(pVehicleOpponent
			, "model\\car_race_1_red.fbx"
			, "model\\wheel_industry_1_yellow.fbx", i);
	}

	//プレイヤーは最後
	VehiclePlayer* pVehiclePlayer = nullptr;
	SetVehicle<VehiclePlayer>(pVehiclePlayer
		, "model\\car_race_1_blue.fbx"
		, "model\\wheel_race_1_white.fbx", playerNumber);
	//ポインタを記憶
	pVehiclePlayer_ = vehicles_[playerNumber];


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

	//待機時間
	standbyTime_ = standbySeconds_ * Global::GetStandardFPS();

	//エフェクト用
	ParticlePackage::Initialize();
}

//更新
void PlayScene::Update()
{
	//ポーズ
	PlayPause();

	//ポーズしてるなら停止
	if (pauseFlag_)
	{
		return;
	}

	//音楽
	Music::Update();

	//時間
	CountUniversalTime();

	//プレイヤーまでの距離
	SetNPCToPlayer();

	//順位
	CalculateRanking();

	//衝突
	CollideVehicle();
}

//描画
void PlayScene::Draw()
{
	Circuit::Draw();
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

//車両が衝突してるか調べる
void PlayScene::CollideVehicle()
{
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
		//加算 オーバーフローは考えない
		universalTime_++;

		for (auto& itr : vehicles_)
		{
			itr->SetTime(universalTime_);
		}
	}

	//車両を操作可能にする
	if (standbyTime_ <= 0 && startFlag_ == false)
	{
		startFlag_ = true;

		for (auto& i : vehicles_)
		{
			(*i).SetOperationInvalid(false);
		}
	}
}

//ポーズ
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

//NPCにプレイヤーまでの距離を教える
void PlayScene::SetNPCToPlayer()
{
	if (pVehiclePlayer_ == nullptr)
		return;

	for (auto& itr : vehicles_)
	{
		if (itr == pVehiclePlayer_)
			continue;	//プレイヤーは無視

		XMFLOAT3 pPos = pVehiclePlayer_->GetPosition();
		XMFLOAT3 nPos = itr->GetPosition();
		float leng = *XMVector3Length(XMLoadFloat3(&pPos) - XMLoadFloat3(&nPos)).m128_f32;

		itr->SetToPlayerVehicleLength(*XMVector3Length(XMLoadFloat3(&pPos) - XMLoadFloat3(&nPos)).m128_f32);
	}
}