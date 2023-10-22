#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"

#include "Engine/Input.h"
#include "Engine/Text.h"


//コンストラクタ
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
{
}

//初期化
void StartScene::Initialize()
{
	//音楽
	Music::Initialize();
}

//更新
void StartScene::Update()
{
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
}

//開放
void StartScene::Release()
{
}