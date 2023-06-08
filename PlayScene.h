#pragma once
#include "Engine/GameObject.h"

class Ground;
class Vehicle;


//■■シーンを管理するクラス
class PlayScene : public GameObject
{
	int hImage_;
	int hModel_;

	Ground* pGround_;

	std::vector<Vehicle*> vehicleVector_;

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

};