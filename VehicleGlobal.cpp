#include "VehicleGlobal.h"

using std::string;
using std::to_string;
using std::stoi;

namespace VehicleGlobal
{
};

//初期化
void VehicleGlobal::Initialize()
{
	nameVector_.push_back("vehicle");
	nameVector_.push_back("wheel");

	LoadVehicleObject();
}

//ファイルから車両とタイヤの名前読み込み
void VehicleGlobal::LoadVehicleObject()
{
	const int pathSize = MAX_PATH;
	std::map<string, string> objectMap;    //探索キーと値を入れる
	string fileName = ".\\gameObject.ini";
	string circuitModelExtension = ".fbx";
	string modelFilePath = Global::GetModelFileName();
	int limit = 100;

	for (auto& itr : nameVector_)
	{
		string app = itr;

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
					//objectMap[key] = name; //追加
					objectVector_[app][key] = name;
				}					
			}
		}
	}
	int a = 0;
	limit = a;
}
