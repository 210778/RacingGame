#include "sceneManager.h"

#include "Model.h"
#include "Image.h"
#include "Audio.h"

#include "../TestScene.h"
#include "../PlayScene.h"
#include "../StartScene.h"
#include "../Music.h"

//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_START;
	nextSceneID_ = currentSceneID_;
	Instantiate<StartScene>(this);

	//音楽
	//Music::Initialize();
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//次のシーンを作成
		switch (nextSceneID_)
		{
		default:
			break;
		case SCENE_ID_TEST: Instantiate<TestScene>(this);
			break;
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this);
			break;
		case SCENE_ID_START: Instantiate<StartScene>(this);
			break;
		}

		//初期化
		Audio::Initialize();
		//音楽
		Music::Initialize();

		currentSceneID_ = nextSceneID_;
	}
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}