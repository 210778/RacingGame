#pragma once
#include "Engine/GameObject.h"

class Text;

//スタート画面シーンを管理するクラス
class StartScene : public GameObject
{
	//文字
	Text* pTextCircuit_;

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