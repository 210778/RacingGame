#pragma once
#include "Engine/GameObject.h"

class Text;

//スタート画面シーンを管理するクラス
class StartScene : public GameObject
{
	int hImageArrow_;

	//文字
	Text* pTextCircuit_;
	int circuitIndex_;

	int chosenCircuit_;	//選んだコースの番号

	struct DataSelection
	{
		int index;
		int minValue;
		int maxValue;
		DataSelection()
		{
			index = 0;
			minValue = 0;
			maxValue = 0;
		}
		DataSelection(int ind,int minV,int maxV)
		{
			index = ind;
			minValue = minV;
			maxValue = maxV;
		}
		//index が最小値未満なら最小値に、最大値オーバーなら最大値にする
		void DataClamp()
		{
			index = min(max(index, minValue), maxValue);//clamp
		}
		//indexにvalueを加算する。最大値、最小値範囲に収まっていたらtrue。そうじゃないならfalse
		bool DataAddition(int value)
		{
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