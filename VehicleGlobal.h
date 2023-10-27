#pragma once
#include "Engine/GameObject.h"

namespace VehicleGlobal
{
	std::string chosenVehicleName;
	std::string chosenWheelName;

	enum ObjectName
	{
		vehicle = 0,
		wheel,
	};

	std::vector<std::string> nameVector_;

	std::map<std::string,std::map<std::string, std::string>> objectVector_;



	//初期化
	void Initialize();

	//ファイルから車両とタイヤの名前読み込み
	void LoadVehicleObject();
};

