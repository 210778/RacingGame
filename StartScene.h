#pragma once
#include <map>
#include "Engine/GameObject.h"

class Text;

//スタート画面シーンを管理するクラス
class StartScene : public GameObject
{
	int hImageArrow_;

	//文字
	Text* pTextCircuit_;

	struct DataSelection
	{
		std::string title;
		int index;
		int minValue;
		int maxValue;
		DataSelection(){
			title = "";
			index = 0;
			minValue = 0;
			maxValue = 0;
		}
		DataSelection(std::string name,int ind,int minV,int maxV){
			title = name;
			index = ind;
			minValue = minV;
			maxValue = maxV;
		}
		void SetDataSelection(std::string name, int ind, int minV, int maxV) {
			title = name;
			index = ind;
			minValue = minV;
			maxValue = maxV;
		}
		//index が最小値未満なら最小値に、最大値オーバーなら最大値にする
		void DataClamp(){
			index = min(max(index, minValue), maxValue);
		}
		//indexにvalueを加算する。最大値、最小値範囲に収まっていたらtrue。そうじゃないならfalse
		bool DataAddition(int value){
			index += value;
			int check = index;
			DataClamp();

			if (check == index)
				return true;
			else
				return false;
		};
	};

	DataSelection circuitSelect_;

	std::map<int,DataSelection> dataSelection_;	//選択するデータのまとめ
	DataSelection selectIndex_;	//データの索引

	enum DataName
	{
		circuit = 0,
		population,
		vehicle,
		wheel,
		NameMax
	};

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	StartScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};