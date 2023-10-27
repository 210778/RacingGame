#pragma once
#include "Engine/GameObject.h"

namespace VehicleGlobal
{
	enum PartName
	{
		vehicle = 0,
		wheel,
	};

	//初期化
	void Initialize();

	//ファイルから車両とタイヤの名前読み込み
	void LoadVehicleObject();


	//セッターとゲッター
	void SetChosenPopulation(int value);
	int GetChosenPopulation();
	void SetChosenVehicleName(std::string str);
	std::string GetChosenVehicleName();
	void SetChosenWheelName(std::string str);
	std::string GetChosenWheelName();

	std::map<PartName, std::map<std::string, std::string>>* GetVehicleObjectVector();

	/// <summary>
	/// パーツの名前とモデル名を格納したベクターへのポインタを返す
	/// </summary>
	/// <param name="pn">車両→vehicle , タイヤ→wheel</param>
	/// <returns>vector[pair[車両パーツの名前、ファイルの名前]] </returns>
	std::vector<std::pair<std::string, std::string>>* GetVehicleNameVector(PartName pn);
};

