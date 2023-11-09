#include "Engine/Global.h"
#include "VehicleGlobal.h"
using std::string;
using std::to_string;
using std::stoi;
using std::pair;
using std::make_pair;
using std::map;
using std::vector;

namespace VehicleGlobal
{
	int chosenPopulation_;
	string chosenVehicleName_;
	string chosenWheelName_;

	vector<pair<string, string>> answerMap_;//返すmap

	vector<pair<PartName,string>> partNameVector_;

	map<PartName, map<string, string>> vehicleObjectVector_;
};

//初期化
void VehicleGlobal::Initialize()
{
	//初期化
	if (!(partNameVector_.empty()))
	{
		return;
	}

	partNameVector_.push_back(make_pair(PartName::vehicle, "vehicle"));
	partNameVector_.push_back(make_pair(PartName::wheel, "wheel"));

	//ファイルから読み込み
	LoadVehicleObject();
}

//ファイルから車両とタイヤの名前読み込み
void VehicleGlobal::LoadVehicleObject()
{
	const int pathSize = MAX_PATH;
	map<string, string> objectMap;    //探索キーと値を入れる
	string fileName = ".\\gameObject.ini";
	string circuitModelExtension = ".fbx";
	int limit = 100;

	for (auto& itr : partNameVector_)
	{
		string app = itr.second;
		PartName appName = itr.first;

		for (int number = 0; number < limit; number++)
		{
			char section[pathSize];
			DWORD result = GetPrivateProfileSection(app.c_str(), section, pathSize, fileName.c_str());
			if (result >= (pathSize - 2) || result <= 0)
				continue; // サイズオーバーエラー

			int start = 0;  //開始位置
			for (int i = 0; i < result; i++)
			{
				string key = "";
				string name = "";
				bool equal = false;
				for (int j = start; j < result; j++)
				{
					if (section[j] == '\0')
					{
						start = j + 1;
						break;
					}
					else if (section[j] == '=')
					{
						if (!equal)
							equal = true;
					}
					else
					{
						if (equal)
							name += section[j];
						else
							key += section[j];
					}
				}
				if (!(key.empty()) && !(name.empty()))
				{
					vehicleObjectVector_[appName][key] = name;
				}					
			}
		}
	}
}

//セッターとゲッター
void VehicleGlobal::SetChosenPopulation(int value) { chosenPopulation_ = value; }
int VehicleGlobal::GetChosenPopulation() { return chosenPopulation_; }

void VehicleGlobal::SetChosenVehicleName(string str) { chosenVehicleName_ = str; }
string VehicleGlobal::GetChosenVehicleName() { return chosenVehicleName_; }

void VehicleGlobal::SetChosenWheelName(string str) { chosenWheelName_ = str; }
string VehicleGlobal::GetChosenWheelName() { return chosenWheelName_; }

//パーツの名前とモデル名を格納したベクターへのポインタを返す
vector<pair<string, string>>* VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName pn)
{
	answerMap_.clear();//初期化

	for (auto& itr : vehicleObjectVector_[pn])
	{
		answerMap_.push_back(make_pair(itr.first, itr.second));
	}
	
	return &answerMap_;
}
