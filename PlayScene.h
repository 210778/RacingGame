#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//■■シーンを管理するクラス
class PlayScene : public GameObject
{
	int hModel_;

	std::vector<Vehicle*> vehicles_;
	Vehicle* pVehiclePlayer_;

	unsigned long long universalTime_;	//車両の共通時間(単位：フレーム)

	unsigned long long standbyTime_;	//発車前の待機時間(単位：フレーム)
	unsigned int	   standbySeconds_;	//何秒発射前に待つのか(単位：フレーム * 60)
	bool startFlag_;	//スタートしたか
	bool pauseFlag_;	//false -> 通常、 true -> ポーズ中

	enum RankName
	{
		goalRank = 0,
		lap,
		check,
		distance,
		pointer
	};

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//車両クラスのための初期化
	template <class V>
	V* VehicleInstantiate(GameObject* pParent, std::string vehicleName, std::string wheelName);

	//車両を初期化して必要な値をセット
	template <class V>
	void SetVehicle(Vehicle* pVehicle, std::string vehicleName, std::string wheelName,int number);

	//車両が衝突してるか調べる
	void CollideVehicle();

	//順位を調べて車両に教える
	void CalculateRanking();

	//時間を加算して車両に教える
	void CountUniversalTime();

	//ポーズ
	void PlayPause();

	//NPCにプレイヤーまでの距離を教える
	void SetNPCToPlayer();
	
	/// <summary>
	/// プレイヤーとNPCをセット
	/// </summary>
	/// <param name="population">人数</param>
	/// <param name="playerNumber">プレイヤーの位置</param>
	void SetPlayerNPCVector(int population, int playerNumber);
};