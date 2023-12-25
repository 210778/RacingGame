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

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
	, pVehiclePlayer_(nullptr)
	, universalTime_(0), standbyTime_(1), standbySeconds_(6)
	, startFlag_(false), pauseFlag_(false), playerGoalFlag_(false)
	, playMusic_(Music::MusicName::se_se_selectError)
{
}

//初期化
void PlayScene::Initialize()
{
	Circuit::ResetCircuitModelHandle();	//コース初期化
	Circuit::CreateChosenCircuit(this); //コース生成

	int population = VehicleGlobal::GetChosenPopulation();	//人数
	int playerNumber = population - 1;	//プレイヤーは一番後ろ

	//プレイヤーとNPCをセット
	SetPlayerNPCVector(population, playerNumber);

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

	//エフェクト初期化
	ParticlePackage::Initialize();
}

//更新
void PlayScene::Update()
{
	//音楽
	//スタート後の曲
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
	//ゴールしたら
	if (!playerGoalFlag_ && pVehiclePlayer_->GetGoalFlag())
	{
		Music::Stop(playMusic_);
		playerGoalFlag_ = true;
	}
	//ゴール曲
	if (playerGoalFlag_)
	{
		Music::Play(Music::MusicName::bgm_goal);
	}

	Music::Update();

	//操作
	VehicleInput::Update();

	//ポーズ
	PlayPause();

	//タイトル画面
	if (pauseFlag_ && VehicleInput::GetInput(VehicleInput::Button::selectDecide))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_START);
	}

	//ポーズしてるなら停止
	if (pauseFlag_)
	{
		return;
	}

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

//車両を初期化して必要な値をセット
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
	if (VehicleInput::GetInput(VehicleInput::Button::pause))
	{
		Music::Play(Music::MusicName::se_select2);	//音声

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

void PlayScene::SetPlayerNPCVector(int population, int playerNumber)
{
	//人数
	int people = min(max(population, 1), Circuit::GetChosenCircuit()->startTransform_.size());
	//順番
	int pNum = min(max(playerNumber, 0), population - 1);

	//配列
	vehicles_.clear();
	vehicles_.resize(population);

	//プレイヤーのモデル
	string playerVehicleModelName = VehicleGlobal::GetChosenVehicleName();

	//モデル名が入ってる配列を作る
	//車両
	vector<string> vehicleModelVector;
	for (auto& itr : *VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle))
	{
		vehicleModelVector.push_back(itr.second);
	}
	//タイヤ
	vector<string> wheelModelVector;
	for (auto& itr : *VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel))
	{
		wheelModelVector.push_back(itr.second);
	}

	//配列のサイズが１以上ならプレイヤーと被ってるモデル名は消す　タイヤは被っても問題ない
	if (vehicleModelVector.size() >= 1)
	{
		// イテレート中に要素削除をするような場合には範囲for文は使用できない
		for (auto itr = vehicleModelVector.begin(); itr != vehicleModelVector.end();)
		{
			// 条件一致した要素を削除する
			if (*itr == playerVehicleModelName)
				itr = vehicleModelVector.erase(itr);// 削除された要素の次を指すイテレータが返される。
			else
				++itr;// 要素削除をしない場合に、イテレータを進める
		}
	}

	//車両をセット
	for (int i = 0; i < people; i++)
	{
		if (i == pNum)
			continue;	//プレイヤーでない

		//ランダムに選ぶ
		int vehicleIndex = rand() % vehicleModelVector.size();
		int wheelIndex   = rand() % wheelModelVector.size();
		//範囲内に収める
		vehicleIndex = min(max(vehicleIndex, 0), vehicleModelVector.size() - 1);
		wheelIndex   = min(max(wheelIndex  , 0), wheelModelVector.size() - 1);
		
		VehicleOpponent* pVehicleOpponent = nullptr;
		SetVehicle<VehicleOpponent>(pVehicleOpponent
			, vehicleModelVector[vehicleIndex].c_str()
			, wheelModelVector[wheelIndex].c_str(), i);
	}

	//プレイヤーは最後
	VehiclePlayer* pVehiclePlayer = nullptr;
	SetVehicle<VehiclePlayer>(pVehiclePlayer, playerVehicleModelName
		, VehicleGlobal::GetChosenWheelName(), pNum);

	//ポインタを記憶
	pVehiclePlayer_ = vehicles_[pNum];
}
