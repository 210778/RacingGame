#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"

#include "Engine/Input.h"
#include "Engine/Text.h"


//�R���X�g���N�^
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
{
}

//������
void StartScene::Initialize()
{
	//���y
	Music::Initialize();
}

//�X�V
void StartScene::Update()
{
	//���y
	Music::Update();

	if(true || Input::IsKey(DIK_P))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//�`��
void StartScene::Draw()
{
}

//�J��
void StartScene::Release()
{
}