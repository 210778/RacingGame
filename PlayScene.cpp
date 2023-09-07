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

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	, pGround_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(5)
	, startFlag_(false)
{
}

//初期化
void PlayScene::Initialize()
{
	//Instantiate<Background>(this);
	pGround_ = Instantiate<Ground>(this);
	pGround_->CreateChosenCircuit(0);


	//音楽
	Music::Initialize();

	//初期化ファイル（setup.ini）から必要な情報を取得
	int h = GetPrivateProfileInt("GAM", "Fp", 656565, ".\\setup.ini");

	//タイトルバーに表示する内容
	std::string moji;
	char caption[MAX_PATH];
	GetPrivateProfileString("moji", "alfa", "***", caption, MAX_PATH, ".\\setup.ini");
	moji = caption;

	int population = 2;
	int playerNumber = 0;

	//人数
	if (population < 1)
	{
		population = 1;
	}
	if (population >= pGround_->GetCircuitUnion()->startTransform_.size())
	{
		population = pGround_->GetCircuitUnion()->startTransform_.size() - 1;
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


	//車両をセット
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
	//プレイヤーは最後
	VehiclePlayer* pVehiclePlayer = nullptr;
	SetVehicle<VehiclePlayer>(pVehiclePlayer
		, "model\\car_race_1_blue.fbx"
		, "model\\wheel_race_1_white.fbx"
		, playerNumber);


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

	//文字
	//Instantiate<TextPrinter>(this);
}

//更新
void PlayScene::Update()
{
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
	pVehicle->SetPointCountMax(pGround_->GetCircuitUnion()->check_.size());
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