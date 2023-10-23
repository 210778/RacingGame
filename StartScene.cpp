#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"

#include "Engine/Input.h"
#include "Engine/Text.h"

using std::string;
using std::to_string;
using std::stoi;

//コンストラクタ
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene"),
	pTextCircuit_(nullptr)
{
}

//初期化
void StartScene::Initialize()
{
	Circuit::Initialize();

	//文字
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();
}

//更新
void StartScene::Update()
{
	Circuit::SetChosenCircuit(0);

	//音楽
	Music::Update();

	if(true || Input::IsKey(DIK_P))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//描画
void StartScene::Draw()
{
	int height = 30;

	for (auto& itr : *Circuit::GetCircuitNameArray())
	{
		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height, itr.c_str());

		height += 40;
	}

}

//開放
void StartScene::Release()
{
	pTextCircuit_->Release();

}